#include "Eye.hpp"

Eye::Eye(std::string eyePath)
{
    this->eyePath = eyePath;
    this->img = cv::imread(eyePath,1);
}

Eye::~Eye()
{
    this->img.release();
    this->eyeImg.release();
    this->imgInp.release();
    this->blueInp.release();
    this->greenInp.release();
    this->redInp.release();
    this->pupilROI.release();
    std::cout << "every field of Eye object has been released" << std::endl;
}

// getter
std::string Eye::getEyePath(){ return this->eyePath; }

int Eye::getImgHeight(){ return this->resHeight; }
int Eye::getImgWidth(){ return this->resWidth; }
int Eye::getIrisRadius(){ return this->irisRadius; }
double Eye::getIrisValue(){ return this->irisValue; }
cv::Point Eye::getIrisCenter(){ return this->irisCenter; }
int Eye::getPupilRadius(){ return this->pupilRadius; }
double Eye::getPupilValue(){ return this->pupilValue; }
cv::Point Eye::getPupilCenter(){ return this->pupilCenter; }
int Eye::getPupilLen(){ return this->pupilLen; }

cv::Mat* Eye::getImg(){ return &(this->img); }
cv::Mat* Eye::getEyeImg(){ return &(this->eyeImg); }
cv::Mat* Eye::getImgInp(){ return &(this->imgInp); }
cv::Mat* Eye::getBlueInp(){ return &(this->blueInp); }
cv::Mat* Eye::getGreenInp(){ return &(this->greenInp); }
cv::Mat* Eye::getRedInp(){ return &(this->redInp); }
cv::Mat* Eye::getPupilROI(){ return &(this->pupilROI); }

std::map<size_t, cv::Point>* Eye::getEyeCoords(){ return &(this->eyeCoords);}

// getter for Normalization
cv::Mat* Eye::getNormImg(){ return &(this->normImg); }
cv::Mat* Eye::getBinMask(){ return &(this->binMask); }

cv::Mat* Eye::getNormMask(){ return &(this->normMask); }

// setter for Preprocessing
void Eye::setImgInp(cv::Mat* imgInp){ this->imgInp = *imgInp; }
void Eye::setBlueInp(cv::Mat* specInp){ this->blueInp = *specInp; }
void Eye::setGreenInp(cv::Mat* specInp){ this->greenInp = *specInp; }
void Eye::setRedInp(cv::Mat* specInp){ this->redInp = *specInp; }

// setter for Segmentation
void Eye::setIrisCenter(cv::Point center){ this->irisCenter = center; }
void Eye::setIrisRadius(int radius){ this->irisRadius = radius; }
void Eye::setIrisValue(double value){ this->irisValue = value; }
void Eye::setPupilCenter(cv::Point center){ this->pupilCenter = center; }
void Eye::setPupilRadius(int radius){ this->pupilRadius = radius; }
void Eye::setPupilValue(double value){ this->pupilValue = value; }
void Eye::setPupilROI(cv::Mat* pupilROI){ this->pupilROI = *pupilROI; }
void Eye::setPupilLen(int len){ this->pupilLen = len; }

// setter for Normalization
void Eye::setNormImg(cv::Mat* normImg){ this->normImg = *normImg; }
void Eye::setBinMask(cv::Mat* binMask){ this->binMask = *binMask; }

void Eye::setEyeCoords(std::map<size_t, cv::Point>* eyeCoords){ this->eyeCoords = *eyeCoords; }

void Eye::setNormMask(cv::Mat* normMask){ this->normMask = *normMask; }

void Eye::resize(int width, int height)
{
    std::cout << "width " << width << " , height " << height << std::endl;
    cv::resize((this->img), this->eyeImg, cv::Size(width, height));
    this->resWidth = width; this->resHeight = height;
}