#include "Application.h"
#include "CPUstress.h"
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

Application::Application() : m_isExiting(false), m_cpuToStart(20), m_cpuCores(0) {}

Application::~Application() {

}

void Application::run(void) {
	m_isExiting = false;
	
	m_cpuCores = std::thread::hardware_concurrency();
	if (m_cpuCores > 0) m_cpuToStart = m_cpuCores * 2;

	while (!m_isExiting) {
		// Show menu
		os::clearConsole();
		os::printBuildInformation();
		std::cout << std::endl << "Machine information:" << std::endl;
		if (m_cpuCores == 0) std::cout << "\tNumber of CPU cores: <Failed to detect>" << std::endl;
		else std::cout << "\tNumber of CPU cores: " << m_cpuCores << std::endl;
		if (m_cpu.empty()) std::cout << "\tCPU stress:          STOPPED (" << m_cpuToStart << " threads to be started)" << std::endl;
		else std::cout << "\tCPU stress:          RUNNING (" << m_cpu.size() << " threads)" << std::endl;

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
		else m_errorMessage = "Unknown command";
	}
}

// ####################################################################################################

bool Application::anythingIsRunning(void) const {
	return !(m_cpu.empty());
}

std::string Application::readInput(void) const {
	std::cout << "Input: ";
	std::string inp;
	std::getline(std::cin, inp);
	if (inp.empty()) return readInput();
	else return inp;
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

void Application::stopAll(void) {
	if (!m_cpu.empty()) {
		for (auto c : m_cpu) {
			c->stop(true);
			delete c;
		}
		m_cpu.clear();
	}

	m_infoMessage = "All threads were stopped";
}