#include <cassert>
#ifdef NDEBUG
#undef NDEBUG
//Release build assert is ignoring code inside the assertions - big nono
#define EmuAssert(expr) assert(expr);
#define NDEBUG
#else
#define EmuAssert(expr) assert(expr);
#endif