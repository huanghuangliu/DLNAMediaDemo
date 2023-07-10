cmake_minimum_required(VERSION 3.10.2)
project("mediarender")
include_directories("./depends/include")
include_directories("./depends/include/Core")
include_directories("./depends/include/Devices/MediaConnect")
include_directories("./depends/include/Devices/MediaRenderer")
include_directories("./depends/include/Devices/MediaServer")
include_directories("./depends/include/Extras")
include_directories("./depends/include/Extras/Managed")
include_directories("./depends/include/Neptune")
#链接库
link_directories("${CMAKE_SOURCE_DIR}/depends/libs/${CMAKE_ANDROID_ARCH_ABI}")
#添加 lib 名称
add_library(git-platinum
        SHARED
        native-lib.cpp)
#日志库
find_library(log-lib log)
set(CMAKE_C_FLAGS "DNPT_CONFIG_ENABLE_LOGGING")
target_link_libraries(git-platinum
        ${log-lib}
        Platinum
        Neptune
        axTLS
        PltMediaConnect
        PltMediaRenderer
        PltMediaServer
        android
        stdc++
        Zlib)