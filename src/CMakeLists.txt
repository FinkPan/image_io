#添加include目录
include_directories(${PROJECT_SOURCE_DIR}/include/image_io)

#按目录搜索源文件,将文件添加到项目中.
set(LIB_SRC "${PROJECT_SOURCE_DIR}/src/image_io.cpp" 
			"${PROJECT_SOURCE_DIR}/include/image_io/image_io.hpp")
			
#指定编译为库文件
add_library(image_io ${LIB_SRC})