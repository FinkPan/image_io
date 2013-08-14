#包含include搜索			  
include_directories(${PROJECT_SOURCE_DIR}/include/image_io/)
include_directories(${PROJECT_SOURCE_DIR}/src/)

set(apps main.cpp)
#生成可执行文件			  
add_executable(apps ${apps})
#设置依赖的动态库
target_link_libraries(apps image_io)