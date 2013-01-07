#ifndef LIBLDETECT_COMMON
#define LIBLDETECT_COMMON

#include <string>
#include <sstream>
#include <memory>

#include "libldetect.h"

#define NON_EXPORTED __attribute__((visibility("hidden")))

#pragma GCC visibility push(hidden) 

namespace ldetect {

extern const std::string table_name_dir NON_EXPORTED;

std::string hexFmt(uint32_t value, uint8_t w = 4, bool prefix = true);

struct kmod_ctx* modalias_init(void) NON_EXPORTED;
#pragma GCC visibility push(default)
std::string modalias_resolve_module(struct kmod_ctx *ctx, const std::string &modalias) EXPORTED;
#pragma GCC visibility pop

#define MAX_DEVICES 300
#define BUF_SIZE 512

#define psizeof(a) (sizeof(a) / sizeof(a[0]))
#define ifree(p) do { if (p) { free(p); p = nullptr; } } while (0)
#define alloc_v(v) calloc(1, sizeof(v[0]));

#ifdef __UCLIBCXX_MAJOR__
typedef std::auto_ptr<std::istream> instream;
#else
typedef std::unique_ptr<std::istream> instream;
#endif
instream i_open(std::string &&name) NON_EXPORTED;
instream fh_open(std::string &&name) NON_EXPORTED;
}

#pragma GCC visibility pop

#endif
