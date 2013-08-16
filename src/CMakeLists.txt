
#包含include搜索			  
include_directories(${PROJECT_SOURCE_DIR}/include/image_io/)
include_directories(${PROJECT_SOURCE_DIR}/src/)
#源文件
file(GLOB lib_srcs *.cpp *.hpp)
#头文件
file(GLOB lib_hdrs ${PROJECT_SOURCE_DIR}/include/image_io/*.*)

#Define a grouping for sources in the makefile.
source_group("INCLUDE" FILES ${lib_hdrs} )
source_group("SRC" FILES ${lib_srcs})


#生成静态库
add_library(image_io STATIC ${lib_hdrs} ${lib_srcs})

