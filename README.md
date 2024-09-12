# Parallel Frame Processing System

The Parallel Frame Processing System captures frames from an MP4 video file and processes them in parallel using multithreaded modules. This README provides instructions for setting up the system and running it on a Linux OS.

1. Download the source code to a directory on your machine.
2. Install Required Packages via Requirements.txt file.
3. Run the following command to compile the program:
'g++ -std=c++11 -I/usr/include/opencv4 -o parallel_frame_processing_system main.cpp SystemApplication.cpp FrameCapture.cpp FastModule.cpp SlowModule.cpp ProcessingModule.cpp Logger.cpp -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_highgui'
4. Once the program is compiled, you can run it with the following command:
'./parallel_frame_processing_system <path_to_MP4_video_file>'
