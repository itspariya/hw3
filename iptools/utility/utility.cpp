#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

using namespace cv;

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_gray(Mat &src, Mat &tgt)
{
	cvtColor(src, tgt, COLOR_BGR2GRAY);
}

/*-----------------------------------------------------------------------**/
void utility::cv_avgblur(Mat &src, Mat &tgt, int WindowSize)
{
	blur(src,tgt,Size(WindowSize,WindowSize));
}
/*-----------------------------------------------------------------------**/
void utility::histogramStretching(image &src, image &tgt, int A, int B)
{
    int I_min = MINRGB;
    int I_max = MAXRGB;
    int c = 1.05 * I_min;
    int d = 0.95 * I_max;

    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    for (int i=0; i<src.getNumberOfRows(); i++)
    {
        for (int j=0; j<src.getNumberOfColumns(); j++)
        {
            int pixelValue = src.getPixel(i, j);

            if (pixelValue < c)
                tgt.setPixel(i, j, A);
            else if (pixelValue > d)
                tgt.setPixel(i, j, B);
            else
            {
                // Mapping from [c,d] to [A,B]
                int stretchedValue = A + (pixelValue - c) * (B - A) / (d - c);
                tgt.setPixel(i, j, checkValue(stretchedValue));
            }
        }
    }
}
/*-----------------------------------------------------------------------**/

void utility::histogramStretch(Mat& src, Mat& tgt, int A, int B) 
{    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }

    // Find I-min and I-max
    double Imin, Imax;
    cv::minMaxLoc(gray, &Imin, &Imax);

    // Define c and d
    double c = 1.05 * Imin;
    double d = 0.95 * Imax;

    cv::Mat stretched = gray.clone();
    for (int y = 0; y < gray.rows; y++) {
        for (int x = 0; x < gray.cols; x++) {
            if (gray.at<uchar>(y, x) < c) {
                stretched.at<uchar>(y, x) = A;
            } else if (gray.at<uchar>(y, x) > d) {
                stretched.at<uchar>(y, x) = B;
            } else {
                stretched.at<uchar>(y, x) = 
                    static_cast<uchar>(A + ((gray.at<uchar>(y, x) - c) * (B - A) / (d - c)));
            }
        }
    }


    // Compute and plot histograms
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::Mat histOrig, histStretch;
    cv::calcHist(&gray, 1, 0, cv::Mat(), histOrig, 1, &histSize, &histRange, true, false);
    cv::calcHist(&stretched, 1, 0, cv::Mat(), histStretch, 1, &histSize, &histRange, true, false);

    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    cv::Mat histImageOrig(hist_h, hist_w, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat histImageStretch(hist_h, hist_w, CV_8UC3, cv::Scalar(255, 255, 255));

    normalize(histOrig, histOrig, 0, histImageOrig.rows, cv::NORM_MINMAX, -1, cv::Mat());
    normalize(histStretch, histStretch, 0, histImageStretch.rows, cv::NORM_MINMAX, -1, cv::Mat());

    for (int i = 1; i < histSize; i++) {
        line(histImageOrig, cv::Point(bin_w * (i - 1), hist_h - cvRound(histOrig.at<float>(i - 1))),
             cv::Point(bin_w * (i), hist_h - cvRound(histOrig.at<float>(i))), cv::Scalar(0, 0, 0), 2, 8, 0);

        line(histImageStretch, cv::Point(bin_w * (i - 1), hist_h - cvRound(histStretch.at<float>(i - 1))),
             cv::Point(bin_w * (i), hist_h - cvRound(histStretch.at<float>(i))), cv::Scalar(0, 0, 0), 2, 8, 0);
    }
     tgt = stretched.clone(); 

    // Save histogram images
    cv::imwrite("histOrig.png", histImageOrig);
    cv::imwrite("histStretch.png", histImageStretch);
}
/*-----------------------------------------------------------------------**/
void utility::histogramEqualization(Mat &src, Mat &tgt)
{
    // Convert the input image to gray level
    cv_gray(src, tgt);

    // Apply histogram equalization
    equalizeHist(tgt, tgt);


}
/*-----------------------------------------------------------------------**/
void utility::equalizeDarkPixels(Mat &src, Mat &tgt, int threshold) {
    // Convert the image to grayscale if not already
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    // Create the mask for dark pixels
    cv::Mat mask = (gray < threshold);

    // Equalize the dark pixels
    cv::Mat equalized;
    cv::equalizeHist(gray, equalized);

    // Using the mask, set the dark pixels in the original image to the equalized values
    gray.setTo(equalized, mask);

    tgt = gray;
}

/*-----------------------------------------------------------------------**/

