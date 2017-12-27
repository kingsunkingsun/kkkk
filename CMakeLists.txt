cmake_minimum_required(VERSION 3.2)
PROJECT(ResEngine)

SET(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
SET(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")

include_directories("${SRC_DIR}/include")
include_directories("${LIB_DIR}")

#Resecs
add_subdirectory("${LIB_DIR}/Resecs")
include_directories(${LIB_DIR}/Resecs)

#GLFW
SET(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
include_directories("${GLFW_DIR}/include")
#target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

#GLAD
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
include_directories("${GLAD_DIR}/include")

#glm
set(GLM_DIR "${LIB_DIR}/glm")
add_subdirectory("${GLM_DIR}")

#freetype
FIND_PACKAGE(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})


SET(LIBRARIES 
"glfw"
"glad"
${FREETYPE_LIBRARIES}
)


#build engine
file(GLOB RENDER_HEADERS "${SRC_DIR}/include/RenderAPI/*.h")
file(GLOB RENDER_SOURCES "${SRC_DIR}/src/RenderAPI/*.cpp")
file(GLOB ENGINE_HEADERS "${SRC_DIR}/include/ResEngine/*.h")
file(GLOB ENGINE_SOURCES "${SRC_DIR}/src/ResEngine/*.cpp")
add_library(${PROJECT_NAME} 
    ${RENDER_HEADERS} ${RENDER_SOURCES}
    ${ENGINE_HEADERS} ${ENGINE_SOURCES}
)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

source_group("RenderAPI" FILES ${RENDER_HEADERS} ${RENDER_SOURCES})
source_group("ResEngine" FILES ${ENGINE_HEADERS} ${ENGINE_SOURCES})

#sample
add_subdirectory(sample)

#tests
SET(GOOGLETEST_DIR "${LIB_DIR}/googletest")
add_subdirectory(${GOOGLETEST_DIR})
add_subdirectory("tests")

#copy shader and font into build