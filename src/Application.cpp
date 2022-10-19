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

#include "Application.h"
#include "CPUstress.h"
#include "RAMstress.h"
#include "OS.h"

#include <iostream>
#include <string>
#include <sstream>
#include <thread>

template <class T> T toNumber(const std::string& _string, bool & _failed) {
	std::stringstream ss(_string);
	T v;
	ss >> v;
	_failed = false;
	if (ss.fail()) { _failed = true; }
	std::string rest;
	ss >> rest;
	if (!rest.empty()) { _failed = true; }
	return v;
}

// ####################################################################################################

Application::Application() : m_isExiting(false), m_cpuToStart(20), m_ramToStart(5), m_cpuCores(0), m_physicalMemoryTotal(0), m_virtualMemoryTotal(0) {}

Application::~Application() {

}

void Application::run(void) {
	m_isExiting = false;
	
	m_cpuCores = std::thread::hardware_concurrency();
	m_physicalMemoryTotal = os::getTotalPhysicalMemory();
	m_virtualMemoryTotal = os::getTotalVirtualMemory();
	if (m_cpuCores > 0) {
		m_cpuToStart = m_cpuCores * 2;
		m_ramToStart = m_cpuCores / 2;
	}
	if (m_ramToStart == 0) m_ramToStart = 1;

	while (!m_isExiting) {
		// Show menu
		os::clearConsole();
		os::printBuildInformation();
		std::cout << std::endl << "Machine information:" << std::endl;
		if (m_cpuCores == 0) std::cout << "\tNumber of CPU cores:   <Failed to detect>" << std::endl;
		else std::cout << "\tNumber of CPU cores:   " << m_cpuCores << std::endl;
		if (m_physicalMemoryTotal > 0)	std::cout << "\tTotal physical memory: " << memoryToString(m_physicalMemoryTotal) << std::endl;
		else std::cout << "\tTotal physical memory: <Failed to detect>" << std::endl;
		if (m_virtualMemoryTotal > 0)	std::cout << "\tTotal virtual memory:  " << memoryToString(m_physicalMemoryTotal) << std::endl;
		else std::cout << "\tTotal virtual memory:  <Failed to detect>" << std::endl;

		if (m_cpu.empty()) std::cout << "\tCPU stress:            STOPPED (" << m_cpuToStart << " threads to be started)" << std::endl;
		else std::cout << "\tCPU stress:            RUNNING (" << m_cpu.size() << " threads)" << std::endl;

		if (m_ram.empty()) std::cout << "\tRAM stress:            STOPPED (" << m_ramToStart << " threads to be started)" << std::endl;
		else std::cout << "\tRAM stress:            RUNNING (" << m_ram.size() << " threads)" << std::endl;

		// Show options
		std::cout << std::endl << "Options:" << std::endl;
		if (m_cpu.empty()) {
			std::cout << "\tcpu-t <number>  Will set the number of threads that should be started for the CPU stress" << std::endl;
			std::cout << "\tcpu" << std::endl;
			std::cout << "\t  -or-" << std::endl;
			std::cout << "\tcpu-start       Will start the CPU stress" << std::endl;
		}
		else {
			std::cout << "\tcpu" << std::endl;
			std::cout << "\t   -or-" << std::endl;
			std::cout << "\tcpu-stop        Will stop the CPU stress" << std::endl;
		}
		if (m_ram.empty()) {
			std::cout << "\tram-t <number>  Will set the number of threads that should be started for the RAM stress" << std::endl;
			std::cout << "\tram" << std::endl;
			std::cout << "\t  -or-" << std::endl;
			std::cout << "\tram-start       Will start the RAM stress" << std::endl;
		}
		else {
			std::cout << "\tram" << std::endl;
			std::cout << "\t   -or-" << std::endl;
			std::cout << "\tram-stop        Will stop the RAM stress" << std::endl;
		}

		if (anythingIsRunning()) {
			std::cout << "\tstop            Will stop any stress" << std::endl << std::endl;
		}
		std::cout << "\texit            Will exit the application" << std::endl << std::endl;

		// Show error and info message
		if (!m_errorMessage.empty()) std::cout << "[error]: " << m_errorMessage << std::endl << std::endl;
		m_errorMessage.clear();
		if (!m_infoMessage.empty()) std::cout << "[info]: " << m_infoMessage << std::endl << std::endl;
		m_infoMessage.clear();

		// Read input
		std::string inp = readInput();
		if (inp == "exit") { m_isExiting = true; stopAll(); }
		else if (inp == "stop") stopAll();
		else if ((inp == "cpu-start" && m_cpu.empty()) || (inp == "cpu" && m_cpu.empty())) startCpu();
		else if ((inp == "cpu-stop" && !m_cpu.empty()) || (inp == "cpu" && !m_cpu.empty())) stopCpu();
		else if (inp.find("cpu-t ") != std::string::npos) setCpuThreads(inp);
		else if ((inp == "ram-start" && m_ram.empty()) || (inp == "ram" && m_ram.empty())) startRam();
		else if ((inp == "ram-stop" && !m_ram.empty()) || (inp == "ram" && !m_ram.empty())) stopRam();
		else if (inp.find("ram-t ") != std::string::npos) setRamThreads(inp);
		else m_errorMessage = "Unknown command";
	}
}

