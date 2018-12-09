#include "Config.cc"
#include "Preprocess.cc"
#include "Corners.cc"
#include "Visualization.cc"
#include "Utils.cc"

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


  Mat im1 = gray_images[0];
  Mat im2 = gray_images[1];
  imageCorrespondences(im1, im2, mappings, corners);


  /**
   *   Calculation of the fundemental Matrix
   *   Construct the matrix 
   *
   *
   */
  std::cout<<mappings.size();

  cv::Mat A = constructPointsMatrix(mappings);
  cv::Mat W, U, Vt;
  cv::SVD::compute(A, W, U, Vt)	;
  std::cout<<"Vt on the SVD of A (before RANSAC on correspoinding points (Last column of Vt)) "<<Vt;

  // Fundemental Matrix  computation using ransac
  //https://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html?#findhomography
  cv::Mat fundamental_matrix = get_fundemental_matrix_after_ransac(mappings);
  std::cout<<"Fundemental Matrix after RANSAC on correspoinding points "<<fundamental_matrix<<"\n";
  

  /**
   *  Dense Disparity Map
   *  https://sourishghosh.com/2016/dense-disparity-maps-orb-descriptors/
   *
   *  https://github.com/sourishg/disparity-map/blob/master/epipolar.cpp
   */
  

}
