cmake_minimum_required(VERSION 3.10.2)
project("libanvil" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# Set library type
if (LIBANVIL_BUILD_SHARED)
    set(LIBANVIL_LIBRARY_TYPE "SHARED")
else()
    set(LIBANVIL_LIBRARY_TYPE "STATIC")
endif()

set(ZLIB_COMPAT ON CACHE BOOL " " FORCE)
set(ZLIB_ENABLE_TESTS OFF CACHE BOOL " " FORCE)
unset(BUILD_SHARED_LIBS CACHE)
message(STATUS "shared:" ${BUILD_SHARED_LIBS})
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/zlib-ng")

# Compile main libanvil library
add_library(libanvil
        include/Block.h src/Block.cpp
        include/ChunkRegistry.h src/ChunkRegistry.cpp
        include/byte_stream.h src/byte_stream.cpp
        include/chunk_info.h src/chunk_info.cpp
        include/chunk_tag.h src/chunk_tag.cpp
        include/compression.h src/compression.cpp
        include/region.h src/region.cpp
        include/region_file.h src/region_file.cpp
        include/region_file_reader.h src/region_file_reader.cpp
        include/region_file_writer.h src/region_file_writer.cpp
        include/region_header.h src/region_header.cpp

        include/tag/byte_array_tag.h src/tag/byte_array_tag.cpp
        include/tag/byte_tag.h src/tag/byte_tag.cpp
        include/tag/compound_tag.h src/tag/compound_tag.cpp
        include/tag/double_tag.h src/tag/double_tag.cpp
        include/tag/end_tag.h src/tag/end_tag.cpp
        include/tag/float_tag.h src/tag/float_tag.cpp
        include/tag/generic_tag.h src/tag/generic_tag.cpp
        include/tag/int_array_tag.h src/tag/int_array_tag.cpp
        include/tag/int_tag.h src/tag/int_tag.cpp
        include/tag/list_tag.h src/tag/list_tag.cpp
        include/tag/long_array_tag.h src/tag/long_array_tag.cpp
        include/tag/long_tag.h src/tag/long_tag.cpp
        include/tag/short_tag.h src/tag/short_tag.cpp
        include/tag/string_tag.h src/tag/string_tag.cpp

        $<TARGET_OBJECTS:zlibstatic>
        )

target_include_directories(libanvil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libanvil PUBLIC zlibstatic)

add_executable(LibandvilTest src/LibanvilTest.cpp)
target_link_libraries(LibandvilTest zlibstatic)