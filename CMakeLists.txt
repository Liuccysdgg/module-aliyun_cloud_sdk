cmake_minimum_required(VERSION 3.5)

set(MODULE_NAME "aliyunsdk")
# 设置项目名为当前目录名
project(${MODULE_NAME})

# 搜索源文件和头文件
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES 
    "${PROJECT_SOURCE_DIR}/src/*.h"
)

# 将源文件分配到 Source Files 文件夹
foreach(source IN LISTS SOURCE_FILES)
    get_filename_component(source_path "${source}" PATH)
    file(RELATIVE_PATH source_path_rel "${PROJECT_SOURCE_DIR}" "${source_path}")
    string(REPLACE "/" "\\" source_path_rel_win "${source_path_rel}")
    source_group("Source Files\\${source_path_rel_win}" FILES "${source}")
endforeach()

# 将头文件分配到 Header Files 文件夹
foreach(header IN LISTS HEADER_FILES)
    get_filename_component(header_path "${header}" PATH)
    file(RELATIVE_PATH header_path_rel "${PROJECT_SOURCE_DIR}" "${header_path}")
    string(REPLACE "/" "\\" header_path_rel_win "${header_path_rel}")
    source_group("Header Files\\${header_path_rel_win}" FILES "${header}")
endforeach()

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# 安装复制
set(CMAKE_INSTALL_ALWAYS_COPY TRUE)

set(YLIB ${CMAKE_INSTALL_PREFIX}/../ylib)
set(FASTWEB ${CMAKE_INSTALL_PREFIX}/../fastweb)

# 包含路径
if(MSVC)

else()
	include_directories(
		/usr/local/include/ylib
		/usr/local/include/fastweb
		/opt/lua54/include
		/usr/local/include)
	add_definitions(-DfPIC)
endif()

# 添加共享库
add_library(${MODULE_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

if(MSVC)

else()
	target_link_libraries(${MODULE_NAME} 
			hpsocket
			ylib
			crypto
			/opt/lua54/lib/liblua.a
			pthread
			/usr/lib/x86_64-linux-gnu/libalibabacloud-sdk-core.so
	)

endif()


install(TARGETS ${MODULE_NAME} DESTINATION $<IF:$<CONFIG:Debug>,${FASTWEB}/bin/debug/module/${MODULE_NAME},${FASTWEB}/bin/release/module/${MODULE_NAME}>)