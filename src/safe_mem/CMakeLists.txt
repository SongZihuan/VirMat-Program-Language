PROJECT(vtc_mem LANGUAGES C)

ADD_LIBRARY(vtc_mem SHARED ${vtc_mem_SOURCE_DIR}/mem.c)  # 将vtc mem编译为动态库
SET_TARGET_PROPERTIES(vtc_mem PROPERTIES OUTPUT_NAME "vtmem")  # 输出为 libvtmem.so 或 libvtmem.dll

INSTALL(TARGETS vtc_mem)