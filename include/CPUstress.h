#pragma once

namespace std { class thread; }
namespace std { class mutex; }

class CPUstress {
public:
	CPUstress(bool _start);
	virtual ~CPUstress();

	void start(void);
	void stop(bool _join);

private:
	bool checkContinueRunning(void);
	void workerFunction(void);

	bool			m_isStopping;
	std::thread *	m_thread;
	std::mutex *	m_mutex;

	CPUstress() = delete;
	CPUstress(CPUstress&) = delete;
	CPUstress& operator = (CPUstress&) = delete;
};