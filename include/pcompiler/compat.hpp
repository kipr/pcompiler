#ifndef _PCOMPILER_COMPAT_HPP_
#define _PCOMPILER_COMPAT_HPP_

#ifdef _MSC_VER

#pragma section(".CRT$XCU", read)
#define INITIALIZER(f) \
   static void __cdecl f(void); \
   __declspec(allocate(".CRT$XCU")) void (__cdecl*f##_)(void) = f; \
   static void __cdecl f(void)
#define DLL_EXPORT __declspec(dllexport)

#else

#define INITIALIZER(f) \
	static void f(void) __attribute__((constructor)); \
	static void f(void)
#define DLL_EXPORT

#endif

#endif