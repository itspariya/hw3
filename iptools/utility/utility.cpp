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
// Apply functions to up to 3 ROIs with different functions
void utility::applyFunctionsToROIs(image &src, image &tgt, 
                                   const std::vector<cv::Rect>& rois, 
                                   const std::vector<std::string>& functions, 
                                   const std::vector<std::vector<int>>& params)
{
    // Check if the number of ROIs and functions match
    if (rois.size() != functions.size() || rois.size() != params.size()) {
        throw std::invalid_argument("Mismatch in the number of ROIs, functions, and parameters.");
    }

    tgt = src; // Start with a copy of the source image

    for (size_t i = 0; i < rois.size(); ++i)
    {
        // Check if the ROI is within the image bounds
        if (rois[i].x < 0 || rois[i].y < 0 ||
            rois[i].x + rois[i].width > src.getNumberOfColumns() ||
            rois[i].y + rois[i].height > src.getNumberOfRows()) {
            throw std::out_of_range("ROI is out of image bounds.");
        }

        // Ensure we have valid functions and parameters for each ROI
        if (functions[i] == "add")
        {
            // Check if the parameters list for the 'add' operation is valid
            if (params[i].size() != 1) {
                throw std::invalid_argument("Invalid number of parameters for 'add' operation.");
            }
            int value = params[i][0]; // Get the value from the parameters
            // Apply addGrey to the ROI
            for (int y = rois[i].y; y < rois[i].y + rois[i].height; ++y) {
                for (int x = rois[i].x; x < rois[i].x + rois[i].width; ++x) {
                    int pixelValue = src.getPixel(y, x) + value;
                    tgt.setPixel(y, x, checkValue(pixelValue));
                }
            }
        }
        else if (functions[i] == "binarize")
        {
            // Check if the parameters list for the 'binarize' operation is valid
            if (params[i].size() != 1) {
                throw std::invalid_argument("Invalid number of parameters for 'binarize' operation.");
            }
            int threshold = params[i][0]; // Get the threshold from the parameters
            // Apply binarize to the ROI
            for (int y = rois[i].y; y < rois[i].y + rois[i].height; ++y) {
                for (int x = rois[i].x; x < rois[i].x + rois[i].width; ++x) {
                    int pixelValue = src.getPixel(y, x) < threshold ? MINRGB : MAXRGB;
                    tgt.setPixel(y, x, pixelValue);
                }
            }
        }
        else {
            throw std::invalid_argument("Unknown function requested.");
        }
    }
}
/*-----------------------------------------------------------------------**/
// Apply functions to up to 3 ROIs with different functions for OpenCV Mat
void utility::applyCvFunctionsToROIs(Mat &src, Mat &tgt, 
                                     const std::vector<cv::Rect>& rois, 
                                     const std::vector<std::string>& functions, 
                                     const std::vector<std::vector<int>>& params)
{
    // Check if the number of ROIs and functions match
    if (rois.size() != functions.size() || rois.size() != params.size()) {
        throw std::invalid_argument("Mismatch in the number of ROIs, functions, and parameters.");
    }

    tgt = src.clone(); // Start with a copy of the source Mat

    for (size_t i = 0; i < rois.size(); ++i)
    {
        // Check if the ROI is within the Mat bounds
        if (rois[i].x < 0 || rois[i].y < 0 ||
            rois[i].x + rois[i].width > src.cols ||
            rois[i].y + rois[i].height > src.rows) {
            throw std::out_of_range("ROI is out of Mat bounds.");
        }

        // Define the ROI on the target Mat
        Mat roi = tgt(rois[i]);

        // Ensure we have valid functions and parameters for each ROI
        if (functions[i] == "cv_gray")
        {
            // Convert the specified ROI to grayscale
            cv_gray(roi, roi);
        }
        else if (functions[i] == "cv_avgblur")
        {
            // Check if the parameters list for the 'cv_avgblur' operation is valid
            if (params[i].size() != 1) {
                throw std::invalid_argument("Invalid number of parameters for 'cv_avgblur' operation.");
            }
            int windowSize = params[i][0]; // Get the window size from the parameters
            cv_avgblur(roi, roi, windowSize);
        }
        // Add more OpenCV functions as needed
        else {
            throw std::invalid_argument("Unknown OpenCV function requested.");
        }

        // Place the processed ROI back into the target Mat
        roi.copyTo(tgt(rois[i]));
    }
}
