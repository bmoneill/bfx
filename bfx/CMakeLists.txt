cmake_minimum_required(VERSION 3.31.6)

set(BFX_BINARY_NAME bfx-bin)

include_directories(../libbfx/src)
add_executable(${BFX_BINARY_NAME} src/main.c)
target_link_libraries(${BFX_BINARY_NAME} PRIVATE bfx)

set_target_properties(${BFX_BINARY_NAME} PROPERTIES OUTPUT_NAME bfx)
install(TARGETS ${BFX_BINARY_NAME} RUNTIME DESTINATION bin)
