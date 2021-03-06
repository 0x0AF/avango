################################################################
# Avango
################################################################
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
project(AVANGO CXX)

# version number
set(AVANGO_VERSION_MAJOR 2)
set(AVANGO_VERSION_MINOR 11)
set(AVANGO_VERSION_MAINT 11)

set(AVANGO_VERSION
    ${AVANGO_VERSION_MAJOR}.${AVANGO_VERSION_MINOR}.${AVANGO_VERSION_MAINT})
set(AVANGO_DESCRIPTION "AVANGO")
set(AVANGO_HOMEPAGE "https://github.com/vrsys")
set(AVANGO_EXENAME "AVANGO")
set(AVANGO_PACKAGENAME "AVANGO")

# We require at least version 2.8.0
cmake_minimum_required(VERSION 2.8.0)

if (UNIX)
  find_package(PkgConfig)
endif(UNIX)

# Location where cmake first looks for modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

################################################################
# Configure and find libraries
################################################################
set(GLOBAL_EXT_DIR ${AVANGO_SOURCE_DIR}/externals)

if(MSVC)
    option(CONTAINER_DEPLOYMENT_BUILD "Set to enable Docker-based deployment behaviors" ON)
endif()

if(CONTAINER_DEPLOYMENT_BUILD)
    set(GLOBAL_EXT_DIR ${DEPLOYMENT_EXTERNALS})
endif(CONTAINER_DEPLOYMENT_BUILD)


if (UNIX)
  pkg_check_modules(GL REQUIRED gl)
endif (UNIX)

include(find_compiler)
include(file_copy_macro)

include(find_guacamole)

#if (AVANGO_PBR_SUPPORT)
#  include(FindGuacamolePLOD)
#endif (AVANGO_PBR_SUPPORT)

set(GUACAMOLE_EXT_DIR ${GUACAMOLE_INCLUDE_DIRS}/../externals)

if (UNIX)
  pkg_check_modules(JSON REQUIRED jsoncpp)
  pkg_check_modules(PYTHON3 REQUIRED python-3.5)
elseif (WIN32)
  include(findAssimp)
  include(findJson)
  include(findPythonLibs)
endif (UNIX)

if (UNIX)
  set (BOOST_ROOT "/opt/boost/current" CACHE PATH "Set to your boost install")
  find_package(Boost COMPONENTS python-py35 thread signals system REQUIRED)
  set(Boost_PYTHON3_LIBRARY_RELEASE ${Boost_PYTHON-PY35_LIBRARY_RELEASE} CACHE FILEPATH "")
  set(Boost_PYTHON3_LIBRARY_DEBUG ${Boost_PYTHON-PY35_LIBRARY_DEBUG} CACHE FILEPATH "")
elseif (MSVC)
  set (BOOST_ROOT ${GLOBAL_EXT_DIR}/boost CACHE PATH "Set to your boost install")
  message(STATUS "Set BOOST_ROOT to ${BOOST_ROOT}")
  set (Boost_USE_MULTITHREADED ON)
  #set (Boost_USE_STATIC_LIBS ON)
  ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")
  find_package(Boost COMPONENTS python3 thread signals system REQUIRED)
endif(UNIX)

set (BULLET_ROOT
     /opt/bullet/default
     ${GLOBAL_EXT_DIR}/bullet
     CACHE PATH
    "Set to your bullet install path.")
