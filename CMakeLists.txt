cmake_minimum_required(VERSION 2.8)

#项目名
project(image_io)

#包含include搜索			  
include_directories(${PROJECT_SOURCE_DIR}/include/image_io/)
include_directories(${PROJECT_SOURCE_DIR}/src/)
#源文件
file(GLOB lib_srcs src/*.*)
#头文件
file(GLOB lib_hdrs include/image_io/*.*)
#单元测试
file(GLOB unit_test unit_test/*.*)

#Define a grouping for sources in the makefile.
source_group("INCLUDE" FILES ${lib_hdrs} )
source_group("SRC" FILES ${lib_srcs})


#生成静态库
add_library(image_io STATIC ${lib_hdrs} ${lib_srcs})
#生成可执行文件			  
add_executable(unit_test ${unit_test})
#设置依赖的动态库
target_link_libraries(unit_test image_io)
