include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}
  #${PROJECT_SOURCE_DIR}/pompeii
  ${CMAKE_CURRENT_BINARY_DIR}
)

source_group(\\ FILES CMakeLists.txt)

set( POMPEIIENGINE_PUBLIC_HEADERS
  IComponent.h
  GameObject.h
  Light.h
  Mesh.h
  IMaterial.h
  SimpleMaterial.h
)
set( POMPEIIENGINE_HEADERS
  includes.hpp
  GameObject.inl
)
set( POMPEIIENGINE_SOURCES
  GameObject.cpp
  SimpleMaterial.cpp
  Mesh.cpp
)

set( POMPEIIENGINE_LINK_LIBRARIES
#  pompeii
  ${GLM_LIBRARIES}
)

set( POMPEIIENGINE_INCLUDE_NAME pompeiiEngine )
set( POMPEIIENGINE_NAMESPACE pompeiiEngine )

common_library( pompeiiEngine )

target_include_directories( pompeiiEngine
  PUBLIC    "${CMAKE_SOURCE_DIR}"
)



configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/routes.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/routes.h)

set( ENGINEDEMO_SOURCES
  demo.cpp
)
set( ENGINEDEMO_LINK_LIBRARIES
#  pompeii
  pompeiiEngine
)
common_application( EngineDemo )