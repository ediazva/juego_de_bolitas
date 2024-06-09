public class Mice {
   private OnMouseRemovedCallback m_mouseRemovedCallback;
   private OnMouseAddedCallback m_mouseAddedCallback;
   private OnMouseEventCallback m_mouseEventCallback;

   static {
      System.loadLibrary("Mice-lib");
   }
   
   public void setOnMouseRemovedCallback(OnMouseRemovedCallback callback) {m_mouseRemovedCallback = callback;}
   public void setOnMouseAddedCallback(OnMouseAddedCallback callback) {m_mouseAddedCallback = callback;}
   public void setOnMouseEventCallback(OnMouseEventCallback callback) {m_mouseEventCallback = callback;}
   private void onMouseAdded(String id) {
      if(m_mouseAddedCallback != null)
         m_mouseAddedCallback.onMouseAdded(id);
   }
   private void onMouseEvent(String id, Mouse m) {
      if(m_mouseEventCallback != null)
         m_mouseEventCallback.onMouseEvent(id, m);
   }
   private void onMouseRemoved(String id) {
      if(m_mouseRemovedCallback != null)
         m_mouseRemovedCallback.onMouseRemoved(id);
   }
   public native boolean tryBuild();
   public native void pollEvents();
}