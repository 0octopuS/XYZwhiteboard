# cmake_minimum_required(VERSION 3.5)
# project(YourProjectName)

# Set the output directory for the executables to the 'bin' folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../bin)
set(ProtobufIncludePath ${CMAKE_CURRENT_BINARY_DIR}
    CACHE INTERNAL "Path to generated protobuf files.")
INCLUDE_DIRECTORIES(${ProtobufIncludePath})

# Include protobuf generated files
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/../proto)

# Find required packages
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

# Add all source files for the common functionality
file(GLOB_RECURSE COMMON_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp)

file(GLOB_RECURSE BASE_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/base/*.cpp)
add_library(base_lib ${BASE_SRC_FILES})
target_link_libraries(base_lib proto ${Protobuf_LIBRARIES})

# Create a library for common functionality
# add_library(common ${COMMON_SRC_FILES})

# Create the client library
file(GLOB_RECURSE CLIENT_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/client/*.cpp)
add_library(client_lib ${CLIENT_SRC_FILES})
target_link_libraries(client_lib proto ${Protobuf_LIBRARIES} base_lib)

# Create the server library
file(GLOB_RECURSE SERVER_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/server/*.cpp)
add_library(server_lib ${SERVER_SRC_FILES})

# Link common library with server library
target_link_libraries(server_lib proto ${Protobuf_LIBRARIES} base_lib)

# Create executable for test server
add_executable(test_element ${CMAKE_CURRENT_SOURCE_DIR}/example/test_element.cpp)
target_link_libraries(test_element proto ${Protobuf_LIBRARIES} base_lib ${Boost_LIBRARIES} OpenSSL::Crypto)

# Create executable for test client
add_executable(test_client ${CMAKE_CURRENT_SOURCE_DIR}/example/test_client.cpp)
target_link_libraries(test_client proto ${Protobuf_LIBRARIES} client_lib ${Boost_LIBRARIES} OpenSSL::Crypto)

# Create executable for test server
add_executable(test_server ${CMAKE_CURRENT_SOURCE_DIR}/example/test_server.cpp)
target_link_libraries(test_server proto ${Protobuf_LIBRARIES} server_lib ${Boost_LIBRARIES} OpenSSL::Crypto)
