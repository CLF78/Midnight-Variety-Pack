#ifdef __cplusplus
extern "C" {
#endif

void OSReport(const char* format, ...);
#define DEBUG_REPORT(c, ...) if (DEBUG) { OSReport(c, __VA_ARGS__); }

#ifdef __cplusplus
}
#endif
