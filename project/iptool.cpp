#include "../iptools/core.h"
#include "../iptools/utility/utility.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

#define MAXLEN 256

int main(int argc, char** argv) {
    FILE *fp;
    char str[MAXLEN];
    char infile[MAXLEN];
    char outfile[MAXLEN];
    char *pch;
    
    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Can't open file: %s\n", argv[1]);
        exit(1);
    }

    while (fgets(str, MAXLEN, fp) != NULL) {
        if (strncmp(str, "#", 1) == 0) continue;

        pch = strtok(str, " ");
        strcpy(infile, pch);

        pch = strtok(NULL, " ");
        strcpy(outfile, pch);

        pch = strtok(NULL, " ");
        bool useOpenCV = strncmp(pch, "opencv", 6) == 0;

        Mat I, I2;
        image src, tgt;
        if (useOpenCV) {
            I = imread(infile);
            if (I.empty()) {
                cout << "Could not open or find the image: " << infile << endl;
                return -1;
            }
            I2 = I.clone(); 
        } else {
            src.read(infile);
            tgt = src; 
        }

        // Apply global functions if specified
        pch = strtok(NULL, " ");
        if (useOpenCV) {
            // Apply OpenCV global functions
            if (pch != NULL && strncmp(pch, "gray", 4) == 0) {
                utility::cv_gray(I, I2);
            } else if (pch != NULL && strncmp(pch, "blur_avg", 8) == 0) {
                int windowSize = atoi(strtok(NULL, " "));
                utility::cv_avgblur(I, I2, windowSize);
            }
        } else {
            // Apply non-OpenCV global functions
            if (pch != NULL && strncmp(pch, "add", 3) == 0) {
                pch = strtok(NULL, " ");
                utility::addGrey(src, tgt, atoi(pch));
            } else if (pch != NULL && strncmp(pch, "binarize", 8) == 0) {
                pch = strtok(NULL, " ");
                utility::binarize(src, tgt, atoi(pch));
            } else if (pch != NULL && strncmp(pch, "scale", 5) == 0) {
                pch = strtok(NULL, " ");
                utility::scale(src, tgt, atof(pch));
            }
        }

        // Parse the number of ROIs
        pch = strtok(NULL, " ");
        int numROIs = atoi(pch);
        vector<Rect> rois;
        vector<string> functions;
        vector<vector<int>> params;

        for (int i = 0; i < numROIs; ++i) {
            // Parse ROI parameters
            int x = atoi(strtok(NULL, " "));
            int y = atoi(strtok(NULL, " "));
            int width = atoi(strtok(NULL, " "));
            int height = atoi(strtok(NULL, " "));
            rois.push_back(Rect(x, y, width, height));

            // Parse the function name
            pch = strtok(NULL, " ");
            functions.push_back(string(pch));

            // Parse the function parameters
            vector<int> funcParams;
            while ((pch = strtok(NULL, " ")) && isdigit(pch[0])) {
                funcParams.push_back(atoi(pch));
            }
            params.push_back(funcParams);
        }

        // Apply ROI-specific functions
        if (useOpenCV) {
            utility::applyCvFunctionsToROIs(I2, I2, rois, functions, params);
            imwrite(outfile, I2);
        } else {
            utility::applyFunctionsToROIs(src, tgt, rois, functions, params);
            tgt.save(outfile);
        }
    }

    fclose(fp);
    return 0;
}
