PROJECT(remoteq)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8) 

set(fossilizid_INCLUDE_PATH ${PROJECT_SOURCE_DIR}//..//Fossilizid CACHE PATH "Fossilizid include path")
INCLUDE_DIRECTORIES(${fossilizid_INCLUDE_PATH})

set(BOOST_INCLUDE_PATH ${PROJECT_SOURCE_DIR}//..//boost CACHE PATH "BOOST include path")
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_PATH})

set(BOOST_LIBS_PATH ${PROJECT_SOURCE_DIR}//..//boost//libs CACHE PATH "BOOST lib path")
link_directories(${BOOST_LIBS_PATH})


AUX_SOURCE_DIRECTORY(. DIR_SRC)
file(GLOB header_files ./ ./*.h)

SET(SRC_LIST 
    ${DIR_SRC}
)

if(WIN32)
AUX_SOURCE_DIRECTORY(./windows/ windows_DIR_SRC)
file(GLOB windows_head_file ./windows/ ./windows/*.h)

AUX_SOURCE_DIRECTORY(./ipv4/windows/ windows_ip4_DIR_SRC)
file(GLOB windows_ip4_head_file ./ipv4/windows/ ./ipv4/windows/*.h)

AUX_SOURCE_DIRECTORY(./ipv4/tcp/windows/ tcp_windows_DIR_SRC)
file(GLOB tcp_windows_head_file ./ipv4/tcp/windows/ ./ipv4/tcp/windows/*.h)


AUX_SOURCE_DIRECTORY(./ipv4/udp/windows/ udp_windows_DIR_SRC)
file(GLOB udp_windows_head_file ./ipv4/udp/windows/ ./ipv4/udp/windows/*.h)

set(header_tcp_files
    ${header_files}
    ${windows_head_file}
    ${windows_ip4_head_file}
    ${tcp_windows_head_file}
)

SET(SRC_tcp_LIST
    ${DIR_SRC}
    ${windows_DIR_SRC}
    ${windows_ip4_DIR_SRC}
    ${tcp_windows_DIR_SRC}
)

set(header_udp_files
    ${header_files}
    ${windows_head_file}
    ${windows_ip4_head_file}
    ${udp_windows_head_file}
)

SET(SRC_udp_LIST
    ${DIR_SRC}
    ${windows_DIR_SRC}
    ${windows_ip4_DIR_SRC}
    ${udp_windows_DIR_SRC}
)
endif(WIN32)

ADD_LIBRARY(remoteq_tcp ${SRC_tcp_LIST} ${header_tcp_files})
ADD_LIBRARY(remoteq_udp ${SRC_udp_LIST} ${header_udp_files})

add_subdirectory(test)