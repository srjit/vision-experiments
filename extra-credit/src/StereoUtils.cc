#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



using namespace cv;


std::vector<int> getMaxCorrespondenceFromLine(int rightX,
					      int rightY,
					      std::vector<std::vector<int>> leftPoints,
					      cv::Mat leftImage,
					      cv::Mat rightImage){

  int x = rightX - 9 + 20;
  int y = rightY - 9 + 20;

  cv::Rect patchRegion(x, y,5, 5);
  cv::Mat rightPatch  = rightImage(patchRegion);
  cv::Mat rightPatchNorm = normalize(rightPatch);

  std::vector<float> correlations;

  for(int pointIndex=0; pointIndex < leftPoints.size(); pointIndex++){

    int leftImgXTmp = leftPoints[pointIndex][0];
    int leftImgYTmp = leftPoints[pointIndex][1];

    int xPatchLeftBegin = leftImgXTmp - 9 + 20;
    int yPatchLeftBegin = leftImgYTmp - 9 + 20;

  // std::cout<<xPatchLeftBegin<<"\t"<<yPatchLeftBegin<<"\n";
  // std::cout<<"===================================\n";

    

    cv::Rect patchRegion(xPatchLeftBegin, yPatchLeftBegin, 5, 5);
    cv::Mat leftPatch = leftImage(patchRegion);
    cv::Mat leftPatchNorm = normalize(leftPatch);
	  
    cv::Scalar _correlation = cv::sum(rightPatchNorm.mul(leftPatchNorm));
    float correlation = _correlation[0];
    correlations.push_back(correlation);
	
   }

  int maxIndex = std::distance(correlations.begin(),
  			       std::max_element(correlations.begin(),
  						correlations.end()));

  return leftPoints[0];
}


std::vector<int> getCorrespondingPoint(int x_right,
				       int y_right,
				       int height,
				       int width,
				       cv::Mat fundamentalMatrix,
				       cv::Mat leftImage,
				       cv::Mat rightImage){

  float f11 = fundamentalMatrix.at<float>(Point(0,0));
  float f12 = fundamentalMatrix.at<float>(Point(1,0));
  float f13 = fundamentalMatrix.at<float>(Point(2,0));

  float f21 = fundamentalMatrix.at<float>(Point(0,1));
  float f22 = fundamentalMatrix.at<float>(Point(1,1));
  float f23 = fundamentalMatrix.at<float>(Point(2,1));

  float f31 = fundamentalMatrix.at<float>(Point(0,2));
  float f32 = fundamentalMatrix.at<float>(Point(1,2));
  float f33 = fundamentalMatrix.at<float>(Point(2,2));    

  // double f11 = fundamentalMatrix.at<double>(Point(0,0));
  // double f12 = fundamentalMatrix.at<double>(Point(0,1));
  // double f13 = fundamentalMatrix.at<double>(Point(0,2));

  // double f21 = fundamentalMatrix.at<double>(Point(1,1));
  // double f22 = fundamentalMatrix.at<double>(Point(1,1));
  // double f23 = fundamentalMatrix.at<double>(Point(1,2));

  // double f31 = fundamentalMatrix.at<double>(Point(2,0));
  // double f32 = fundamentalMatrix.at<double>(Point(2,1));
  // double f33 = fundamentalMatrix.at<double>(Point(2,2));    
  
  /**
   * Co-efficients of the line
   */
  double a = (f11 * x_right) + (f12 * y_right) + f13;
  double b = (f21 * x_right) + (f22 * y_right) + f23;
  double c = (f31 * x_right) + (f32 * y_right) + f33;

  /**
   * get two points on this line
   */
  double x_min = 0;

  std::vector<std::vector<int>> points;

  for(int colIndex=20; colIndex<width; colIndex++){

    std::vector<int> co_ordinates;
      
    int rowIndex = floor((-c - (a * colIndex))/b);

    if((rowIndex >=20)  && (rowIndex < height - 20)){
      co_ordinates.push_back(colIndex);
      co_ordinates.push_back(rowIndex);

      points.push_back(co_ordinates);
    }

  }

  std::vector<int> correspondingPoint;


  if(points.size()>0){
    correspondingPoint =
      getMaxCorrespondenceFromLine(x_right, y_right, points, leftImage, rightImage);
  }

  return correspondingPoint;
}


void getDisparityMap(cv::Mat leftImage,
		     cv::Mat rightImage,
		     cv::Mat fundamentalMatrix){


  // std::vector<cv::Mat> images;
  // images.push_back(leftImage);
  // images.push_back(rightImage);

  // std::vector<cv::Mat> images_with_padding = addPadding(images);

  // std::cout<<"==================";
  // std::cout<<images_with_padding[0].size();
  // std::cout<<"==================";  
  
  /**
   *  For every pixel in the right image, find the pixel in the left image
   *
   */
  // cv::Mat disparityMapX;
  // cv::Mat disparityMapY;

  int width = rightImage.cols;
  int height = rightImage.rows;



  // cv::Mat foo(height, width, CV_8U);
  // cv::Mat fooNorm(height, width, CV_8U);  
  
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){

      vector<int> point = getCorrespondingPoint(i,
      						j,
      						height,
      						width,
      						fundamentalMatrix,
      						leftImage,
      						rightImage);


      int leftX = i;
      int leftY = j;

      int rightX = 0;
      int rightY = 0;
	  
      // if(point.size()>0){

      // 	rightX = point[0];
      // 	rightY = point[1];

      // }

      // int disparityX = abs(rightX - leftX);
      // int disparityY = abs(rightY - leftY);
      
    }

  }


}
