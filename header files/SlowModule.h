#ifndef __SlowModule_H
#define __SlowModule_H

#include <iostream>
#include <fstream>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <utility>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>


#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include "ProcessingModule.h"
#include "Logger.h"

#define SLEEP_PERIOD_MS 50
#define MAX_Q_SIZE 5


class SlowModule : public ProcessingModule {
public:
	SlowModule(Logger& logger);
	void passFrame(const cv::Mat& frame, const unsigned int frameNumber);
	void start(const std::atomic<bool>& done);  // start processing
	void finish() {}
	~SlowModule() {}

private:
	std::queue<std::pair<cv::Mat, unsigned int>> m_frames_queue;
	std::condition_variable m_cv;
	std::mutex m_queue_mutex;
	std::ofstream m_journal_file;

	void logToJournal(const std::string& message);  // Log saved/dropped frame to journal file
};

#endif