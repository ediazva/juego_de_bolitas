cmake_minimum_required(VERSION 3.2)

project(juego_de_bolitas)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(JAVA_HOME "/usr/lib/jvm/java-17-openjdk-amd64")
find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

set(MICE_EXAMPLE OFF)
add_subdirectory(./mice)

set(MICE_DESTINATION_DIR "/home/phobos/sketchbook/juego_de_bolitas/code")

add_jar(Mice
   SOURCES Mice.java Mouse.java
   OUTPUT_DIR ${MICE_DESTINATION_DIR}
   GENERATE_NATIVE_HEADERS Mice-native DESTINATION ${CMAKE_SOURCE_DIR})

add_library(Mice-lib SHARED Mice.cc)
target_link_libraries(Mice-lib Mice-native mice-lib)
set_target_properties(Mice-lib PROPERTIES
   LIBRARY_OUTPUT_DIRECTORY ${MICE_DESTINATION_DIR}
)