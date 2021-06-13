PROJECT(vtc_tool LANGUAGES C)

AUX_SOURCE_DIRECTORY(${vtc_tool_SOURCE_DIR} src)  # 寻找源文件
ADD_LIBRARY(vtc_tool SHARED ${src})  # 将vtc tool编译为动态库

SET_TARGET_PROPERTIES(vtc_tool PROPERTIES OUTPUT_NAME "vttool")  # 输出为 libvttool.so 或 libvttool.dll
TARGET_LINK_LIBRARIES(vtc_tool vtc_mem dl)

INSTALL(TARGETS vtc_tool)