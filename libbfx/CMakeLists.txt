project("libbfx" C)

set(LIBRARY_NAME "bfx")
set(LIBRARY_VERSION_MAJOR 0)
set(LIBRARY_VERSION_STRING 0.0)

# C standard
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pedantic -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700 -D_FILE_OFFSET_BITS=64")

# Optimization
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -ffast-math")

# Warnings
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-missing-field-initializers")

include(GNUInstallDirs)

set(LIBRARY_BASE_PATH "./src")

INCLUDE_DIRECTORIES (
 "${LIBRARY_BASE_PATH}"
)

set(LIBRARY_PUBLIC_SRC
 "${LIBRARY_BASE_PATH}/bfx.c"
 "${LIBRARY_BASE_PATH}/compile.c"
 "${LIBRARY_BASE_PATH}/interpret.c"
)
set(LIBRARY_PUBLIC_SRC ${LIBRARY_PUBLIC_SRC}
 "${LIBRARY_BASE_PATH}/langs/brainfork.c"
 "${LIBRARY_BASE_PATH}/langs/brainfuck.c"
 "${LIBRARY_BASE_PATH}/langs/grin.c"
 "${LIBRARY_BASE_PATH}/langs/pbrain.c"
 "${LIBRARY_BASE_PATH}/langs/util.c"
 "${LIBRARY_BASE_PATH}/langs/weave.c"
)

set(LIBRARY_PUBLIC_HEADERS
 "${LIBRARY_BASE_PATH}/bfx.h"
 "${LIBRARY_BASE_PATH}/compile.h"
 "${LIBRARY_BASE_PATH}/interpret.h"
 "${LIBRARY_BASE_PATH}/langs/brainfork.h"
 "${LIBRARY_BASE_PATH}/langs/brainfuck.h"
 "${LIBRARY_BASE_PATH}/langs/grin.h"
 "${LIBRARY_BASE_PATH}/langs/pbrain.h"
 "${LIBRARY_BASE_PATH}/langs/util.h"
 "${LIBRARY_BASE_PATH}/langs/weave.h"
)

add_library (
 ${LIBRARY_NAME} SHARED ${LIBRARY_PUBLIC_SRC}
)

target_link_libraries(${LIBRARY_NAME} PRIVATE m)

set_target_properties(
 ${LIBRARY_NAME} PROPERTIES
 VERSION		${LIBRARY_VERSION_STRING}
 SOVERSION	    ${LIBRARY_VERSION_MAJOR}
# PUBLIC_HEADER  ${LIBRARY_PUBLIC_HEADERS}
)


# Compiler definitions
ADD_DEFINITIONS("-g")
if(TEST)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTEST")
endif()

# Library installation directives
INSTALL (
 TARGETS ${LIBRARY_NAME}
 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
 PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
