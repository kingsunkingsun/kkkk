PROJECT(ResEngine_Sample)

file(GLOB_RECURSE SOURCES "*.h" "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} "ResEngine")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
file(COPY "fonts" DESTINATION "/")
file(COPY "shaders" DESTINATION "/")