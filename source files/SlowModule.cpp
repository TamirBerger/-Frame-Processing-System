#include "SlowModule.h"


SlowModule::SlowModule(Logger& logger) : ProcessingModule(logger) {
    // Open journal file in append mode
    m_journal_file.open("journal.txt", std::ios::app);
    if (!m_journal_file.is_open()) {
        logger.log(ERROR, "Failed to open Journal file. in SlowModule::" + std::string(__FUNCTION__));
    }
}

// Pass frame to SlowModule: add frame into the queue. Drop oldest frame if queue is full. 
void SlowModule::passFrame(const cv::Mat& frame, const unsigned int frameNumber) {
    std::lock_guard<std::mutex> lock(m_queue_mutex);  // Acquire lock
    // Critical section: modifying shared resources: framesQueue and journal file
    
    if (m_frames_queue.size() == MAX_Q_SIZE) {
        logToJournal("Dropped frame: " + std::to_string(m_frames_queue.front().second));
        m_frames_queue.pop();
    }
    // Push a deep copy of the frame into framesQueue
    m_frames_queue.push({ frame.clone(), frameNumber });
    
    // End of critical section
    // (automatic release of queueMutex is handled by the destructor of unique_lock)
}

// Start processing: pop and save frames from the frames Queue
void SlowModule::start(const std::atomic<bool>& done) {
    m_logger.log(DEBUG, "Slow Module Start processing. in SlowModule::" + std::string(__FUNCTION__));

    while (true) {
        std::unique_lock<std::mutex> lock(m_queue_mutex);  // Acquire lock
        // Critical section: modifying shared resource - framesQueue
        
        if (done && m_frames_queue.empty()) {
            // Exit the loop if frames capture is done and no more frames in framesQueue
            m_logger.log(DEBUG, "Slow Module finished processing successfully. in SlowModule::" + std::string(__FUNCTION__));
            break;  
        }

        if (!m_frames_queue.empty()) {
            // Make a local copy of the frame, enable to save it after the critical section
            std::pair<cv::Mat, unsigned int> frameData = m_frames_queue.front();
            m_frames_queue.pop();
            // Log journal inside the critical section for the good of the journal order
            logToJournal("Saved frame: " + std::to_string(frameData.second));
            
            // End of critical section
            lock.unlock(); // Release queueMutex 
            
            try{
                // save frame in the working directory
                cv::imwrite("frame_" + std::to_string(frameData.second) + ".jpg", frameData.first);
            }
            catch (const cv::Exception& e) {
                m_logger.log(ERROR, "Failed to save frame " + std::to_string(frameData.second)
                    + ": " + e.what());
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_PERIOD_MS)); // sleep for SLEEP_PERIOD_MS
        }
    }   
}

// Logs a message (saved/dropped frame) to journal file
void SlowModule::logToJournal(const std::string& message) {
    // Get current time
    time_t now = time(0);
    tm timeinfo;
    localtime_r(&now, &timeinfo);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);

    std::ostringstream logEntry;
    logEntry << "[" << timestamp << "] " << message << std::endl;

    // The writing is thread safe as logToJournal() called
    // only from Queue critical section, which protected by QueueMutex   

    // Output to journal file
    if (m_journal_file.is_open()) {
        m_journal_file << logEntry.str();
        m_journal_file.flush();  // Ensure immediate write to file
    }
    else {
        m_logger.log(ERROR, "journal file isn't open. in SlowModule::" + std::string(__FUNCTION__));
    }
}
