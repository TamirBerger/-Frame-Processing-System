#include "FastModule.h"


// Increment frame count by one
void FastModule::passFrame(const cv::Mat& frame, const unsigned int frameNumber) {
    m_frame_count++;
}

// Start processing: calculate and display frame rate per second every 5 seconds
void FastModule::start(const std::atomic<bool>& done) {
    m_logger.log(DEBUG, "Fast Module Start processing. in FastModule::" + std::string(__FUNCTION__));
    std::unique_lock<std::mutex> lock(m_cv_mutex);

    while (!done) {
        // Reset frame count
        m_frame_count = 0;
        // Wait for 5 seconds (TIME_INTERVAL) or until frames capture is done
        m_cv.wait_for(lock, std::chrono::seconds(TIME_INTERVAL), [&done] { return done.load(); });

        if (!done) {
            // Calculate and print frame rate after 5 seconds
            unsigned int currFrameCount = m_frame_count;
            double fps = (double)currFrameCount / TIME_INTERVAL;
            m_logger.log(INFO, "Frame Rate: " + std::to_string(fps) + " fps");
        }
    }
    m_logger.log(DEBUG, "Fast Module finished processing successfully. in FastModule::" + std::string(__FUNCTION__));
}

// Notify start() thread that frames capture finished
void FastModule::finish() {
    std::lock_guard<std::mutex> lock(m_cv_mutex);  // Acquire lock: ensure start() thread waits on cv 
    m_cv.notify_all();
}



