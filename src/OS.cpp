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