cmake_minimum_required(VERSION 3.6)
project(shake)

include_directories("${PROJECT_SOURCE_DIR}/vendor")

set(VENDOR_FILES
        vendor/bstrlib.h
        vendor/bstrlib.c)

add_library(vendor STATIC ${VENDOR_FILES})

set(CMAKE_C_FLAGS "-D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_GNU_SOURCE -fgnu89-inline -std=c11")

set(SOURCE_FILES
    src/args.c
    src/args.h
    src/dbg.h
    src/keyvaluevec.c
    src/keyvaluevec.h
    src/main.c
    src/main.h
    src/runner.c
    src/runner.h
    src/script.c
    src/script.h
    src/scripts.c
    src/scripts.h src/shakefile/functions.c src/shakefile/shakefile.h src/shakefile/project.c src/shakefile/shakefile.c)

add_executable(shake ${SOURCE_FILES})
target_link_libraries(shake vendor)