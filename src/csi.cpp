#include <csi.h>

bool CSICam::open(std::string pipeline)
{
    this->cam.open(pipeline, cv::CAP_GSTREAMER);
    this->ret = this->cam.read(this->frame);

    if (!this->ret)
    {
        std::cerr << "Unable to open Camera\n";
        std::cerr << "Pipeline: " << pipeline << "\n";
        this->release();
        return true;
    }
    else
    {
        this->isrun = false;
        return false;
    }
}

void CSICam::updateCam()
{
    while (this->isrun && this->ret)
    {
        if (this->cam.isOpened())
        {
            this->ret = this->read(this->frame);
        }
        else
        {
            std::cerr << "Video Capture is not open\n";
        }
    }
}

void CSICam::start()
{
    if (this->isrun)
    {
        std::cerr << "Video caputring is already running\n";
        return;
    }
    else
    {
        this->isrun = true;
        this->thrd = std::thread(&CSICam::updateCam, this);
    }
}

void CSICam::stop()
{
    this->isrun = false;
    this->thrd.join();
}

void CSICam::release()
{
    this->cam.release();
    if (this->isrun)
    {
        this->thrd.join();
    }
}

bool CSICam::read(cv::Mat& frame)
{
    this->ret = this->cam.read(this->frame);
    return ret;
}


std::string get_pipeline(int sensor_id, int capture_width, int capture_height, int flip, int framerate, int display_width, int display_height)
{
    std::string pipeline = "nvarguscamerasrc sensor-id=" + std::to_string(sensor_id) + " ! ";
    pipeline += "video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" + std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) + "/1 ! " ;
    pipeline += "nvvidconv flip-method=" + std::to_string(flip) + " ! ";
    pipeline += "video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)"+std::to_string(display_height) + ", format=(string)BGRx ! ";
    pipeline += "videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    return pipeline;
}
