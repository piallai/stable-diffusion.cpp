#ifndef __EXAMPLE_LOG_H__
#define __EXAMPLE_LOG_H__

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif  // _WIN32

#include "stable-diffusion.h"

extern bool log_verbose;
extern bool log_color;

std::string sd_basename(const std::string& path);
void print_utf8(FILE* stream, const char* utf8);
void log_print(sd_log_level_t level, const char* log, bool verbose, bool color);
void example_log_printf(sd_log_level_t level, const char* file, int line, const char* format, ...);

#ifdef SD_EXAMPLES_GLOVE_GUI
#undef interface //macro in combaseapi.h
#define GLOVE_ENABLE_JSON
#include "glove.h"
#endif

#define LOG_DEBUG(format, ...) example_log_printf(SD_LOG_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) example_log_printf(SD_LOG_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#ifndef SD_EXAMPLES_GLOVE_GUI
#define LOG_WARN(format, ...) log_printf(SD_LOG_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
#define LOG_WARN(format, ...) GlvApp::show(SlvStatus(SlvStatus::statusType::warning, format, ##__VA_ARGS__), true);
#endif
#ifndef SD_EXAMPLES_GLOVE_GUI
#define LOG_ERROR(format, ...) log_printf(SD_LOG_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
#define LOG_ERROR(format, ...) GlvApp::show(SlvStatus(SlvStatus::statusType::critical, format, ##__VA_ARGS__), true);
#endif

#endif  // __EXAMPLE_LOG_H__
