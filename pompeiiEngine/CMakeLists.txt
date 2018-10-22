include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/pompeii
  
  ${VULKAN_INCLUDE_DIR} # TODO: WHY?
  
  ${CMAKE_CURRENT_BINARY_DIR}
)

source_group(\\ FILES CMakeLists.txt)

set( POMPEIIENGINE_PUBLIC_HEADERS
  Pipeline.h
  ShaderProgram.h

  ISystem.h
  SystemRegister.h
  SystemUpdater.h

  Components/Component.h
  GameObject.h
  Light.h
  Meshes/Model.h
  Meshes/Mesh.h
  Materials/Material.h
  Materials/MaterialPipeline.h
  Materials/SimpleMaterial.h

  Maths/Frustum.h
  Maths/Mathf.h
  Maths/Easing.h

  Exceptions/Exception.h
  Exceptions/FileNotFoundException.h
  Exceptions/HasParentException.h
  Exceptions/RuntimeException.h
)
set( POMPEIIENGINE_HEADERS
  includes.hpp
  GameObject.inl
)
set( POMPEIIENGINE_SOURCES
  Pipeline.cpp
  
  GameObject.cpp
  Light.cpp
  Materials/SimpleMaterial.cpp
  Meshes/Mesh.cpp

  Maths/Frustum.cpp
  Maths/Mathf.cpp
  Maths/Easing.cpp
)

set( POMPEIIENGINE_LINK_LIBRARIES
  pompeii
  ${GLM_LIBRARIES}


  
  
  glslang
  OGLCompiler
  OSDependent
  SPIRV
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
  pompeii
  pompeiiEngine
)
common_application( EngineDemo )