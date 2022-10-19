/*
MIT License

Copyright (c) 2022 Alexander Kuester

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

	unsigned long long getTotalPhysicalMemory(void);
	unsigned long long getAvailablePhysicalMemory(void);
	double physicalMemoryUsageInPercent(void);

	unsigned long long getTotalVirtualMemory(void);
	unsigned long long getAvailableVirtualMemory(void);
	double virtualMemoryUsageInPercent(void);
}