#include <boost/timer/timer.hpp>
#include "IrisSegm.hpp"

int main()
{
    /*
     should initialize a "string" in order to
     use some kind of path for the dataset
    */
    vector<boost::filesystem::path> ret;
    string ext{".JPG"};
    string path{"/home/max/Desktop/Univ/Magistrale/Biometrics_Systems/Utiris_inpout/RGB_Images/002/"};
    boost::filesystem::path root{path}; // root folder of the dataset
    get_all(root, ext, ret);

    boost::timer::auto_cpu_timer t; // when destructor is called the elapsed time is printed

    IrisSegm irSe(ret[0].string());
//    std::cout << irSe.getEye().getEyePath() << std::endl;
    cv::Mat img = *(irSe.getEye()->getEyeImg());
    cv::namedWindow("Image Window");
   /* cv::imshow("Image window", (img));
    cv::waitKey(0);*/
    irSe.run();
    
   /* cv::imshow("Image window", *(irSe.getEye()->getEyeImgRes()));
    cv::waitKey(0);
    cv::imshow("Image window", *(irSe.getEye()->getMask()));
    cv::waitKey(0);*/
    
   /* cv::Mat img_color = *(irSe.getEye()->getEyeImg());
    int radius = irSe.getEye()->getIrisRadius();
    cv::Point center = irSe.getEye()->getIrisCenter();
    int width = irSe.getEye()->getImgWidth();
    int height = irSe.getEye()->getImgHeight();

    if(width==0 || height == 0 || radius == 0 || center.x==0 || center.y==0)
    {
        std::cout << "something wrong happened" << endl;
    }*/
/*	int new_radius = radius*(img_color.cols/width);
    cv::Point new_center(irSe.getEye()->getIrisCenter().x*(img_color.cols/width), center.y*(img_color.rows/height));
    cv::circle(img_color, new_center, new_radius, cv::Scalar(0,0,255), 3);
*/
    cv::Mat immagine = *(irSe.getEye()->getEyeImgRes());
    cv::circle(immagine, irSe.getEye()->getIrisCenter(), irSe.getEye()->getIrisRadius(), cv::Scalar(0,0,255), 3);
    int xRoi = irSe.getEye()->getIrisCenter().x - irSe.getEye()->getIrisRadius();
    int yRoi = irSe.getEye()->getIrisCenter().y - irSe.getEye()->getIrisRadius();
    
	cv::Point centerPup(xRoi+irSe.getEye()->getPupilCenter().x, yRoi+irSe.getEye()->getPupilCenter().y);
    cv::circle(immagine, centerPup, irSe.getEye()->getPupilRadius(), cv::Scalar(255,0,0), 3);

    cv::imshow("Image Window", immagine);
    cv::waitKey(0);

    cv::Mat pupilROI = *(irSe.getEye()->getPupilROI());
    cv::namedWindow("Show Pupil");
    cv::imshow("Show Pupil", pupilROI);
    cv::waitKey(0);
    cv::destroyAllWindows();
    /*cv::imshow("Image window", *(irSe.getEye()->getEyeImgRes()));
    cv::waitKey(0);
    cv::imshow("Image window", *(irSe.getEye()->getMask()));
    cv::waitKey(0);
    cv::imshow("Image window", *(irSe.getEye()->getImgInp()));
    cv::waitKey(0);        
    cout << irSe.getEye()->getImgHeight() << endl;*/



}