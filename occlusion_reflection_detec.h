#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <vector>
#include "polynomial_regression.h"


#define NUM_RAYS 46
#define KSIZE 3

using namespace cv;
using namespace std;

/**
 * @file
 * @brief individuazione dei riflessi e occlussioni
 */

int drawRays(Mat* norm_img);

void initKernels();

int initRayPos(Mat* normImg);

double pixelConvolution(Mat* normImg, int x, int y, int ray);

Mat upperEyelidDetection(Mat* normImg, string path);

vector<double> localMinima(vector<double> vec);

vector<int> getXOutliers(vector<int>* vec_x, vector<int>* vec_y);

Mat lowerEyelidDetection(Mat* normImg);

Mat threshReflectionDetection(Mat* normImg, int ksize, double c);