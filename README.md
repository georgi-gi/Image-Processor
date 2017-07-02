# Image-Processor

Image Processor is a C++ application, developed during the OOP course in Sofia University. 
What it does is take as input a path to a file in the command line and according to the chosen
options, it outputs the converted image in the same directory. It supports PPM, PGM and PBM files
and can convert images from those types to grayscale and monochrome versions and to create their 
red, green or blue histograms.

The histogram is an image with 100 pixels in height and 255 pixels in width.
For an X histogram (red, green or blue) on column Y the height of the column is Z when
Z% of the pixels in the image have value Y for the X component.

sample input:
prog.exe --grayscale image01.ppm
prog.exe --histogram=red --histogram=green image02.ppm