#include "../iptools/core.h"
#include <string.h>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char infile[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) {
		if (strncmp(str,"#",1)==0) continue;
		int NumParameters = 0;
		
		pch = strtok(str, " ");
		strcpy(infile, pch);

		pch = strtok(NULL, " ");
		strcpy(outfile, pch);

		pch = strtok(NULL, " ");
		if (strncmp(pch,"opencv",6)==0) {
			cv::Mat I = cv::imread(infile);
			cv::Mat I2;
			
			if( I.empty()) {
				cout << "Could not open or find the image: " << infile << endl;
				return -1;
			}

			//cv::namedWindow("Grayscale Image", cv::WINDOW_AUTOSIZE);
			
			pch = strtok(NULL, " ");
			if (strncmp(pch,"gray",4)==0) {
				utility::cv_gray(I, I2);
			}
			else if (strncmp(pch,"blur_avg",8)==0) {
				pch = strtok(NULL, " ");
				utility::cv_avgblur(I, I2, atoi(pch));
			}
			else if (strncmp(pch,"equalize",8)==0) {
				
    			utility::histogramEqualization(I, I2);
			}

			else if (strncmp(pch,"equalize_dark",13)==0) {
				pch = strtok(NULL, " ");
				int threshold = atoi(pch);
				utility::equalizeDarkPixels(I, I2, threshold);
			}
			
			else if (strncmp(pch,"stretch",17)==0) {
				pch = strtok(NULL, " ");
				int A = atoi(pch);
				pch = strtok(NULL, " ");
				int B = atoi(pch);
				utility::histogramStretch(I, I2, A, B);
			}
			else if (strncmp(pch, "grey_augment", 12) == 0) {
				pch = strtok(NULL, " ");
				int x = atoi(pch);
				pch = strtok(NULL, " ");
				int y = atoi(pch);
				pch = strtok(NULL, " ");
				int width = atoi(pch);
				pch = strtok(NULL, " ");
				int height = atoi(pch);
				pch = strtok(NULL, " ");
				int A = atoi(pch);
				pch = strtok(NULL, " ");
				int B = atoi(pch);
				
				cv::Rect ROI_rect(x, y, width, height);
				utility::greyAugmentedImages(I, ROI_rect, A, B);
			}
			else if (strncmp(pch, "equalize_rgb", 12) == 0) {
				pch = strtok(NULL, " ");
				std::string channels(pch);
				utility::equalizeRGB(I, I2, channels);
			}
			else if (strncmp(pch, "equalize_hsi", 12) == 0) {
				pch = strtok(NULL, " ");
				bool equalizeH = atoi(pch);
				pch = strtok(NULL, " ");
				bool equalizeS = atoi(pch);
				pch = strtok(NULL, " ");
				bool equalizeI = atoi(pch);
				utility::equalizeHSI(I, I2, equalizeH, equalizeS, equalizeI);
			}
			else if (strncmp(pch, "color_augment", 12) == 0) {
				pch = strtok(NULL, " ");
				int x = atoi(pch);
				pch = strtok(NULL, " ");
				int y = atoi(pch);
				pch = strtok(NULL, " ");
				int width = atoi(pch);
				pch = strtok(NULL, " ");
				int height = atoi(pch);
				
				cv::Rect ROI_rect(x, y, width, height);
				utility::colorAugmentedImages(I, ROI_rect);
			}
			
			else {
				printf("No function: %s\n", pch);
				continue;
			}
			
			cv::imwrite(outfile, I2);
		}
		else {
			src.read(infile);
			if (strncmp(pch,"add",3)==0) {
				pch = strtok(NULL, " ");
				utility::addGrey(src,tgt,atoi(pch));
			}

			else if (strncmp(pch,"binarize",8)==0) {
				pch = strtok(NULL, " ");
				utility::binarize(src,tgt,atoi(pch));
			}

			else if (strncmp(pch,"scale",5)==0) {
				pch = strtok(NULL, " ");
				utility::scale(src,tgt,atof(pch));
			}

			else {
				printf("No function: %s\n", pch);
				continue;
			}
			
			tgt.save(outfile);
		}
	}
	fclose(fp);
	return 0;
}