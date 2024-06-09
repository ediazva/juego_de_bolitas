#include "Mice.h"

#include <mice/mice.h>

static std::shared_ptr<Mice> g_mice = nullptr;
static JavaVM* g_jvm = NULL;
static jobject g_obj = NULL;

inline static jobject get_member_from_instance(JNIEnv* env, const char* name, const char* sig) {
   jclass objClass;
   jfieldID fieldID;
   jobject res;
   
   objClass = env->GetObjectClass(g_obj);
   fieldID = env->GetFieldID(objClass, name, sig);
   res = env->GetObjectField(g_obj, fieldID);

   env->DeleteLocalRef(objClass);

   return res;
}

inline static jobject mouse_to_java(JNIEnv* env, const Mice::Mouse& mouse) {
   jclass mouseClass = env->FindClass("Mouse");
   jmethodID mouseInit = env->GetMethodID(mouseClass, "<init>", "()V");
   jfieldID mouseFieldX = env->GetFieldID(mouseClass, "x", "F");
   jfieldID mouseFieldY = env->GetFieldID(mouseClass, "y", "F");
   jfieldID mouseFieldButton = env->GetFieldID(mouseClass, "button", "I");
   jfieldID mouseFieldButtonState = env->GetFieldID(mouseClass, "button_state", "I");
   jobject mouseObj = env->NewObject(mouseClass, mouseInit);

   env->SetFloatField(mouseObj, mouseFieldX, mouse.rel_x);
   env->SetFloatField(mouseObj, mouseFieldY, mouse.rel_y);
   env->SetIntField(mouseObj, mouseFieldButton, (int)mouse.button);
   env->SetIntField(mouseObj, mouseFieldButtonState, (int)mouse.button_state);

   env->DeleteLocalRef(mouseClass);
   return mouseObj;
}

inline static void call_onmouseaddedcallback(JNIEnv* env, const char* id) {
   jclass objClass = env->GetObjectClass(g_obj);
   jmethodID methodID = env->GetMethodID(objClass, "onMouseAdded", "(Ljava/lang/String;)V");

   jstring value = env->NewStringUTF(id);
   env->CallVoidMethod(g_obj, methodID, value);

   env->DeleteLocalRef(value);
   env->DeleteLocalRef(objClass);
}

inline static void call_onmouseremovedcallback(JNIEnv* env, const char* id) {
   jclass objClass = env->GetObjectClass(g_obj);
   jmethodID methodID = env->GetMethodID(objClass, "onMouseRemoved", "(Ljava/lang/String;)V");

   jstring value = env->NewStringUTF(id);
   env->CallVoidMethod(g_obj, methodID, value);

   env->DeleteLocalRef(value);
   env->DeleteLocalRef(objClass);
}

inline static void call_onmouseventcallback(JNIEnv* env, const Mice::Mouse& m) {
   jclass objClass = env->GetObjectClass(g_obj);
   jmethodID methodID = env->GetMethodID(objClass, "onMouseEvent", "(Ljava/lang/String;LMouse;)V");

   jstring value = env->NewStringUTF(m.id());
   jobject mouseJava = mouse_to_java(env, m);
   env->CallVoidMethod(g_obj, methodID, value, mouseJava);

   env->DeleteLocalRef(mouseJava);
   env->DeleteLocalRef(value);
   env->DeleteLocalRef(objClass);
}

JNIEXPORT jboolean JNICALL Java_Mice_tryBuild
  (JNIEnv * env, jobject obj) {
   if(g_mice)
      return true;

   g_mice = Mice::MakeFromSystem();
   if(!g_mice)
      return false;

   env->GetJavaVM(&g_jvm);
   g_obj = env->NewGlobalRef(obj);
   g_mice->onMouseAdded = [] (const char* id) {
      JNIEnv* env = NULL;
      if(g_jvm->AttachCurrentThread((void**)&env, NULL) < 0)
         puts("Failed to attach JVM");
      call_onmouseaddedcallback(env, id);
      g_jvm->DetachCurrentThread();
   };
   g_mice->onEvent = [] (const Mice::Mouse& m) {
      JNIEnv* env = NULL;
      if(g_jvm->AttachCurrentThread((void**)&env, NULL) < 0)
         puts("Failed to attach JVM");
      call_onmouseventcallback(env, m);
      g_jvm->DetachCurrentThread();
   };
   g_mice->onMouseRemoved = [] (const char* id) {
      JNIEnv* env = NULL;
      if(g_jvm->AttachCurrentThread((void**)&env, NULL) < 0)
         puts("Failed to attach JVM");
      call_onmouseremovedcallback(env, id);
      g_jvm->DetachCurrentThread();
   };
   
   return true;
}

JNIEXPORT void JNICALL Java_Mice_pollEvents
  (JNIEnv *, jobject) {
   if(!g_mice)
      return;
   
   g_mice->pollEvents();
}