find_package(Bullet)

 if (UNIX)
                if (BULLET_COLLISION_LIBRARY)
                  unset(BULLET_COLLISION_LIBRARY_DEBUG CACHE)
                  set(BULLET_COLLISION_LIBRARY_DEBUG ${BULLET_COLLISION_LIBRARY} CACHE FILEPATH "")
                endif (BULLET_COLLISION_LIBRARY)

                if (BULLET_HACD_LIBRARY)
                  unset(BULLET_HACD_LIBRARY_DEBUG CACHE) 
                  set(BULLET_HACD_LIBRARY_DEBUG ${BULLET_HACD_LIBRARY} CACHE FILEPATH "")
                endif (BULLET_HACD_LIBRARY)

                if (BULLET_DYNAMICS_LIBRARY)
                  unset(BULLET_DYNAMICS_LIBRARY_DEBUG CACHE) 
                  set(BULLET_DYNAMICS_LIBRARY_DEBUG ${BULLET_DYNAMICS_LIBRARY} CACHE FILEPATH "")
                endif (BULLET_DYNAMICS_LIBRARY)

                if (BULLET_MATH_LIBRARY)
                  unset(BULLET_MATH_LIBRARY_DEBUG CACHE) 
                  set(BULLET_MATH_LIBRARY_DEBUG ${BULLET_MATH_LIBRARY} CACHE FILEPATH "")
                endif (BULLET_MATH_LIBRARY)

                if (BULLET_SOFTBODY_LIBRARY)
                  unset(BULLET_SOFTBODY_LIBRARY_DEBUG CACHE) 
                  set(BULLET_SOFTBODY_LIBRARY_DEBUG ${BULLET_SOFTBODY_LIBRARY} CACHE FILEPATH "")
                endif (BULLET_SOFTBODY_LIBRARY)
        endif(UNIX)

include(FindSchism)

include(FindZMQ)

if (AVANGO_SPOINTS_SUPPORT)
include(FindTurboJPEG)
endif (AVANGO_SPOINTS_SUPPORT)

if (${AVANGO_OCULUS_RIFT_SUPPORT})
  include(findOVR)
endif(${AVANGO_OCULUS_RIFT_SUPPORT})

if (${AVANGO_VIVE_SUPPORT})
  include(FindOpenVR)
endif(${AVANGO_VIVE_SUPPORT})

set(LIB_PATHS
  ${GUACAMOLE_LIBRARY_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${GL_LIBRARY_DIRS}
)
# allow linking with local libraries
link_directories(${LIB_PATHS})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
################################################################
# preprocessor configuration
################################################################
if (UNIX)
    set(CMAKE_CXX_FLAGS_RELEASE "-s -O4 --std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall --std=c++11")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "-D NDEBUG -D WIN32_LEAN_AND_MEAN -D _SECURE_SCL=0 -D _SCL_SECURE_NO_WARNINGS -D _CRT_SECURE_NO_DEPRECATE /MD /MP")
    set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -D WIN32_LEAN_AND_MEAN /MDd /Zi")
    add_definitions(-D AV_INSTANTIATE_FIELD_TEMPLATES -D NOMINMAX)
endif(UNIX)

################################################################
# Avango Configuration
################################################################
set (AVANGO_LOG_LEVEL "WARN" CACHE STRING
    "Set AvangoNG log level: FATAL, ERROR, WARN, INFO, DEBUG, TRACE")
set (AVANGO_INSTALL_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib CACHE PATH
    "Install path for avango.")

