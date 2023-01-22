#ifndef CSI_H
#define CSI_H
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>

class CSICam
{
public:
	CSICam() {}
	~CSICam() {}
	bool open(std::string pipeline);
    void start();
    void stop();
    void release();
    cv::Mat& getFrame() { return frame; }

protected:
    void updateCam();
    bool read(cv::Mat& frame);

private:
	cv::VideoCapture cam;
    std::thread thrd;
    bool isrun;
    bool ret;
    cv::Mat frame;
};

std::string get_pipeline(int sensor_id, int capture_width, int flip, int capture_height, int framerate, int display_width, int display_height);
#endif

