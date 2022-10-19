#include "CPUstress.h"

#include <thread>
#include <mutex>

CPUstress::CPUstress(bool _start)
	: m_thread(nullptr), m_mutex(nullptr), m_isStopping(false)
{
	if (_start) start();
}

CPUstress::~CPUstress() {
	stop(false);
}

void CPUstress::start(void) {
	if (m_thread) return;
	m_isStopping = false;
	m_mutex = new std::mutex;
	m_thread = new std::thread(&CPUstress::workerFunction, this);
}

void CPUstress::stop(bool _join) {
	if (m_thread == nullptr) return;
	
	m_mutex->lock();
	m_isStopping = true;
	m_mutex->unlock();

	m_thread->join();
	delete m_thread;
	delete m_mutex;
	m_thread = nullptr;
	m_mutex = nullptr;
}

bool CPUstress::checkContinueRunning(void) {
	bool ret = false;
	m_mutex->lock();
	ret = !m_isStopping;
	m_mutex->unlock();
	return ret;
}

void CPUstress::workerFunction(void) {
	double * nr = new double;
	while (checkContinueRunning()) {
		for (int i = 0; i < 1000; i++) (*nr) += 1.144564542315345;
	}
	delete nr;
}