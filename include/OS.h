#pragma once

// WINDOWS
#if defined(_WIN32) || defined(_WIN64)
//! The current OS is windows
#define OS_WINDOWS

// UNIX
#elif defined(unix) || defined(__unix) || defined (__unix__)
//! The current OS is Unix
#define OS_UNIX

// APPLE
#elif defined(__APPLE__) || defined(__MACH__)
//! The current OS is Mac
#define OS_MAC

// LINUX
#elif defined(__linux__) || defined(linux) || defined(__linux)
//! The current OS is Linux
#define OS_LINUX
#endif

namespace os {

	void printBuildInformation(void);

	void clearConsole(void);

}