# Add the executable target
add_executable(testThreadPool  testThreadPool.cpp )
add_executable(server  server.cpp )
add_executable(client  client.cpp )

# Link the library with executable target
target_link_libraries(testThreadPool libwebserver)
target_link_libraries(server libwebserver)
target_link_libraries(client libwebserver)


