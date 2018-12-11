#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

void getCorrespondingPoint(int x, int y, cv::Mat fundamentalMatrix){

  double f11 = fundamentalMatrix.at<double>(Point(0,0));
  double f12 = fundamentalMatrix.at<double>(Point(1,0));
  double f13 = fundamentalMatrix.at<double>(Point(2,0));

  double f21 = fundamentalMatrix.at<double>(Point(0,1));
  double f22 = fundamentalMatrix.at<double>(Point(1,1));
  double f23 = fundamentalMatrix.at<double>(Point(2,1));

  double f31 = fundamentalMatrix.at<double>(Point(0,2));
  double f32 = fundamentalMatrix.at<double>(Point(1,2));
  double f33 = fundamentalMatrix.at<double>(Point(2,2));    

  
  
  
}


void getDisparityMap(cv::Mat leftImage,
		     cv::Mat rightImage,
		     cv::Mat fundamentalMatrix){

 /**
  *  For every pixel in the right image, find the pixel in the left image
  *
  */
  cv::Mat disparityMap;

  int width = rightImage.cols;
  int height = rightImage.rows;

  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      
      
    }
  }
  

}


