cmake_minimum_required(VERSION 3.18)

find_package(Perl)
if(NOT PERL_FOUND)
    message(FATAL_ERROR "Perl is needed for generating the fsdata.c file")
endif()

set(PICO_BOARD pico_w)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)
# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(flora C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
        -Wall
        -Werror
        )


# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

set(PROGRAM_NAME flora)

set(MAKE_FS_DATA_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/external/makefsdata)

if (NOT EXISTS ${MAKE_FS_DATA_SCRIPT})
        file(DOWNLOAD
                https://raw.githubusercontent.com/lwip-tcpip/lwip/e799c266facc3c70190676eccad49d6c2db2caac/src/apps/http/makefsdata/makefsdata
                ${MAKE_FS_DATA_SCRIPT}
                )
endif()
message("Running makefsdata script")
execute_process(COMMAND
        perl ${MAKE_FS_DATA_SCRIPT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        ECHO_OUTPUT_VARIABLE
        ECHO_ERROR_VARIABLE
        )
file(RENAME fsdata.c source/my_fsdata.c)

add_executable(${PROGRAM_NAME}
        source/main.c
        source/ssi.c
        source/cgi.c
        source/waterctl.c
        )

target_link_libraries(${PROGRAM_NAME}
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_http
        pico_stdlib
        hardware_adc
        )

target_include_directories(${PROGRAM_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        )

configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/config.h.in
        ${CMAKE_SOURCE_DIR}/source/config.h
)

pico_enable_stdio_usb(${PROGRAM_NAME} TRUE)
pico_enable_stdio_uart(${PROGRAM_NAME} TRUE)

pico_add_extra_outputs(${PROGRAM_NAME})
