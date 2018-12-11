#include "Config.cc"
#include "Preprocess.cc"
#include "Corners.cc"
#include "Visualization.cc"
#include "Utils.cc"
#include "Epilines.cc"
#include "StereoUtils.cc"

#include <algorithm>
#include <random>
#include <eigen3/Eigen/SVD>
#include <eigen3/Eigen/Dense>

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/highgui.hpp>



using namespace std;
using namespace cv;


int main(){


  std::map<std::string,std::string> config = loadConfig("./src/config.cfg");
  std::vector<std::string> image_filenames = get_files("./data/");
  std::sort(image_filenames.begin(), image_filenames.end());

  // for(int i=0;i<image_filenames.size();i++){
  //   std::cout<<image_filenames[i]<<"\t";
  // }
  // std::cout<<"\n";

  /**
   * (a-i) Reading and Scaling
   */
  std::vector<cv::Mat> color_images = get_color_images(image_filenames, 1 , 1);
  std::vector<cv::Mat> gray_images = convertAllToGrayscale(color_images);
  std::vector<cv::Mat> padded_images = addPadding(gray_images);


  /**
   * (a-ii) Sparse corner coordinates
   */

  int threshold = 60;
  int cornerCount = 100;

  std::vector<std::vector<std::tuple<int, int, int>>> corners = getCornersInImageArray(gray_images,
  										       threshold,
  										       cornerCount);

  std::vector<std::vector<Template>> patches = createPatches(padded_images,
  							     corners);
  std::vector<PatchPair> patch_correlations_sorted = comparePatches(patches[0],
  								    patches[1]);


  /**
   * (a-iii) Find corner correspondences
   */

  std::vector<PatchPair> mappings = removeDuplicateCorrespondences(patch_correlations_sorted);

  // Prune correspondences based on geometric constraints
  for (int i = mappings.size() - 1; i >= 0; i--){
    if (abs(mappings.at(i).coordinate1[0] - mappings.at(i).coordinate2[0]) > 30 ||
  	abs(mappings.at(i).coordinate1[1] - mappings.at(i).coordinate2[1]) > 250){
      mappings.erase( mappings.begin() + i );
    }
  }


  // castle left
  Mat im1 = gray_images[0];
  // castle right
  Mat im2 = gray_images[1];
  imageCorrespondences(im1, im2, mappings, corners);


  /**
   *   Calculation of the fundemental Matrix
   *   Construct the matrix 
   *
   */

  cv::Mat A = constructPointsMatrix(mappings);
  cv::Mat W, U, Vt;
  cv::SVD::compute(A, W, U, Vt)	;
  cv::Mat fundementalMatrix = get_fundemental_matrix_after_ransac(mappings);
  std::cout<<"Fundemental Matrix after RANSAC on correspoinding points \n "
	   <<fundementalMatrix<<"\n";



  // find the epipolar lines for each pixel

  getDisparityMap(im1, im2, fundementalMatrix);


  // for every pixel in image 1, find the location in the second image
  
  
  

  

  

  /**
   *  Dense Disparity Map
   *  https://sourishghosh.com/2016/dense-disparity-maps-orb-descriptors/
   *
   *  https://github.com/sourishg/disparity-map/blob/master/epipolar.cpp
   */

  /**
   *  Find the pixels along a line
   *
   *   For each pixel in first image
   *      - Find the epipolar line in the right image
   *      - Examine the epipolar line to determine the best match
   *      - Triangulate the matches to get depth information
   */

  std::vector<cv::Point> points1  = getCorrespondancePointsFromImage(mappings, 1);
  std::vector<cv::Point> points2  = getCorrespondancePointsFromImage(mappings, 2);
  int indexOfImage = 2;
  cv::Mat epipolarLines;

  //computeCorrespondEpilines(points2, indexOfImage, fundementalMatrix, epipolarLines);

  std::cout<<"\n\n";
  std::cout<<epipolarLines;
  std::cout<<"\n\n";


  // plot the epipolar lines
  

  

  // //  cv::Matx33f m((float*)fundementalMatrix.ptr());


  // // drawing epipolar lines
  // cv::Mat H1;
  // cv::Mat H2;

  // cv::Size imgSize(im1.cols, im1.rows);
  // stereoRectifyUncalibrated(points1, points2, fundementalMatrix, imgSize, H1, H2);

  // // TODO- http://answers.opencv.org/question/90742/opencv-depth-map-from-uncalibrated-stereo-system/
  // std::cout<<"\n";
  // std::cout<<fundementalMatrix;
  std::cout<<"\n";

  // http://www.hasper.info/opencv-draw-epipolar-lines/
  // drawEpipolarLines("Test", fundementalMatrix, im1, im2, points1, points2);
  
  

}
