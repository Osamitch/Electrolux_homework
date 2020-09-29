Project(Usart_api_test)
set(CMAKE_C_STANDARD C99)

add_executable(Test test.c)

target_link_libraries(ELLUX_USART_API)