void utility::greyAugmentedImages(Mat &src, Rect &ROI_rect, int A, int B)
{
    // Extract ROI from the input image
    cv::Mat ROI = src(ROI_rect);

    // a. Rotate original ROI
    imwrite("Original_Roi.jpg", ROI);
    
    // Rotate and save original ROI
    for(int angle = 1; angle <= 3; angle++)
    {
        Mat rotated;
        cv::rotate(ROI, rotated, angle-1); // Note the rotation code is used directly        
        imwrite("Rotated_ROI_" + std::to_string(angle*90) + ".jpg", rotated);
    }

    // b. Histogram Stretching and then rotate
    Mat stretchedROI;
    utility::histogramStretch(ROI, stretchedROI, A, B);
    imwrite("Stretched_Roi.jpg", stretchedROI);
    
    // Rotate and save stretched ROI
    for(int angle = 1; angle <= 3; angle++)
    {
        Mat rotated;
        cv::rotate(stretchedROI, rotated, angle-1);
        imwrite("Stretched_Rotated_ROI_" + std::to_string(angle*90) + ".jpg", rotated);
    }

    // c. Histogram Equalization and then rotate
    Mat equalizedROI;
    utility::histogramEqualization(ROI, equalizedROI);
    imwrite("Equalized_Roi.jpg", equalizedROI);
    
    // Rotate and save equalized ROI
    for(int angle = 1; angle <= 3; angle++)
    {
        Mat rotated;
        cv::rotate(equalizedROI, rotated, angle-1);
        imwrite("Equalized_Rotated_ROI_" + std::to_string(angle*90) + ".jpg", rotated);
    }
}

/*-----------------------------------------------------------------------**/
void utility::equalizeRGB(Mat& src, Mat& tgt, const std::string& channels)
{
    // Ensure the source image is in color
    if(src.channels() != 3) 
    {
        std::cerr << "Source image is not a color image" << std::endl;
        return;
    }

    std::vector<cv::Mat> rgbChannels(3);
    cv::split(src, rgbChannels);

   if(channels.find('R') != std::string::npos) 
    {
        cv::equalizeHist(rgbChannels[2], rgbChannels[2]);
        cv::imwrite("Equalized_R.jpg", rgbChannels[2]);
    }
    if(channels.find('G') != std::string::npos) 
    {
        cv::equalizeHist(rgbChannels[1], rgbChannels[1]);
        cv::imwrite("Equalized_G.jpg", rgbChannels[1]);
    }
    if(channels.find('B') != std::string::npos) 
    {
        cv::equalizeHist(rgbChannels[0], rgbChannels[0]);
        cv::imwrite("Equalized_B.jpg", rgbChannels[0]);
    }

    cv::merge(rgbChannels, tgt);

    try {
        cv::cvtColor(rgbChannels, tgt, cv::COLOR_HSV2BGR);
    } catch(cv::Exception& e) {
        std::cerr << "OpenCV exception: " << e.what() << std::endl;
        std::cerr << "Error while converting from HSV to BGR." << std::endl;
        return;
    }

    cv::imwrite("Equalized_RGB_infunction.jpg", tgt);
}
/*-----------------------------------------------------------------------**/

void utility::equalizeHSI(Mat& src, Mat& tgt, bool equalizeH, bool equalizeS, bool equalizeI)
{   
    if(src.channels() != 3) 
    {
        std::cerr << "Source image is not a color image" << std::endl;
        return;
    }

    cv::Mat hsi;
    cv::cvtColor(src, hsi, cv::COLOR_BGR2HSV);
    
    std::vector<cv::Mat> hsiChannels(3);
    cv::split(hsi, hsiChannels);

    if(equalizeH) cv::equalizeHist(hsiChannels[0], hsiChannels[0]);
    if(equalizeS) cv::equalizeHist(hsiChannels[1], hsiChannels[1]);
    if(equalizeI) cv::equalizeHist(hsiChannels[2], hsiChannels[2]);

    cv::merge(hsiChannels, hsi);
    
    // Convert Back to Original Color Space
    try {
        cv::cvtColor(hsi, tgt, cv::COLOR_HSV2BGR);
    } catch(cv::Exception& e) {
        std::cerr << "OpenCV exception: " << e.what() << std::endl;
        std::cerr << "Error while converting from HSV to BGR." << std::endl;
        return;
    }
    cv::imwrite("Equalized_HSI_infunction.jpg", tgt);
}

/*-----------------------------------------------------------------------**/

void utility::colorAugmentedImages(Mat &src, Rect &ROI_rect)
{
    // Extract ROI from the input image
    cv::Mat ROI = src(ROI_rect);
    
    // a. Rotate original ROI
    imwrite("Original_Color_Roi.jpg", ROI);
    for(int angle = 1; angle <= 3; angle++) {
        Mat rotated;
        rotate(ROI, rotated, angle-1);
        imwrite("Rotated_Color_ROI_" + std::to_string(angle*90) + ".jpg", rotated);
    }

    // b. perform histogram equalization on I component
    Mat equalizedIROI;
    utility::equalizeHSI(ROI, equalizedIROI, false, false, true);
    imwrite("Equalized_I_Roi.jpg", equalizedIROI);
    for(int angle = 1; angle <= 3; angle++)
    {
        Mat rotated;
        rotate(equalizedIROI, rotated, angle-1);
        imwrite("Equalized_I_Rotated_ROI_" + std::to_string(angle*90) + ".jpg", rotated);
    }

    // c. perform histogram equalization on S component
    Mat equalizedSROI;
    utility::equalizeHSI(ROI, equalizedSROI, false, true, false);  // Corrected parameters
    imwrite("Equalized_S_ROI.jpg", equalizedSROI);
    for(int angle = 1; angle <= 3; angle++)
    {
        Mat rotated;
        rotate(equalizedSROI, rotated, angle-1);
        imwrite("Equalized_S_Rotated_ROI_" + std::to_string(angle*90) + ".jpg", rotated);
    }
}