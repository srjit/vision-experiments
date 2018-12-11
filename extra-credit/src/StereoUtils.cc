#include <iostream>
#include <cmath>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;


std::vector<int> getMaxCorrespondenceFromLine(int rightX,
				  int rightY,
				  std::vector<std::vector<int>> leftPoints,
				  cv::Mat leftImage,
				  cv::Mat rightImage){

      int x = rightX - 9 + 20;
      int y = rightY - 9 + 20;

      cv::Rect patchRegion(x, y, 21, 21);
      cv::Mat rightPatch  = rightImage(patchRegion);
      cv::Mat rightPatchNorm = normalize(rightPatch);

      std::vector<float> correlations;

      for(int pointIndex=0; pointIndex <= leftPoints.size(); pointIndex++){

	int leftImgXTmp = leftPoints[pointIndex][0];
	int leftImgYTmp = leftPoints[pointIndex][1];

	int xPatchLeftBegin = leftImgXTmp - 9 + 20;
	int yPatchLeftBegin = leftImgYTmp - 9 + 20;
	

	cv::Rect patchRegion(xPatchLeftBegin, yPatchLeftBegin, 21, 21);
	cv::Mat leftPatch = leftImage(patchRegion);
	cv::Mat leftPatchNorm = normalize(leftPatch);
	  
	cv::Scalar _correlation = cv::sum(rightPatchNorm.mul(leftPatchNorm));
	float correlation = _correlation[0];
	correlations.push_back(correlation);
	
      }

      int maxIndex = std::distance(correlations.begin(),
				   std::max_element(correlations.begin(),
						    correlations.end()));

      return leftPoints[maxIndex];
}

std::vector<int> getCorrespondingPoint(int x,
			   int y,
			   int height,
			   int width,
			   cv::Mat fundamentalMatrix,
			   cv::Mat leftImage,
			   cv::Mat rightImage){

  double f11 = fundamentalMatrix.at<double>(Point(0,0));
  double f12 = fundamentalMatrix.at<double>(Point(1,0));
  double f13 = fundamentalMatrix.at<double>(Point(2,0));

  double f21 = fundamentalMatrix.at<double>(Point(0,1));
  double f22 = fundamentalMatrix.at<double>(Point(1,1));
  double f23 = fundamentalMatrix.at<double>(Point(2,1));

  double f31 = fundamentalMatrix.at<double>(Point(0,2));
  double f32 = fundamentalMatrix.at<double>(Point(1,2));
  double f33 = fundamentalMatrix.at<double>(Point(2,2));    

  /**
   * Co-efficients of the line
   */
  double a = (f11 * x) + (f12 * y) + f13;
  double b = (f21 * x) + (f22 * y) + f23;
  double c = (f31 * x) + (f32 * y) + f33;

  /**
   * get two points on this line
   */
  double x_min = 0;

  std::vector<std::vector<int>> points;

  for(int colIndex=0; colIndex<width; colIndex++){

    std::vector<int> co_ordinates;
      
    int rowIndex = floor((-c - (a * colIndex))/b);

    co_ordinates.push_back(colIndex);
    co_ordinates.push_back(rowIndex);

    points.push_back(co_ordinates);
    
  }


  std::vector<int> correspondingPoint =
    getMaxCorrespondenceFromLine(x, y, points, leftImage, rightImage);
  
  
  return correspondingPoint;
}


void getDisparityMap(cv::Mat leftImage,
		     cv::Mat rightImage,
		     cv::Mat fundamentalMatrix){

 /**
  *  For every pixel in the right image, find the pixel in the left image
  *
  */
  cv::Mat disparityMapX;
  cv::Mat disparityMapY;

  int width = rightImage.cols;
  int height = rightImage.rows;

  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){

      vector<int> point = getCorrespondingPoint(i,
						j,
						height,
						width,
						fundamentalMatrix,
						leftImage,
						rightImage);


      int leftX = height;
      int leftY = width;

      int rightX = point[0];
      int rightY = point[1];
    }
  }
  

}


