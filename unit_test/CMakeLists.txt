#添加include目录
include_directories(${PROJECT_SOURCE_DIR}/include/image_io)

#按目录搜索源文件,将文件添加到项目中.
set(APP_SRC main.cpp)
#指定编译为可执行文件
add_executable(unit_test ${APP_SRC})

#设置依赖的动态库
target_link_libraries(unit_test image_io)