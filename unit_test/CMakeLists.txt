#包含include搜索			  
include_directories(${PROJECT_SOURCE_DIR}/include/image_io/)
include_directories(${PROJECT_SOURCE_DIR}/src/)
set(gtest_unit gtest_unit.cpp)
#生成可执行文件			  
add_library(gtest_unit ${gtest_unit})
#设置依赖的动态库
#target_link_libraries(gtest_unit image_io)



include_directories(E:/LIB/gtest-1.6.0/gtestlib/include/)
set(gtest_main gtest_main.cpp)
#生成可执行文件			  
add_executable(gtest_main ${gtest_main})
#设置依赖的动态库
target_link_libraries(gtest_main image_io "E:/LIB/gtest-1.6.0/gtestlib/lib/gtestd.lib")