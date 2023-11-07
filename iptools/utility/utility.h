#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <math.h>
#include <string>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int value);
		static void binarize(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		static void cv_gray(cv::Mat &src, cv::Mat &tgt);
		static void cv_avgblur(cv::Mat &src, cv::Mat &tgt, int WindowSize);
		static void histogramStretching(image &src, image &tgt, int A, int B);
		static void histogramStretch(cv::Mat& src,cv::Mat &tgt, int A, int B);
		static void histogramEqualization(cv::Mat &src, cv::Mat &tgt);
		static void equalizeDarkPixels(cv::Mat &src, cv::Mat &tgt, int threshold);
		static void greyAugmentedImages(cv::Mat &src, cv::Rect &ROI_rect, int A, int B);
		static void equalizeRGB(cv::Mat& src, cv::Mat& tgt, const std::string& channels);
		static void equalizeHSI(cv::Mat& src, cv::Mat& tgt, bool equalizeH, bool equalizeS, bool equalizeI);
		static void colorAugmentedImages(cv::Mat &src, cv::Rect &ROI_rect);
};

#endif