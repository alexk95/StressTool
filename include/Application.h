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

#include <list>
#include <string>

class CPUstress;
class RAMstress;

class Application {
public:
	Application();
	virtual ~Application();

	void run(void);

private:
	bool anythingIsRunning(void) const;
	std::string readInput(void) const;
	std::string memoryToString(unsigned long long _memoryInBytes);

	void startCpu(void);
	void stopCpu(void);
	void setCpuThreads(const std::string& _command);
	
	void startRam(void);
	void stopRam(void);
	void setRamThreads(const std::string& _command);

	void stopAll(void);

	unsigned int			m_cpuCores;
	unsigned long long		m_physicalMemoryTotal;
	unsigned long long		m_virtualMemoryTotal;

	bool					m_isExiting;
	std::string				m_errorMessage;
	std::string				m_infoMessage;

	unsigned int			m_cpuToStart;
	std::list<CPUstress *>	m_cpu;

	unsigned int			m_ramToStart;
	std::list<RAMstress *>	m_ram;

};