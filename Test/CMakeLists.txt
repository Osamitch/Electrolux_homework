project(Usart_api_test)
add_executable(test test.c)
target_link_libraries(test PUBLIC ellux_usart_api)
