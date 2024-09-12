#ifndef __FastModule_H
#define __FastModule_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <string>

#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include "ProcessingModule.h"
#include "Logger.h"

#define TIME_INTERVAL 5


class FastModule : public ProcessingModule{
public:
	FastModule(Logger& logger) : ProcessingModule(logger), m_frame_count(0) {}
	void passFrame(const cv::Mat& frame, const unsigned int frameNumber);
	void start(const std::atomic<bool>& done);  // Start processing
	void finish();  // Notify that frames capture is done
	~FastModule() {}

private:
	std::atomic<unsigned int> m_frame_count;  // Frames count: resets avery 5 seconds 
	std::condition_variable m_cv;
	std::mutex m_cv_mutex;  // Condition variable mutex
};

#endif