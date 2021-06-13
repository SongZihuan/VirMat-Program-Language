/*
 * 文件名: signal.c
 * 目标: 信号处理的相关实用函数
 */

#include <signal.h>
#include "tool.h"

volatile struct SignalTag signal_tag = {.signum=0, .status=signal_reset};

void vtSignalHandler(int signum) {
    signal_tag.status = signal_appear;
    signal_tag.signum = signum;
    signal(signum, vtSignalHandler);  // vmSignalHandler 触发后，会和信号解除绑定，因此必须再次绑定
}