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

#include "AbstractThreadObject.h"

#include <thread>
#include <mutex>

AbstractThreadObject::AbstractThreadObject() : m_isStopping(false), m_thread(nullptr), m_mutex(nullptr) {}

AbstractThreadObject::~AbstractThreadObject() {
	stop(true);
}

void AbstractThreadObject::start(void) {
	if (m_thread) return;
	m_isStopping = false;
	m_mutex = new std::mutex;
	startWorker(m_thread);
}

void AbstractThreadObject::stop(bool _join) {
	if (m_thread == nullptr) return;
	
	m_mutex->lock();
	m_isStopping = true;
	m_mutex->unlock();
	
	if (_join) join();
}

void AbstractThreadObject::join(void) {
	if (m_thread == nullptr) return;

	m_thread->join();
	delete m_thread;
	delete m_mutex;
	m_thread = nullptr;
	m_mutex = nullptr;
}

bool AbstractThreadObject::checkContinueRunning(void) {
	if (m_mutex == nullptr) return false;
	bool ret = false;
	m_mutex->lock();
	ret = !m_isStopping;
	m_mutex->unlock();
	return ret;
}