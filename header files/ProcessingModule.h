#ifndef __ProcessingModule_H
#define __ProcessingModule_H

#include <iostream>
#include <atomic>

#include <opencv2/opencv.hpp>
#include "Logger.h"


class ProcessingModule {
public:
	ProcessingModule(Logger& logger) : m_logger(logger) {}
	virtual void passFrame(const cv::Mat& frame, const unsigned int frameNumber) = 0;
	virtual void start(const std::atomic<bool>& done) = 0;
	virtual void finish() = 0;
	virtual ~ProcessingModule() = 0;

protected:
	Logger& m_logger;
};

#endif