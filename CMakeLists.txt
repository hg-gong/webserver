cmake_minimum_required(VERSION 3.0)
project(webserver)

# Set the C++ standard to C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")



# Specify the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Specify the source directory
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

# Add the executable target
add_executable(testThreadPool ${SRC} testThreadPool.cpp )

# Add a custom target to run clang-format on the source files
add_custom_target(
  format
  COMMAND clang-format -i -style=file ${PROJECT_SOURCE_DIR}/*.cpp
  COMMAND clang-format -i -style=file ${PROJECT_SOURCE_DIR}/src/*.cpp
  COMMAND clang-format -i -style=file ${PROJECT_SOURCE_DIR}/include/*.h
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