option (AVANGO_DEBUG "Set to enable avango debugging.")
option (AVANGO_DISTRIBUTION_SUPPORT "Set to enable distribution support." ON)
option (AVANGO_PBR_SUPPORT "Set to enable point based rendering support.")
option (AVANGO_LOD_SUPPORT "Set to enable level of detail support.")
option (AVANGO_NURBS_SUPPORT "Set to enable direct NURBS rendering." OFF)
option (AVANGO_NRP_SUPPORT "Set to enable interface for the Neurorobotics Platform." OFF)
option (AVANGO_PCL_SUPPORT "Use PCL.")
option (AVANGO_UNITTESTS "Compile Unittests for Avango.")
option (AVANGO_OCULUS_RIFT_SUPPORT "Compile Oculus Rift module for Avango.")
option (AVANGO_VIVE_SUPPORT "Compile Vive module for Avango.")
option (AVANGO_VOLUME_SUPPORT "Compile Volume module for Avango.")
option (AVANGO_SKELANIM_SUPPORT "Compile Skeletal animation module for Avango.")
option (AVANGO_SPOINTS_SUPPORT "Compile SPoints module.")
option (AVANGO_OPENAL_SUPPORT "Enable building OpenAL sound module.")
option (AVANGO_AWESOMIUM_SUPPORT "Enable building Awesomium module.")
option (AVANGO_DISPLAY_SUPPORT "Enable building display setup module.")
option (AVANGO_PHYSICS_SUPPORT "Set to enable physics support.")
option (AVANGO_RUNTIME_PROGRAM_COMPILATION "Set to enable runtime program compilation." ON)
option (AVANGO_TV_3_SUPPORT "Compile time-varying volume rendering support.")
option (AVANGO_VIDEO3D_SUPPORT "Compile Video3d module.")
option (AVANGO_VIRTUAL_TEXTURING_SUPPORT "Set to enable virtual texturing support (requires lamure-vt and guacamole-vt)." OFF)

