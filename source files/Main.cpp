#include <iostream>
#include "SystemApplication.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_MP4_video_file>" << std::endl;
        return -1;
    }

    std::string videoFilePath = argv[1];
    SystemApplication app(videoFilePath);
    app.start();

    return 0;
}