cmake_minimum_required(VERSION 3.1)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

set(CMAKE_CXX_STANDARD 20)
project(rtaudio-js)
set(CMAKE_DEBUG_POSTFIX "")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

if(WIN32)
  option(RTAUDIO_API_DS "" ON)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX "-win32.node")
endif()

if(LINUX)
  option(RTAUDIO_API_PULSE "" OFF)
  option(RTAUDIO_API_JACK "" OFF)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX "-linux-alsa.node")

  # if(DEFINED ENV{RTAUDIO_JS_LINUX_ALSA_ONLY})
  #   option(RTAUDIO_API_PULSE "" OFF)
  #   option(RTAUDIO_API_JACK "" OFF)
  #   set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX "-linux-alsa.node")
  # elseif(DEFINED ENV{RTAUDIO_JS_LINUX_PULSE_ALSA})
  #   option(RTAUDIO_API_PULSE "" ON)
  #   option(RTAUDIO_API_JACK "" OFF)
  #   set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX "-linux-pulse-alsa.node")
  # else()
  #   set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX "-linux-custom.node")
  # endif()
endif()

# Use relative RPATH=$ORIGIN for portability.
# This will allow the addon to use the librtaudio.so located in the same directory on unix-like systems.
# On Windows, rtaudio.dll in the same directory is already visible to the addon.
if(UNIX)
  set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_RPATH_USE_ORIGIN TRUE)
endif()

add_subdirectory(vendor/rtaudio rtaudio)
include_directories(vendor/rtaudio)

# Disable symlinks for portability.
if(UNIX)
  set_property(TARGET rtaudio PROPERTY SOVERSION)
  set_property(TARGET rtaudio PROPERTY VERSION)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
add_compile_definitions(NAPI_ENABLE_CPP_EXCEPTIONS)
add_compile_definitions(NODE_ADDON_API_DISABLE_DEPRECATED)
add_compile_definitions(NAPI_VERSION=4)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

execute_process(COMMAND node -p "require('node-addon-api').include"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE NODE_ADDON_API_DIR
)

string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBS} ${CMAKE_JS_LIB} rtaudio)

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()
