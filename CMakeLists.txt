cmake_minimum_required(VERSION 3.2)
PROJECT(resecs_sample)

SET(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
SET(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")
SET(SOURCES "${SRC_DIR}/main.cpp")

file(GLOB_RECURSE HEADERS "source/*.h")
file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source/include")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PRIVATE "${LIB_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}/include")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

#Resecs
add_subdirectory("${LIB_DIR}/Resecs")
target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR}/Resecs)

#GLFW
SET(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

#GLAD
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad")

#glm
set(GLM_DIR "${LIB_DIR}/glm")
add_subdirectory("${GLM_DIR}")

#freetype
FIND_PACKAGE(Freetype REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})

#tests
SET(GOOGLETEST_DIR "${LIB_DIR}/googletest")
add_subdirectory(${GOOGLETEST_DIR})
add_subdirectory("tests")

#copy shader and font into build
file(COPY "source/fonts" DESTINATION "/")
file(COPY "source/shaders" DESTINATION "/")