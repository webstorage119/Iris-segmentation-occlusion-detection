#include "Segmentation.hpp"

Segmentation::Segmentation(Eye* eye)
{
    this->eye = eye;
    this->rMin = round(eye->getImgHeight()/6);
    this->rMax = round(eye->getImgHeight()/2);
}

Segmentation::~Segmentation(){ eye = 0; }

// Getter
int Segmentation::getRMax(){ return this->rMax;}
int Segmentation::getRMin(){ return this->rMin;}


uchar Segmentation::pixelValue(Eye* eye, double angle, cv::Point center, int r, eEyePart eyePart)
{
	int x = round(r * cos(angle) + center.x);
	int y = round(r * sin(angle) + center.y);
	if(eyePart==iris)
    {
        uchar valueBlue = round(eye->getBlueInp()->at<uchar>(y,x) * 0.114); // 0.114 is the percentage of perception of the blue spec at the human eye
        uchar valueGreen = round(eye->getGreenInp()->at<uchar>(y,x) * 0.587); // 0.587 same but for green spec
        uchar valueRed = round(eye->getRedInp()->at<uchar>(y,x) * 0.299); // 0.299 same but for red spec
	    return valueBlue+valueGreen+valueRed;
    }
    else // eyePart==pupil
    {
        return eye->getPupilROI()->at<uchar>(y,x);
    }
}

int Segmentation::contourSum(Eye* eye, cv::Point center, int r, eEyePart eyePart)
{	// calcolo il contorno della circonferenza che va' da 0° a 45°, e da 135° a 360° (evito una probabile palpebra superiore)
	int sum = 0;
	int circPixels = 4*round(2*((1/(sqrt(2)) * r))); // amount of pixels on the circumference
	double theta = (2*M_PI)/circPixels; // step of angle to evaluate a single pixel value
	if(eyePart == iris)
    {
        for(double angle = 0; angle <= 2*M_PI/8; angle+=theta){ sum += pixelValue(eye, angle, center, r, eyePart); } // from 0° to 45° 
	    for(double angle = (2*M_PI*3)/8; angle <= (2*M_PI*5)/8; angle+=theta){ sum += pixelValue(eye, angle, center, r, eyePart); } // from 135° to 180°
	    for(double angle = (2*M_PI*7)/8; angle <= 2*M_PI; angle+=theta){ sum += pixelValue(eye, angle, center, r, eyePart); } // from 270° to 360°
        //for(double angle = (2*M_PI*3)/8; angle <= 2*M_PI; angle+=theta){ sum += pixelValue(eye, angle, center, r, eyePart); } // from 135° to 180°
    }
    else // eyePart == pupil
    {
        for(double angle = 0; angle <= 2*M_PI; angle+=theta)	// for pupil contour calculation i took the full circumference
        {
            sum += pixelValue(eye, angle, center, r, eyePart);
        }
    }
	return sum;
}

std::vector<int> Segmentation::linearIntegVec(Eye* eye, cv::Point center, std::vector<int> radiusRange, eEyePart eyePart)
{
	std::vector<int> lineIntegral;
	for( int i = 0; i < radiusRange.size(); i++ ){					// ignoro l'arco superiore ( > 45° fino a < 135° ) poiché non viene considerato all'interno del calcolo di integrale lineare
		//int p1_y = radiusRange[i] * sin(2*M_PI/8) + center.y;		// quindi prendo le coordinate y estreme ( a 45° e a 135° ) che mi consentono di calcolare l'integrale anche se la circonferenza
		//int p2_y = radiusRange[i] * sin((2*M_PI*3)/8) + center.y;	// non include l'arco superiore nell'immagine
        if( eyePart==iris && (center.x - radiusRange[i] < 0 || center.x + radiusRange[i] > eye->getImgWidth() || center.y + radiusRange[i] > eye->getImgHeight() || (center.y - radiusRange[i]) < 0 )) // oppure  p1_y < 0 || p2_y < 0
		{
			lineIntegral.push_back(0);
		}
        else if( eyePart==pupil && (center.x - radiusRange[i] < 0 || center.x + radiusRange[i] > eye->getPupilROI()->cols || center.y + radiusRange[i] > eye->getPupilROI()->rows || (center.y - radiusRange[i]) < 0))
        {
            lineIntegral.push_back(0);
        }
		else{
			int tmp = contourSum(eye, center, radiusRange[i], eyePart);
			lineIntegral.push_back(tmp);
		}
	}
	return lineIntegral;
}


