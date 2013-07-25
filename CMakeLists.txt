cmake_minimum_required(VERSION 2.8)

#项目名
project(image_io)

#子工程 将生成的库文件放到bin文件夹里
add_subdirectory(src bin)
#子工程
add_subdirectory(unit_test)