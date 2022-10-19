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

#include "OS.h"
#include <iostream>

void os::printBuildInformation(void) {
#if defined (WIN32)
	std::cout << "StressTool (Windows 32-bit version)" << std::endl;
#elif defined (OS_WINDOWS)
	std::cout << "StressTool (Windows 64-bit version)" << std::endl;
#else
	std::cout << "StressTool" << std::endl;
#endif // WIN32
}

void os::clearConsole(void) {
#ifdef OS_WINDOWS
	system("cls");
#elif defined(OS_UNIX)
	system("clear");
#elif defined(OS_LINUX)
	system("clear");
#elif defined(OS_MAC)
	system("clear");
#endif
}