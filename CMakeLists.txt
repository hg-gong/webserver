cmake_minimum_required(VERSION 3.10)
project(webserver)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
# Complie Flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")

# Specify the include directory
set(SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
include_directories(${SRC_INCLUDE_DIR} ${EST_INCLUDE_DIR})

# Specify the source directory
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_CPP_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/test TEST_CPP_FILES)

# Generate shared library
add_library(libwebserver SHARED ${SRC_CPP_FILES})

# Output library
install(TARGETS libwebserver LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib)
# SET_TARGET_PROPERTIES(libwebserver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add subdirectory
add_subdirectory(test)

######################################################################################################################
# MAKE TARGETS
######################################################################################################################

##########################################
# "make format"
##########################################

# Add a custom target to run clang-format on the source files
add_custom_target(
  format
  # COMMAND clang-format -i -style=file ${PROJECT_SOURCE_DIR}/*.cpp
  COMMAND clang-format -i -style=file ${PROJECT_SOURCE_DIR}/src/*.cpp
  COMMAND clang-format -i -style=file ${PROJECT_SOURCE_DIR}/src/include/*.h
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
