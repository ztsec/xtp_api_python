cmake_minimum_required(VERSION 2.8)
project(vn_xtp_api)  

# 设置使用的编译器
set(CMAKE_BUILD_TYPE "Release")
if (CMAKE_COMPILER_IS_GNUC OR CMAKE_COMPILER_IS_GNUCXX) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11")
endif ()

# 设置输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# 使用64位编译
option(USE_64BITS "comiple 64bits" ON)
if (USE_64BITS)
  add_definitions(-DUSE_64BITS)
endif()

# 设置C++ API源文件的所在目录
set(XTPAPI_PATH )
if (WIN32)
    set(XTPAPI_PATH xtpapi)
elseif (UNIX)
    message("Under unix: " ${CMAKE_SIZEOF_VOID_P})
    if (CMAKE_SIZEOF_VOID_P MATCHES "8")
        set(XTPAPI_PATH xtpapi)
    endif()
endif()

include_directories(${XTPAPI_PATH})


set(XTPAPI_MD_LIBRARY )
set(XTPAPI_TD_LIBRARY )

# 链接C++ API库文件
if (WIN32)
find_library(XTPAPI_MD_LIBRARY
	NAMES xtpxquoteapi.lib
	PATHS ${CMAKE_CURRENT_SOURCE_DIR}/xtpapi/) 
find_library(XTPAPI_TD_LIBRARY
	NAMES xtptraderapi.lib
	PATHS ${CMAKE_CURRENT_SOURCE_DIR}/xtpapi/)
elseif (UNIX)
find_library(XTPAPI_MD_LIBRARY
  NAMES libxtpxquoteapi.so
  PATHS ${XTPAPI_PATH}) 
find_library(XTPAPI_TD_LIBRARY
  NAMES libxtptraderapi.so
  PATHS ${XTPAPI_PATH})
endif()

# 设置Python所在的目录
set(PYTHON_LIBRARY )
if (WIN32)
	set(PYTHON_INCLUDE_PATH C:/Programs/Python/Python39/include)
	link_directories(C:/Programs/Python/Python39/libs)
	include_directories(${PYTHON_INCLUDE_PATH})
elseif(UNIX)
	set(PYTHON_LIBRARY /usr/local/python3/lib/)
	set(PYTHON_INCLUDE_PATH /usr/local/python3/include/python3.9/)
	
	include_directories(${PYTHON_INCLUDE_PATH})
endif()

# 链接boost库，anaconda /python3用的是python3.9没有用Anaconda
set(BOOST_ROOT ) 
if (WIN32)
	set(BOOST_ROOT   D:/MyProject/boost_1_80_0/)
    set(Boost_USE_STATIC_LIBS        ON)    #链接boost静态库
	link_directories(D:/MyProject/boost_1_80_0/stage/lib)
	include_directories(${BOOST_ROOT})
elseif(UNIX)
	set(Boost_USE_MULTITHREADED      ON)
	set(BOOST_ROOT   /usr/local/boost_1_80_0/)
	find_package(Boost 1.80.0 COMPONENTS python39 thread date_time system chrono REQUIRED) # 如果boost库没有完全编译，需要将编译的库明确地指出，否者message(${Boost_LIBRARIES})会出错
	if(Boost_FOUND)
		include_directories(${Boost_INCLUDE_DIRS})
	endif()
endif()

# 设置编译源文件
set (vnxtpxquote )
set (vnxtptrader )

option(BUILD_XTP_MD "build xtp md" ON)
if (BUILD_XTP_MD)
   add_definitions(-BUILD_XTP_MD)
  set(XTP_MD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vnxtpxquote)
   include_directories(${XTP_MD_PATH})
   set(VN_XTP_MD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/vnxtpxquote/vnxtpxquote.cpp)
   add_library(vnxtpxquote SHARED ${VN_XTP_MD_SOURCE})
endif()
option(BUILD_XTP_TD "build xtp td" ON)
if (BUILD_XTP_TD)
   add_definitions(-BUILD_XTP_TD)
   set(XTP_TD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vnxtptrader/)
   include_directories(${XTP_TD_PATH})
   set(VN_XTP_TD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/vnxtptrader/vnxtptrader.cpp)
   add_library(vnxtptrader SHARED ${VN_XTP_TD_SOURCE})  
endif()

# 去掉生成的so文件名中前缀的lib
set_target_properties(vnxtpxquote PROPERTIES PREFIX "")

# 链接生成.so文件
if (WIN32)
	target_link_libraries(vnxtpxquote  ${XTPAPI_MD_LIBRARY})
elseif(UNIX)
	target_link_libraries(vnxtpxquote  ${Boost_LIBRARIES} ${PYTHON_LIBRARY} ${XTPAPI_MD_LIBRARY})
endif()
# 将生成的dll文件后缀名修改为pyd（仅在windows下）
set(MD_DLL ${LIBRARY_OUTPUT_PATH}/Release/vnxtpxquote.dll)
if (EXISTS ${MD_DLL})
  file(RENAME ${MD_DLL} ${LIBRARY_OUTPUT_PATH}/Release/vnxtpxquote.pyd)
endif()

# 和上面的MD相同
set_target_properties(vnxtptrader PROPERTIES PREFIX "")
if (WIN32)
	target_link_libraries(vnxtptrader ${XTPAPI_TD_LIBRARY})
elseif(UNIX)
	target_link_libraries(vnxtptrader  ${Boost_LIBRARIES} ${PYTHON_LIBRARY} ${XTPAPI_TD_LIBRARY})
endif()

set(TD_DLL ${LIBRARY_OUTPUT_PATH}/Release/vnxtptrader.dll)
if (EXISTS ${TD_DLL})
  file(RENAME  ${TD_DLL} ${LIBRARY_OUTPUT_PATH}/Release/vnxtptrader.pyd)
endif()
