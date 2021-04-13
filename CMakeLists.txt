project(MyComplier)
set(cmake_build_type DEBUG)
add_compile_options(-std=gnu++17)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/library)
aux_source_directory(${CMAKE_SOURCE_DIR}/src DIR_SRC)
add_executable(MyComplier ${DIR_SRC})