# # Add the executable target
add_executable(testThreadPool  testThreadPool.cpp )
add_executable(server  server.cpp )
add_executable(client  client.cpp )
add_executable(multipleClients multipleClients.cpp )

# # Link the library with executable target
target_link_libraries(testThreadPool libwebserver)
target_link_libraries(server libwebserver)
target_link_libraries(client libwebserver)
target_link_libraries(multipleClients libwebserver)

