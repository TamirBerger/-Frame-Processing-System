#ifndef __SystemApplication_H
#define __SystemApplication_H

//std
#include <iostream>
#include <string>
#include <atomic>
#include <vector>
#include <memory>
#include <stdexcept>
#include <thread>
#include <functional>
// project
#include "ProcessingModule.h"
#include "FastModule.h"
#include "SlowModule.h"
#include "FrameCapture.h"
#include "Logger.h"


class SystemApplication {
public:
	SystemApplication();
	SystemApplication(const std::string& filePath);
	void setFilePath(const std::string& filePath);
	void start();
	~SystemApplication() {}

private:
	Logger m_logger;
	std::string m_file_path;
	std::atomic<bool> m_done;
	std::vector<ProcessingModule*> m_processors;
	FrameCapture m_frame_capture;
	FastModule m_fast_module;
	SlowModule m_slow_module;
};

#endif