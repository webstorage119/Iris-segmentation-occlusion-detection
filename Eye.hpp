#pragma once
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>
#include <map>

class Eye
{
    private:
        std::string eyePath; // absolute path
        int resWidth, resHeight; // width and height of the eyeImgRes (so dimensions post-resizing)
        int irisRadius; cv::Point irisCenter; double irisValue;
        int pupilRadius; cv::Point pupilCenter; double pupilValue;
        cv::Mat img;
        cv::Mat eyeImg;  // same image as eyeImg but resized with the resize method
        cv::Mat imgInp;
        cv::Mat blueInp, greenInp, redInp;
        cv::Mat pupilROI;
        int pupilLen;
        cv::Mat normImg;
        std::map<size_t, cv::Point> eyeCoords;
        cv::Mat binMask;
        cv::Mat normMask;

    public:
        Eye(std::string eyePath);
        ~Eye();
        
        void resize(int width, int height);

        // getter
        std::string getEyePath();
        cv::Mat* getImg();
        int getImgHeight();
        int getImgWidth();
        cv::Mat* getEyeImg();
        cv::Mat* getImgInp();
        int getIrisRadius();
        double getIrisValue();
        cv::Point getIrisCenter();
        int getPupilRadius();
        double getPupilValue();
        cv::Point getPupilCenter();
        cv::Mat* getBlueInp();
        cv::Mat* getGreenInp();
        cv::Mat* getRedInp();
        cv::Mat* getPupilROI();
        int getPupilLen();
        cv::Mat* getNormImg();
        std::map<size_t, cv::Point>* getEyeCoords();
        cv::Mat* getBinMask();
        cv::Mat* getNormMask();


        // setter
        void setImgInp(cv::Mat* imgInp);
        void setIrisRadius(int radius);
        void setIrisCenter(cv::Point center);
        void setIrisValue(double value);
        void setPupilRadius(int radius);
        void setPupilCenter(cv::Point center);
        void setPupilValue(double value);
        void setBlueInp(cv::Mat* specInp);
        void setGreenInp(cv::Mat* specInp);
        void setRedInp(cv::Mat* specInp);
        void setPupilROI(cv::Mat* pupilROI);
        void setPupilLen(int len);
        void setNormImg(cv::Mat* normImg);
        void setEyeCoords(std::map<size_t, cv::Point>* coords);
        void setBinMask(cv::Mat* binMask);
        void setNormMask(cv::Mat* normMask);
};