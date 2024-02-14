# Set the output directory for the executables to the 'bin' folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../bin)
set(ProtobufIncludePath ${CMAKE_CURRENT_BINARY_DIR}
    CACHE INTERNAL "Path to generated protobuf files.")
INCLUDE_DIRECTORIES(${ProtobufIncludePath})

# include_directories(${CMAKE_BINARY_DIR}/proto)

# Add all source files for the client
file(GLOB_RECURSE CLIENT_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/client/*.cpp)

# Add all source files for the server
file(GLOB_RECURSE SERVER_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/server/*.cpp)

# Add all source files for common functionality
file(GLOB_RECURSE COMMON_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp)
message(">>>> progo hds" ${PROTO_SRC} ${PROTO_HEADER})

# Create separate executables for client and server
ADD_EXECUTABLE(client ${CLIENT_SRC_FILES} ${COMMON_SRC_FILES} ${PROTO_SRC} ${PROTO_HEADER})
ADD_EXECUTABLE(server ${SERVER_SRC_FILES} ${COMMON_SRC_FILES})

# Link with the proto library and Protobuf for both client and server
TARGET_LINK_LIBRARIES(client proto ${Protobuf_LIBRARIES})
TARGET_LINK_LIBRARIES(server proto ${Protobuf_LIBRARIES})

# message("Proto lib path: " ${Protobuf_LIBRARIES})