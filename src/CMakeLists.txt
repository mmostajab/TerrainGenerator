PROJECT(TerrainGenerator-core)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0016 NEW)

MESSAGE(STATUS "Configuring TerrainGenerator Source")



# glob sources from core directories
FILE(GLOB TerrainGeneratorSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  glsl/*.frag
  glsl/*.vert
  cl/*.cl
  *.cpp
)

FILE(GLOB TerrainGeneratorHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.h
)

include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH} ${GLM_INCLUDE_DIR})


ADD_DEFINITIONS(${TerrainGeneratorGlobalDefinitions})

INCLUDE_DIRECTORIES(${TerrainGeneratorIncludeDirs})
LINK_DIRECTORIES(${GLFW_LIBRARY} ${GLEW_LIBRARY})

MESSAGE("GLEW Library = ${GLEW_LIBRARY}")
MESSAGE("GLFW Library = ${GLFW3_LIBRARY}")

add_executable (TerrainGenerator
  ${TerrainGeneratorSources}
  ${TerrainGeneratorHeaders}
  )

IF(WIN32)
  TARGET_LINK_LIBRARIES( TerrainGenerator ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY} )
ELSE(WIN32)
  TARGET_LINK_LIBRARIES( TerrainGenerator ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY}  -lX11 -lm -lXxf86vm -lXrandr -lXi -lGLEW -lstdc++ -lXinerama -lXcursor )
ENDIF(WIN32)

DEFINE_SOURCE_GROUPS_FROM_SUBDIR(TerrainGeneratorSources ${TerrainGeneratorHome} "")
DEFINE_SOURCE_GROUPS_FROM_SUBDIR(TerrainGeneratorHeaders ${TerrainGeneratorHome} "")
