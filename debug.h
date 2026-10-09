// 26.10.2025 version of debug.h:
//     * ERROR() -> ERROR_LOG()
//     * Every macro requires ;

// shoul be renamed to colourfull_output.h (?)

// 09.10.2026
//      теперь единый DEBUG_VAR() и опциональный DEBUG_CHR()
//      FIXME: DEBUG_CHR(c++) with typeof(name) K_DEBUG_H_tmp_var

#ifndef K_DEBUG_H
#define K_DEBUG_H

#include <stdio.h>

#define RED_COLOR           "\33[31m"
#define GREEN_COLOR         "\33[32m"
#define YELLOW_COLOR        "\33[33m"
#define BLUE_COLOR          "\33[34m"
#define MAGENTA_COLOR       "\33[35m"

#define RED_BOLD_COLOR      "\33[1;31m"
#define GREEN_BOLD_COLOR    "\33[1;32m"
#define YELLOW_BOLD_COLOR   "\33[1;33m"
#define BLUE_BOLD_COLOR     "\33[1;34m"

#define COLOR_END           "\33[0m"

#ifdef PRINT_DEBUG
//     struct varInfo_t
//     {
//         const char *name;
//         const char *file;
//         int line;
//         const char *func;
//     };

    // You can use DEBUG_CHR to see it's value
    #define FORMAT_STR(pre, x, after) _Generic((x), \
        char:               pre "\'%c\'"    after,  \
        signed char:        pre "%hhd"      after,  \
        unsigned char:      pre "%hhu"      after,  \
        short:              pre "%hd"       after,  \
        unsigned short:     pre "%hu"       after,  \
        int:                pre "%d"        after,  \
        unsigned int:       pre "%u"        after,  \
        long:               pre "%ld"       after,  \
        unsigned long:      pre "%lu"       after,  \
        long long:          pre "%lld"      after,  \
        unsigned long long: pre "%llu"      after,  \
        float:              pre "%f"        after,  \
        double:             pre "%f"        after,  \
        long double:        pre "%Lf"       after,  \
        char *:             pre "\"%s\""    after,  \
        const char *:       pre "\"%s\""    after,  \
        void *:             pre "[%p]"      after)

    #define K_DEBUG_H_DEBUG_FORMAT \
            BLUE_BOLD_COLOR "[DEBUG]" COLOR_END " %s:%d:%s(): \t"

    // IMPORTANT! It is  forks only when you pass K_DEBUG_H_DEBUG_FORMAT in formatFinal
    // It works this way because you can't pass _Generic into printf and concatinate with another string
    // compiler fails of fprintf("string" _Generic((x), int: "%d") так что это не работает сук
    // так и живём
    #define K_DEBUG_H_DEBUG_FPRINTF(formatFinal, ...)                       \
            do {                                                            \
                fprintf(stderr, formatFinal,                   \
                        __FILE__, __LINE__, __func__, ##__VA_ARGS__);       \
            } while(0)

    #define DEBUG_LOG(format, ...)                                          \
            do {                                                            \
                K_DEBUG_H_DEBUG_FPRINTF(K_DEBUG_H_DEBUG_FORMAT format "\n", ##__VA_ARGS__);        \
            } while(0)

    #define DEBUG_PRINT(format, ...)                                             \
            do {                                                                 \
                fprintf(stderr, BLUE_BOLD_COLOR format COLOR_END, ##__VA_ARGS__);\
            } while(0)

    #define DEBUG_CHR(name)                                     \
            do {                                                \
                DEBUG_LOG ("%s = '%c'(%d)", #name, name, name); \
            } while(0)

    #define DEBUG_VAR(name)                                                     \
            do {                                                                \
                K_DEBUG_H_DEBUG_FPRINTF(                                        \
                        FORMAT_STR(K_DEBUG_H_DEBUG_FORMAT "%s = ", name, "\n"), \
                        #name, (name));                                         \
            } while (0)

    #define DEBUG_BITS(name, bitSize)                           \
            do                                                  \
            {                                                   \
                K_DEBUG_H_DEBUG_FPRINTF (K_DEBUG_H_DEBUG_FORMAT "%s = 0b", #name);      \
                for (unsigned int K_DEBUG_H_i = 0;                                      \
                    K_DEBUG_H_i < (bitSize);                                            \
                    K_DEBUG_H_i++) {                                                    \
                    fprintf (stderr, "%lu",                                             \
                            (uint64_t)(((name) >> ((bitSize) - K_DEBUG_H_i - 1)) & 1)); \
                }                                                                       \
                fprintf (stderr, "\n");                                                 \
            } while (0)
    #define ON_DEBUG(...) __VA_ARGS__
    #define ON_RELEASE(...)
#else
    #define DEBUG_LOG(format, ...)
    #define DEBUG_PRINT(format, ...)
    #define DEBUG_CHR(name)
    #define DEBUG_VAR(name)
    #define DEBUG_BITS(name, bitSize) 
    #define ON_DEBUG(...)
    #define ON_RELEASE(...) __VA_ARGS__
#endif // PRINT_DEBUG

#define PRINT(format, ...)                                                              \
        printf (GREEN_BOLD_COLOR format COLOR_END, ##__VA_ARGS__)

#define ERROR_LOG(format, ...)                                                          \
        fprintf (stderr, RED_BOLD_COLOR "[ERROR] %s:%d:%s(): " format "\n" COLOR_END,   \
                 __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define ERROR_PRINT(format, ...)                                                        \
        fprintf (stderr, RED_BOLD_COLOR format "\n" COLOR_END, ##__VA_ARGS__)

#endif // K_DEBUG_H