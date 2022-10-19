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

#include "RAMstress.h"
#include "OS.h"

#include <list>
#include <thread>

#define BLOCK_SIZE 128
#define RAM_LIMIT 95.

#if defined (WIN32)
#define MAX_DATA_SIZE 0xFFF0
#else
#define MAX_DATA_SIZE 0xFFFFFFF0
#endif

RAMstress::RAMstress(bool _start) {
	if (_start) start();
}

RAMstress::~RAMstress() {}

void RAMstress::startWorker(std::thread *& _thread) {
	_thread = new std::thread(&RAMstress::workerFunction, this);
}

void RAMstress::workerFunction(void) {
	std::list<unsigned long long *> data;
	while (checkContinueRunning()) {
		if (os::physicalMemoryUsageInPercent() < RAM_LIMIT && data.size() < MAX_DATA_SIZE) {
			data.push_back(new unsigned long long[BLOCK_SIZE]);
		}
		else {
			for (auto e : data) delete[] e;
			data.clear();
		}
	}
	for (auto e : data) delete[] e;
}