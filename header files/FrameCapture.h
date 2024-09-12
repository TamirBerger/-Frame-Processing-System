#ifndef __FrameCapture_H
#define __FrameCapture_H

// std
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
// opencv
#include <opencv2/opencv.hpp>    
#include <opencv2/videoio.hpp>

#include "ProcessingModule.h"
#include "Logger.h"


class FrameCapture{
public:
	FrameCapture(Logger& logger) : m_logger(logger) {}
	FrameCapture(const std::string& filePath, Logger& logger);
	void setFilePath(const std::string& filePath);
	void readFrames(const std::vector<ProcessingModule*>& processors);
	bool validFile();
	~FrameCapture() {}

private:
	std::string m_file_path; // unnecessary - can delete it
	cv::VideoCapture m_cap;
	Logger& m_logger;
};

#endif