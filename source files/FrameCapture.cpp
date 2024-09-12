#include "FrameCapture.h"


// Ctor: Open m_cap with input video file
FrameCapture::FrameCapture(const std::string& filePath, Logger& logger) :
m_file_path(filePath), m_cap(filePath), m_logger(logger) {
	
	if (!m_cap.isOpened()) {
		m_logger.log(ERROR, "Uunable to open video file: " + filePath + 
			". in FrameCapture::" + std::string(__FUNCTION__));
	}
	else {
		m_logger.log(INFO, "Succeed to open video file: " + filePath + ". File info:");
		// log video file info
		m_logger.log(INFO, "Total frames: " + std::to_string(m_cap.get(cv::CAP_PROP_FRAME_COUNT)));
		m_logger.log(INFO, "Frame width: " + std::to_string(m_cap.get(cv::CAP_PROP_FRAME_WIDTH)));
		m_logger.log(INFO, "Frame height: " + std::to_string(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
		m_logger.log(INFO, "FPS: " + std::to_string(m_cap.get(cv::CAP_PROP_FPS)));
	}
}

// Read video frames one by one. Pass each frame and frame number to all processing modules 
void FrameCapture::readFrames(const std::vector<ProcessingModule*>& processors) {
	m_logger.log(DEBUG, "Start reading frames. in FrameCapture::" + std::string(__FUNCTION__));

	cv::Mat frame;
	unsigned int frameNumber = 0;

	try {
		while (m_cap.read(frame)) {
			frameNumber++;
			for (auto& processor : processors) {
				processor->passFrame(frame, frameNumber);
			}
		}
	}
	catch (const cv::Exception& e) {
		m_logger.log(ERROR, "OpenCV exception: " + std::string(e.what())
			+ " in FrameCapture::" + std::string(__FUNCTION__));
	}
	catch (...) {
		m_logger.log(CRITICAL, "Unknown error occurred in FrameCapture::" + std::string(__FUNCTION__));
	}
}

// Set a new video file: open m_cap with the new video
void FrameCapture::setFilePath(const std::string& filePath) {
	m_file_path = filePath;

	// Release the current video capture if it is opened
	if (m_cap.isOpened()) {
		m_cap.release();
	}

	// Open the new video file
	m_cap.open(filePath);

	if (!m_cap.isOpened()) {
		m_logger.log(ERROR, "Unable to open new video file: " + filePath + ". in FrameCapture::" + std::string(__FUNCTION__));
	}
	else {
		m_logger.log(INFO, "Succeed to open video file. File info:");
		// log video file info
		m_logger.log(INFO, "Total frames: " + std::to_string(m_cap.get(cv::CAP_PROP_FRAME_COUNT)));
		m_logger.log(INFO, "Frame width: " + std::to_string(m_cap.get(cv::CAP_PROP_FRAME_WIDTH)));
		m_logger.log(INFO, "Frame height: " + std::to_string(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
		m_logger.log(INFO, "FPS: " + std::to_string(m_cap.get(cv::CAP_PROP_FPS)));
	}
}

bool FrameCapture::validFile() {
	if (!m_cap.isOpened()) {
		return false;
	}
	return true;
}
