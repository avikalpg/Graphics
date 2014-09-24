# Install script for directory: /home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3.0.1264"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/lib/libassimp.so.3.0.1264"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/lib/libassimp.so.3"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/lib/libassimp.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3.0.1264"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/anim.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/ai_assert.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/camera.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/color4.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/color4.inl"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/config.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/defs.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/cfileio.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/light.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/material.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/material.inl"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/matrix3x3.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/matrix3x3.inl"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/matrix4x4.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/matrix4x4.inl"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/mesh.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/postprocess.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/quaternion.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/quaternion.inl"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/scene.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/texture.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/types.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/vector2.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/vector2.inl"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/vector3.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/vector3.inl"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/version.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/cimport.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/importerdesc.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/Importer.hpp"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/DefaultLogger.hpp"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/ProgressHandler.hpp"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/IOStream.hpp"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/IOSystem.hpp"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/Logger.hpp"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/LogStream.hpp"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/NullLogger.hpp"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/cexport.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/Exporter.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/Compiler/pushpack1.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/../include/assimp/Compiler/poppack1.h"
    "/home/avikalpg/Documents/Graphics/assimp--3.0.1270-sdk/code/pstdint.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")

