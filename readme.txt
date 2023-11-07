This software is architectured as follows.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image. (without OpenCV)
	utility - This folder hosts the implemented algorithms.
	roi - This folder hosts the roi structure
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make
The executable file will be created in bin/



*** Run the program:
cd bin/
./iptool parameters.txt



*** FUNCTIONS (without OpenCV) ***

1. Add intensity: add
Increase the intensity for a gray-level image.
parameter: intensity

2. Binarization: binarize
Binarize the pixels with the threshold.
parameter: threshold

3. Scaling: Scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.
parameter: scaling_factor




*** FUNCTIONS (with OpenCV) ***

1. Gray Scale: gray
Converts the whole image to gray-level.
Parameters: None


2. Average Blur: blur_avg
Uniform smoothing.
parameter: window_size


2. Average Blur: blur_avg
Uniform smoothing.
Parameters: window_size

3. Histogram Stretching: stretch
Stretches the histogram of the grayscale image
Parameters:
- A (int): The lower bound of the target range
- B (int): The upper bound of the target range

4. Histogram Equalization: equalize
Enhances the contrast of images by equalizing the histogram.
Parameters: None

5. Equalize Dark Pixels: equalize_dark
nhances the contrast of images by equalizing the histogram.
Parameters: threshold (int): The pixel intensity below which pixels are considered dark.

6. Grey Augmented Images: grey_augment
Creates a set of augmented images (rotated and histogram modified) from a grayscale Region of Interest (ROI).
Parameters:
- ROI_rect (Rect): A rectangle defining the ROI.
- A and B (int): Parameters for histogram stretching.

7.Equalize RGB: equalizeRBG
Performs histogram equalization on specified RGB channels of the image
Parameters:
- channels (string): String containing channel indicators ('R', 'G', 'B') for which channels to equalize.

8. Equalize HSI: equalizeHSI
Performs histogram equalization on selected channels (Hue, Saturation, Intensity) of the image in the HSV(OpenCV) color space.
arameters:
equalizeH (bool): Whether to equalize the Hue channel.
equalizeS (bool): Whether to equalize the Saturation channel.
equalizeI (bool): Whether to equalize the Intensity channel.

9. Color Augmented Images: color_augment
Creates a set of augmented images (rotated and histogram modified) from a color Region of Interest (ROI).
Parameters:
- ROI_rect (Rect): A rectangle defining the ROI.

*** PARAMETERS FILE ***
(Please check parameters.txt)
There are four parameters:
1. the input file name
2. the output file name
3. opencv / FUNCTIONS (without OpenCV): skip to 5 for parameters
4. the name of the function with OpenCV. Check FUNCTIONS (with OpenCV).
5. parameters (Check FUNCTIONS (with OpenCV) for parameters)


