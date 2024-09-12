#include <iostream>
#include "SystemApplication.h"


SystemApplication::SystemApplication() : 
m_done(false), m_logger("log.txt"), m_frame_capture(m_logger),
m_fast_module(m_logger), m_slow_module(m_logger) {
    
    m_logger.log(DEBUG, "Create System Application. Setting a video file is required. in SystemApplication::" + std::string(__FUNCTION__));

    // Init processors vector with FastModule and SlowModule  
    m_processors.push_back(&m_fast_module);
    m_processors.push_back(&m_slow_module);
}

SystemApplication::SystemApplication(const std::string& filePath) :
m_file_path(filePath), m_done(false), m_logger("log.txt"),
m_frame_capture(filePath, m_logger), m_fast_module(m_logger), m_slow_module(m_logger) {

    m_logger.log(DEBUG, "Create System Application. in SystemApplication::" + std::string(__FUNCTION__));

    // Init processors vector with fast_module and slow_module
    m_processors.push_back(&m_fast_module);
    m_processors.push_back(&m_slow_module);
}

void SystemApplication::start() {
    m_done = false;

    // check if filePath is set
    if (!m_frame_capture.validFile()) {
        m_logger.log(ERROR, "Processing file " + m_file_path
            + " is canceled, Due to failure in video file opening.  in SystemApplication::" + std::string(__FUNCTION__));
        return;
    }

    m_logger.log(INFO, "Starting run with file: " + m_file_path + " in SystemApplication::" + std::string(__FUNCTION__));

    // Start read frames and precess them in parallel:
    // Create a thread to read frames
    std::thread frameCaptureThread(&FrameCapture::readFrames, &m_frame_capture, std::cref(m_processors));
    
    // Create threads for each processor's start method
    std::vector<std::thread> processorThreads;
    for (auto& processor : m_processors) {
        processorThreads.emplace_back(&ProcessingModule::start, processor, std::ref(m_done));
    }

    // Wait for frame capture thread to finish read all frames
    frameCaptureThread.join();
    m_logger.log(DEBUG, "Frame capture thread has finished. in SystemApplication::" + std::string(__FUNCTION__));

    // notify to all processing threads that frames capture finished
    m_done = true;
    for (auto& processor : m_processors) {
        processor->finish();
    }
    
    // Wait for all processor threads to finish
    for (auto& thread : processorThreads) {
        thread.join();
    }

    m_logger.log(INFO, "Finished to process file: " + m_file_path + " in SystemApplication::" + std::string(__FUNCTION__));
    // add info to log: overall read x frames...
}

// Set a new video file
void SystemApplication::setFilePath(const std::string& filePath) {
    m_file_path = filePath;
    m_frame_capture.setFilePath(filePath);
}
    