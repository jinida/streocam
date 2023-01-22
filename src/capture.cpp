#include <opencv2/opencv.hpp>
#include <csi.h>
#include <iostream>

int main()
{
    std::string pipeline_1 = get_pipeline(0, 3280, 2464, 0, 21, 3280, 2464);
    std::string pipeline_2 = get_pipeline(1, 3280, 2464, 0, 21, 3280, 2464);
    CSICam cam_1, cam_2;
    
    if(cam_1.open(pipeline_1))
    {
        return -1;
    }
    if (cam_2.open(pipeline_2))
    {
        return -1;
    }
    
    cam_1.start(), cam_2.start();

    cv::Mat left_frame, right_frame, out;
    char key;
    int cnt = 1;
    
    cv::namedWindow("StreoCam", cv::WINDOW_NORMAL);
    cv::resizeWindow("StreoCam", 2560, 720);
    while (true)
    {
        left_frame = cam_1.getFrame();
        right_frame = cam_2.getFrame();
        cv::hconcat(left_frame, right_frame, out);
        cv::imshow("StreoCam", out);
        key = cv::waitKey(10);

        if (key == 27)
        {
            break;
        }
        else if(key == 97)
        {
            cv::imwrite("./src0_captures/camera_img_" + std::to_string(cnt) + ".png", left_frame);
            cv::imwrite("./src1_captures/camera_img_" + std::to_string(cnt) + ".png", right_frame);
            cnt++;
        }
        // std::cout << "key: " <<  key << "\n";
    }

    cam_1.stop(), cam_2.stop();
    cam_1.release(), cam_2.release();
    return 0;
}