IF (AVANGO_RUNTIME_PROGRAM_COMPILATION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGUACAMOLE_RUNTIME_PROGRAM_COMPILATION")
ENDIF (AVANGO_RUNTIME_PROGRAM_COMPILATION)

################################################################
# Create libraries
################################################################
file(MAKE_DIRECTORY ${AVANGO_INSTALL_DIRECTORY})
file(MAKE_DIRECTORY ${AVANGO_INSTALL_DIRECTORY}/python3.5)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(avango-unittest)
add_subdirectory(avango-core)
add_subdirectory(avango-python)
add_subdirectory(avango-gua)
add_subdirectory(avango-daemon)
add_subdirectory(avango-tools)
add_subdirectory(avango-sound)
add_subdirectory(avango-utils)

if (AVANGO_OCULUS_RIFT_SUPPORT)
    add_subdirectory(avango-oculus)
endif(AVANGO_OCULUS_RIFT_SUPPORT)

if (AVANGO_VIVE_SUPPORT)
    add_subdirectory(avango-vive)
endif(AVANGO_VIVE_SUPPORT)

if (AVANGO_SKELANIM_SUPPORT)
    add_subdirectory(avango-skelanim)
endif(AVANGO_SKELANIM_SUPPORT)

if (AVANGO_LOD_SUPPORT)
  add_subdirectory(avango-lod)
endif(AVANGO_LOD_SUPPORT)

if (AVANGO_OPENAL_SUPPORT)
    add_subdirectory(avango-openal)
endif(AVANGO_OPENAL_SUPPORT)

if (AVANGO_AWESOMIUM_SUPPORT)
    add_subdirectory(avango-awesomium)
endif(AVANGO_AWESOMIUM_SUPPORT)

if (AVANGO_TV_3_SUPPORT)
  add_subdirectory(avango-tv_3)
endif(AVANGO_TV_3_SUPPORT)

if (AVANGO_VOLUME_SUPPORT)
    add_subdirectory(avango-volume)
endif(AVANGO_VOLUME_SUPPORT)

if (AVANGO_NURBS_SUPPORT)
  add_subdirectory(avango-nurbs)
endif (AVANGO_NURBS_SUPPORT)

if (AVANGO_NRP_SUPPORT)
  add_subdirectory(avango-nrp)
endif (AVANGO_NRP_SUPPORT)

################################################################
# Summary
################################################################

message(STATUS "" )
message(STATUS "Summary:" )
message(STATUS " build type: ${CMAKE_BUILD_TYPE}" )
message(STATUS "" )
message(STATUS " boost:" )
message(STATUS "   library: ${Boost_LIBRARIES}" )
message(STATUS "   library path: ${Boost_LIBRARY_DIRS}" )
message(STATUS "   include: ${Boost_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " guacamole:" )
message(STATUS "   library: ${GUACAMOLE_LIBRARIES}" )
message(STATUS "   library path: ${GUACAMOLE_LIBRARY_DIRS}" )
message(STATUS "   include: ${GUACAMOLE_INCLUDE_DIRS}" )
message(STATUS "" )

if ( AVANGO_OCULUS_RIFT_SUPPORT  )
  message(STATUS " guacamole-oculus:" )
  message(STATUS "   library: ${GUACAMOLE_OCULUS_LIBRARIES}" )
  message(STATUS "   library path: ${GUACAMOLE_OCULUS_LIBRARY_DIRS}" )
  message(STATUS "   include: ${GUACAMOLE_OCULUS_INCLUDE_DIRS}" )
  message(STATUS "" )
endif ( AVANGO_OCULUS_RIFT_SUPPORT )

if ( AVANGO_VIVE_SUPPORT  )
  message(STATUS " guacamole-vive:" )
  message(STATUS "   library: ${GUACAMOLE_VIVE_LIBRARIES}" )
  message(STATUS "   library path: ${GUACAMOLE_VIVE_LIBRARY_DIRS}" )
  message(STATUS "   include: ${GUACAMOLE_VIVE_INCLUDE_DIRS}" )
  message(STATUS "" )
endif ( AVANGO_VIVE_SUPPORT )

if ( AVANGO_VOLUME_SUPPORT  )
  message(STATUS " guacamole-volume:" )
  message(STATUS "   library: ${GUACAMOLE_VOLUME_LIBRARIES}" )
  message(STATUS "   library path: ${GUACAMOLE_VOLUME_LIBRARY_DIRS}" )
  message(STATUS "   include: ${GUACAMOLE_VOLUME_INCLUDE_DIRS}" )
  message(STATUS "" )
endif ( AVANGO_VOLUME_SUPPORT )

if ( AVANGO_AWESOMIUM_SUPPORT  )
  message(STATUS " guacamole-gui:" )
  message(STATUS "   library: ${GUACAMOLE_GUI_LIBRARIES}" )
  message(STATUS "   library path: ${GUACAMOLE_GUI_LIBRARY_DIRS}" )
  message(STATUS "   include: ${GUACAMOLE_GUI_INCLUDE_DIRS}" )
  message(STATUS "" )
endif ( AVANGO_AWESOMIUM_SUPPORT )

if ( AVANGO_SKELANIM_SUPPORT  )
  message(STATUS " guacamole-skelanim:" )
  message(STATUS "   library: ${GUACAMOLE_SKELANIM_LIBRARIES}" )
  message(STATUS "   library path: ${GUACAMOLE_SKELANIM_LIBRARY_DIRS}" )
  message(STATUS "   include: ${GUACAMOLE_SKELANIM_INCLUDE_DIRS}" )
  message(STATUS "" )
endif ( AVANGO_SKELANIM_SUPPORT )

message(STATUS " schism:" )
message(STATUS "   library: ${SCHISM_LIBRARIES}" )
message(STATUS "   library path: ${SCHISM_LIBRARY_DIRS}" )
message(STATUS "   include: ${SCHISM_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " python:" )
message(STATUS "   library: ${PYTHON_LIBRARIES}" )
message(STATUS "   library path: ${PYTHON_LIBRARY_DIRS}" )
message(STATUS "   include: ${PYTHON_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " gl:" )
message(STATUS "   library: ${GL_LIBRARIES}" )
message(STATUS "   include: ${GL_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " bullet:" )
message(STATUS "   library: ${BULLET_LIBRARIES}" )
message(STATUS "   library path: ${BULLET_LIBRARY_DIRS}" )
message(STATUS "   include: ${BULLET_INCLUDE_DIRS}" )
message(STATUS "" )
message(STATUS " json:" )
message(STATUS "   library: ${JSON_LIBRARIES}" )
message(STATUS "   library path: ${JSON_LIBRARY_DIRS}" )
message(STATUS "   include: ${JSON_INCLUDE_DIRS}" )
message(STATUS "" )
