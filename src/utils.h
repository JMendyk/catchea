/**
 * @file utils.h
 * @author JMendyk
 * @date 06.01.18
 */

#ifndef CATCHEA_UTILS_H
#define CATCHEA_UTILS_H

#include <libgen.h>

/**
 * Get current time in high resolution
 * @return Current time
 */
double get_time();

#define START_BENCH(name) double  bench_start_##name = get_time();
#define STOP_BENCH(name) double  bench_stop_##name = get_time();
#define GET_BENCH(name) (bench_stop_##name - bench_start_##name)

/**
 * Get basename given path
 * @param path path
 * @return basename of path
 */
char* util_basename(char* path);

#endif //CATCHEA_UTILS_H
