cmake_minimum_required(VERSION 3.9.0)
project(libquery VERSION 0.1.0  LANGUAGES C)
link_libraries("-static")
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(STATUS "GCC")
    add_compile_options(-ffunction-sections -fdata-sections)
endif()

add_subdirectory(vendor/plibsys)

include_directories(include)

add_library(libquery STATIC src/main.c src/error.c)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(libquery PRIVATE -Wl,--gc-sections)
endif()

include(CheckIPOSupported)

check_ipo_supported(RESULT result OUTPUT output LANGUAGES C)
if(result)
message(STATUS "IPO is supported")
set_property(TARGET libquery PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
message(WARNING "IPO is not supported: ${output}")
endif()

target_link_libraries(libquery plibsysstatic)


message(STATUS "Build example")
add_executable(mqd main.c)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(mqd PRIVATE -Wl,--gc-sections -s)
endif()
if(result)
    set_property(TARGET mqd PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
target_link_libraries(mqd libquery)
# install(TARGETS msq_example)