void Segmentation::daugmanOperator(eEyePart eyePart)
{
    int rows = (eyePart==iris ? eye->getImgHeight() : eye->getPupilLen());
    int cols = (eyePart==iris ? eye->getImgWidth() : eye->getPupilLen());

    std::vector<int> radiusRange = eyePart==iris ? std::vector<int>(this->rMax-this->rMin+1) : std::vector<int>(round(eye->getIrisRadius()*0.85 - eye->getIrisRadius()/5));
    if(eyePart == iris)
        std::iota(begin(radiusRange), end(radiusRange), rMin);
    else std::iota(begin(radiusRange), end(radiusRange), eye->getIrisRadius()/5);
    cv::Point candidateCenter;
    int candidateRay;
    cv::Mat convolved;
    const int DELTA = (eyePart==iris ? DELTA_R : DELTA_PUP);
    double max = 0;
    double candidateVal = 0;
    int secR = rows/7; int secC = cols/7;
    for( int y = 0; y < rows; y++ )
    {
        for( int x = 0; x < cols; x++ )
        {
            if(eyePart==pupil && !(y>=(secR*3) && y <=(secR*4) && x>=(secC*3) && x<=(secC*4))) continue;
          //  if( (y < (rows/3) || y > (2*rows/3)) && eyePart==pupil ) continue;
		//	if( (x < (cols/3) || x > (2*cols/3)) && eyePart==pupil ) continue;
            cv::Point center(x,y);
            std::vector<int> lineInt = linearIntegVec(eye, center, radiusRange, eyePart);
            if( find(lineInt.begin(), lineInt.end(), 0) != lineInt.end() )
            {
                int index = getIndexOfZeros(lineInt); // from utils.hpp

                if( index == 0 || index < DELTA +1 ) continue;

                std::vector<int> newLineInt = slice(lineInt, 0, index);
                convolved = convolution(eye, &newLineInt, eyePart, center, radiusRange);
                double val = 0;
                for( index = 0; index < convolved.cols - ((DELTA/2)*2)-1; index++)
                {
                    val = abs(convolved.at<double>(0, index) - convolved.at<double>(1, index))/DELTA_R;
                    if( val > max )
                    {
                        max = val;
                        candidateVal = val;
                        candidateCenter = center;
                        candidateRay = radiusRange[index+(DELTA/2)+1];
                    }
                }
            }
            else
            {
                convolved = convolution(eye, &lineInt, eyePart, center, radiusRange);
                double val = 0;
                for( int index = 0; index < convolved.cols-((DELTA/2)*2)-1; index++)
                {
                    val = abs(convolved.at<double>(0, index) - convolved.at<double>(1, index))/DELTA_R;
                    if( val > max )
                    {
                        max = val;
                        candidateVal = val;
                        candidateCenter = center;
                        candidateRay = radiusRange[index+(DELTA/2)+1];
                    }
                }
            }
            convolved.release();
        }
    }
    //convolved = 0;
    eyePart == iris ? eye->setIrisCenter(candidateCenter) : eye->setPupilCenter(candidateCenter);
    eyePart == iris ? eye->setIrisRadius(candidateRay) : eye->setPupilRadius(candidateRay);
    eyePart == iris ? eye->setIrisValue(candidateVal) : eye->setPupilValue(candidateVal);
}


cv::Mat Segmentation::convolution(Eye* eye, vector<int>* lineInt, eEyePart eyePart, cv::Point center, std::vector<int> radiusRange){
	const int DELTA = (eyePart==iris ? DELTA_R : DELTA_PUP);
    vector<double> kernel = cv::getGaussianKernel(DELTA, SIGMA);	// creo il 1-D gaussian Kernel. La funzione ritorna un DELTA_R X 1 Mat, ma facendo cast a vector, diventa vetctor 1xDELTA_R
	int li_size = lineInt->size();
	cv::Mat target(2, li_size, CV_64F);
	int l = 0;
	double value; int divider;
	for( int pos = DELTA/2+1; pos < li_size-DELTA/2; pos++ ){// non prendo come centro i raggi che farebbero andare troppo a sinistra o troppo a destra i valori del kernel (i lati escono dal range)
		value = pixelConv(lineInt, &kernel, pos);
		if( eyePart==pupil )
        {
            divider = pupContourDivider(eye, radiusRange, pos-2, center);
		    value = value / ((double)divider);
        }
		target.at<double>(0,l) = value;
		
		value = pixelConv(lineInt, &kernel, pos-1);
        if( eyePart==pupil ) value = value / ((double)divider);
		target.at<double>(1,l) = value;
		
		l++;
	}
	return target;
}

double Segmentation::pixelConv(vector<int>* lineInt, vector<double>* kernel, int pos){
	double val = 0;
	int l = 0;
	int mean = kernel->size() / 2;	// prendo la metà parte intera inferiore del kernel
	int h = pos + mean;
	for( int j = 0; j < kernel->size(); j++ ){
		val += ((double) lineInt->at(h)) * kernel->at(j);
		h--;
	}
	return val;
}

int Segmentation::pupContourDivider(Eye* eye, vector<int> radiusRange, int pos, cv::Point center){
	int sum = 0; int r = radiusRange[pos];
	int circPixels = 4*round(2*((1/(sqrt(2)) * r)));
	double theta = (2*M_PI)/circPixels;
	int x; int y;
    cv::Mat* red = eye->getPupilROI();
	for(double angle = 0; angle <= 2*M_PI; angle+=theta){
		x = r * cos(angle) + center.x;
		y = r * sin(angle) + center.y;
		sum += red->at<uchar>(y,x);
	}
    red = 0;
	return sum;
}

void Segmentation::cropPupil(Eye* eye)
{
    try
    {
        cv::Point irisCenter = eye->getIrisCenter();
        int irisRay = eye->getIrisRadius();
        auto [xROI, yROI] = pair<int,int>(irisCenter.x - irisRay, irisCenter.y - irisRay);
        int edge = eye->getIrisRadius()*2;
        eye->setPupilLen(edge);
        CvRect roi = cvRect(xROI, yROI, edge, edge);
        cv::Mat eyeImg = *(eye->getEyeImg());
        cv::Mat pupilROI = eyeImg(roi);
        cv::Mat bgr[3];
        cv::split(pupilROI, bgr);
        eye->setPupilROI(&(bgr[2]));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}


void Segmentation::run()
{
    daugmanOperator(eEyePart{iris});
    cropPupil(eye);
    daugmanOperator(eEyePart{pupil});
}