// ####################################################################################################

bool Application::anythingIsRunning(void) const {
	return !m_cpu.empty() || !m_ram.empty();
}

std::string Application::readInput(void) const {
	std::cout << "Input: ";
	std::string inp;
	std::getline(std::cin, inp);
	if (inp.empty()) return readInput();
	else return inp;
}

std::string Application::memoryToString(unsigned long long _memoryInBytes) {
	enum memoryLevel {
		KB = 0,
		MB = 1,
		GB = 2,
		TB = 3,
		PB = 4
	};
	memoryLevel currentLevel = KB;
	double res = _memoryInBytes / 1024.;
	while (currentLevel < PB && res > 1024.) {
		res /= 1024.;
		currentLevel = (memoryLevel)(((int)currentLevel) + 1);
	}
	std::ostringstream retOut;
	retOut.precision(2);
	retOut << std::fixed << res;
	std::string ret = retOut.str();
	switch (currentLevel)
	{
	case KB: ret.append(" KB"); break;
	case MB: ret.append(" MB"); break;
	case GB: ret.append(" GB"); break;
	case TB: ret.append(" TB"); break;
	case PB: ret.append(" PB"); break;
	default: ret.append(" Unknown Unit"); break;
	}
	return ret;
}

void Application::startCpu(void) {
	for (unsigned int i = 0; i < m_cpuToStart; i++) m_cpu.push_back(new CPUstress(true));
	m_infoMessage = "CPU stress started with " + std::to_string(m_cpu.size()) + " threads";
}

void Application::stopCpu(void) {
	for (auto c : m_cpu) {
		c->stop(true);
		delete c;
	}
	m_cpu.clear();
	m_infoMessage = "CPU stress stopped";
}

void Application::setCpuThreads(const std::string& _command) {
	auto ix = _command.find(" ");
	if (ix == std::string::npos) {
		m_errorMessage = "Invalid number format for \"cpu-t <number>\": <number> is missing";
		return;
	}
	std::string nString = _command.substr(ix);
	bool failed = false;
	unsigned int n = toNumber<unsigned int>(nString, failed);
	if (failed) {
		m_errorMessage = "Invalid number format for \"cpu-t <number>\": Expected positive number";
		return;
	}
	else if (n == 0) {
		m_errorMessage = "Invalid number format for \"cpu-t <number>\": <number> must be greater than 0";
		return;
	}
	else m_cpuToStart = n;
}

void Application::startRam(void) {
	for (unsigned int i = 0; i < m_ramToStart; i++) m_ram.push_back(new RAMstress(true));
	m_infoMessage = "RAM stress started with " + std::to_string(m_ram.size()) + " threads";
}

void Application::stopRam(void) {
	std::cout << "Stopping RAM stress (Cleaning up memory... This may take a while)" << std::endl;
	for (auto r : m_ram) r->stop(false);
	for (auto r : m_ram) {
		r->join();
		delete r;
	}
	m_ram.clear();
	m_infoMessage = "RAM stress stopped";
}

void Application::setRamThreads(const std::string& _command) {
	auto ix = _command.find(" ");
	if (ix == std::string::npos) {
		m_errorMessage = "Invalid number format for \"ram-t <number>\": <number> is missing";
		return;
	}
	std::string nString = _command.substr(ix);
	bool failed = false;
	unsigned int n = toNumber<unsigned int>(nString, failed);
	if (failed) {
		m_errorMessage = "Invalid number format for \"ram-t <number>\": Expected positive number";
		return;
	}
	else if (n == 0) {
		m_errorMessage = "Invalid number format for \"ram-t <number>\": <number> must be greater than 0";
		return;
	}
	else m_ramToStart = n;
}

void Application::stopAll(void) {
	if (!anythingIsRunning()) return;

	std::cout << "Stopping all threads" << std::endl;
	for (auto r : m_ram) r->stop(false);
	for (auto c : m_cpu) c->stop(false);

	std::cout << "Stopping RAM stress (Cleaning up memory... This may take a while)" << std::endl;
	for (auto r : m_ram) {
		r->join();
		delete r;
	}
	m_ram.clear();

	std::cout << "Stopping CPU stress" << std::endl;
	for (auto c : m_cpu) {
		c->join();
		delete c;
	}
	m_cpu.clear();

	m_infoMessage = "All threads were stopped";
}