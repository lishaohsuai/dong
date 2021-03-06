/*
 * @Author: your name
 * @Date: 2020-12-02 09:06:35
 * @LastEditTime: 2020-12-03 16:00:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/debug.hpp
 */
#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__
#include <cstdio>
#include <iostream>
#include <string>

//#define __TT__

// #ifdef __TT__
// #define ERROR_D(...) \
// do{ \
//     fprintf(stderr, "[ERROR  ]%s %s(Line %d): ",__FILE__,__FUNCTION__,__LINE__); \
//     fprintf(stderr, __VA_ARGS__); \
// }while(0)
// #else
// #define ERROR_D(...)
// #endif

// #ifdef __TT__
// #define WARNING_D(...) \
// do{ \
//     fprintf(stdout, "[WARNING]%s %s(Line %d): ",__FILE__,__FUNCTION__,__LINE__); \
//     fprintf(stdout, __VA_ARGS__); \
// }while(0)
// #else
// #define WARNING_D(...)
// #endif

// #ifdef __TT__
// #define INFO_D(...) \
//     do{ \
//         fprintf(stdout, "[INFO  ]%s %s(Line %d): ",__FILE__,__FUNCTION__,__LINE__); \
//         fprintf(stdout, __VA_ARGS__); \
//     }while(0)
// #else
// #define INFO_D(...)
// #endif

#ifdef __TT__
#define DBG_D(format, ...)                                                                                             \
    do {                                                                                                               \
        std::string tout_xxdafdsaf = "[DEBUG] ";                                                                       \
        tout_xxdafdsaf += (std::string)format;                                                                         \
        printf(tout_xxdafdsaf.c_str(), ##__VA_ARGS__);                                                                 \
    } while (0)
#else
#define DBG_D(format, ...)                                                                                             \
    do {                                                                                                               \
    } while (0)
#endif

#ifdef __TT__
#define DBG_S(...)                                                                                                     \
    do {                                                                                                               \
        std::cout << __VA_ARGS__;                                                                                      \
    } while (0)
#else
#define DBG_S(...)                                                                                                     \
    do {                                                                                                               \
    } while (0)
#endif

// #ifdef __TT__
// #define SHOW_TIME(...) \
// do{\
//     extern unsigned long long gLatestTime;\
//     timeval tp;\
//     gettimeofday(&tp, NULL);\
//     unsigned long long now = tp.tv_sec*1000000+tp.tv_usec; \
//     if(gLatestTime != 0) \
//     { \
//         fprintf(stdout, ">>>>>>>>>Used Time: %s[%d], %s: %ld.%ld, %llu ms ", __FILE__, __LINE__, __func__, tp.tv_sec,
//         tp.tv_usec, (now-gLatestTime)/1000);\
//         fprintf(stdout, "/n"); \
//     } \
//     gLatestTime = now;\
// }while(0)
// #else
// #define SHOW_TIME(...)
// #endif

#endif
