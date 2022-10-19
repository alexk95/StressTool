#pragma once

#include <list>
#include <string>

class CPUstress;

class Application {
public:
	Application();
	virtual ~Application();

	void run(void);

private:
	bool anythingIsRunning(void) const;
	std::string readInput(void) const;

	void startCpu(void);
	void stopCpu(void);
	void setCpuThreads(const std::string& _command);
	void stopAll(void);

	unsigned int			m_cpuCores;
	bool					m_isExiting;
	std::string				m_errorMessage;
	std::string				m_infoMessage;

	unsigned int			m_cpuToStart;
	std::list<CPUstress *>	m_cpu;

};