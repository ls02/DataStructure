/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-09-22 17:41:55
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-09-22 17:43:09
 * @FilePath: /DataStructure/tcp/src/log.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <string>
#include <sys/time.h>

// 日志是有日志级别的
#define DEBUG 0
#define NORMAL 1
#define WARNING 2
#define ERROR 3
#define FATAL 4

const char *gLevelMap[] = {
    "DEBUG",
    "NORMAL",
    "WARNING",
    "ERROR",
    "FATAL"};

#define LOGFILE "./threadpool.log"

// 完整的日志功能，至少: 日志等级 时间 支持用户自定义(日志内容, 文件行，文件名)
void logMessage(int level, const char *format, ...)
{
#ifndef DEBUG_SHOW
    if (level == DEBUG)
        return;
#endif
    // va_list ap;
    // va_start(ap, format);
    // while()
    // int x = va_arg(ap, int);
    // va_end(ap); //ap=nullptr
    char stdBuffer[1024]; // 标准部分
    time_t timestamp = time(nullptr);
    // struct tm *localtime = localtime(&timestamp);
    char now[64];
    struct tm *ttime;
    ttime = localtime(&timestamp);
    strftime(now, 64, "%Y-%m-%d %H:%M:%S", ttime);
    // snprintf(stdBuffer, sizeof stdBuffer, "[%s] [%ld] ", gLevelMap[level], timestamp);
    snprintf(stdBuffer, sizeof stdBuffer, "[%s] [%s] ", gLevelMap[level], now);

    char logBuffer[1024]; // 自定义部分
    va_list args;
    va_start(args, format);
    // vprintf(format, args);
    vsnprintf(logBuffer, sizeof logBuffer, format, args);
    va_end(args);

    FILE *fp = fopen(LOGFILE, "a");
    printf("%s%s\n", stdBuffer, logBuffer);
    // fprintf(fp, "%s%s\n", stdBuffer, logBuffer);
    fclose(fp);
}