cmake_minimum_required(VERSION 2.8)
project(shake)

include_directories("${PROJECT_SOURCE_DIR}/vendor")

set(VENDOR_FILES
        vendor/bstrlib.h
        vendor/bstrlib.c)

add_library(vendor STATIC ${VENDOR_FILES})

set(CMAKE_C_FLAGS "-D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_GNU_SOURCE -fgnu89-inline -std=c11")
#define _POSIX_C_SOURCE 2

set(SOURCE_FILES
        src/dbg.h
        src/main.c
        src/runner.c
        src/runner.h
        src/shakefile/shakefile.h
        src/shakefile/functions.c
        src/shakefile/project.c
        src/runscripts/runscripts.c src/runscripts/runscripts.h src/colors.h src/config.c src/config.h src/commands.c src/commands.h)

add_executable(shake ${SOURCE_FILES})
target_link_libraries(shake vendor)
