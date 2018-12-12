#include "Config.cc"
#include "Preprocess.cc"
#include "Corners.cc"
#include "Visualization.cc"
#include "Utils.cc"
#include "Epilines.cc"
#include "StereoUtils.cc"

#include <algorithm>
#include <random>

#include <iostream>
#include <opencv2/opencv.hpp>
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

  cv::Mat fundMat(3,3, CV_32F);

  fundMat.at<float>(0,0) = Vt.at<float>(0,8);
  fundMat.at<float>(0,1) = Vt.at<float>(1,8);
  fundMat.at<float>(0,2) = Vt.at<float>(2,8);
  fundMat.at<float>(1,0) = Vt.at<float>(3,8);
  fundMat.at<float>(1,1) = Vt.at<float>(4,8);
  fundMat.at<float>(1,2) = Vt.at<float>(5,8);
  fundMat.at<float>(2,0) = Vt.at<float>(6,8);
  fundMat.at<float>(2,1) = Vt.at<float>(7,8);
  fundMat.at<float>(2,2) = Vt.at<float>(8,8);

        
  std::cout<<"foo:"<<fundMat;

  
  
  // cv::Mat fundamentalMatrix = get_fundemental_matrix_after_ransac(mappings);
  // std::cout<<"Fundemental Matrix after RANSAC on correspoinding points \n "
  // 	   <<fundamentalMatrix<<"\n";


  /**
   *  dense Disparity Map
   */

   getDisparityMap(padded_images[0], padded_images[1], fundMat);
  // getDisparityMap(im1, im2, fundMat);  

  // std::vector<cv::Point> points1  = getCorrespondancePointsFromImage(mappings, 1);
  // std::vector<cv::Point> points2  = getCorrespondancePointsFromImage(mappings, 2);

  // cv::Mat H1(4,4, im1.type());
  // cv::Mat H2(4,4, im2.type());

  
  // stereoRectifyUncalibrated(points1, points2, fundamentalMatrix, im1.size(), H1, H2);

  // cv::Mat rectified1(im1.size(), im1.type());
  // cv::warpPerspective(im1, rectified1, H1, im1.size());

  // cv::Mat rectified2(im2.size(), im2.type());
  // cv::warpPerspective(im2, rectified2, H2, im2.size());

  
  
  // // int indexOfImage = 2;
  // // cv::Mat epipolarLines;

  // // computeCorrespondEpilines(points1, indexOfImage, fundementalMatrix, epipolarLines);

  // namedWindow( "Rect left", WINDOW_AUTOSIZE );  
  // imshow("Rect Left", rectified1);
  // waitKey(0);

  

}
