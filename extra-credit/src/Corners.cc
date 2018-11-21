#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <eigen3/Eigen/SVD>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

struct Template{
    int x;
    int y;
    cv::Mat patch;
    cv::Mat normalized_patch;
};


struct PatchPair{

    cv::Mat normalized_patch1;
    cv::Mat normalized_patch2;

    int coordinate1[2];
    int coordinate2[2];

    float correlation;
};


struct h_inliers{

    Eigen::MatrixXd h;
    int inliers;
    std::vector<int> in_indices;

};

bool sortRANSAC(h_inliers h1, h_inliers h2)
{
    return (h1.inliers > h2.inliers);
}



Mat nonMaxSuppress(Mat image, int windowSize)
{
    image.convertTo(image, CV_32S);
    int r = image.rows;
    int c = image.cols;
    int pad = floor(windowSize/2);

    Mat padded;
    copyMakeBorder(image, padded, pad, pad, pad, pad, BORDER_REPLICATE);
    Mat maxSupress = Mat::zeros(r, c, CV_32S);
    for (int i=pad; i<pad+r; ++i){
        for (int j=pad; j<pad+c; ++j){
            double min, max;
            minMaxLoc(padded(Range(i-pad, i+pad+1),Range(j-pad, j+pad+1)), &min, &max);
            if (padded.at<int>(i,j) < max){
                //cout << max << '\t' << padded.at<int>(i,j) << endl;
                padded.at<int>(i,j) = 0;

                //maxSupress.at<int>(i-pad,j-pad) = padded.at<int>(i,j);
                //cout << padded.at<int>(i,j) << endl;
            }
        }
    }
    //return maxSupress;
    return padded(Range(pad,pad+r),Range(pad,pad+c));
}


bool sortinrev(const std::tuple<int,int,int> &a, const std::tuple<int,int,int> &b)
{
  return (std::get<0>(a) > std::get<0>(b));
}



std::vector<std::tuple<int, int, int>> getCornerCordsAndMag(cv::Mat image, int thresh, int cornerCount)
{
    /*
      Implementation of Corner Detection Algorithm

     */

    image.convertTo(image, CV_32S);
    int r = image.rows;
    int c = image.cols;
    std::vector<std::tuple<int, int, int>> cornerCordsAndMag;

    int i,j;
    for (i=0; i<r; ++i){
        for (j=0; j<c; ++j){
            if (image.at<int>(i,j) >= thresh){
	      std::tuple<int, int, int> cordsAndMag = std::make_tuple(image.at<int>(i,j),
								      i,
								      j);
              cornerCordsAndMag.push_back(cordsAndMag);
            }
        }
    }
    sort(cornerCordsAndMag.begin(), cornerCordsAndMag.end(), sortinrev);
    std::vector<std::tuple<int,int,int>> out(&cornerCordsAndMag[0], &cornerCordsAndMag[cornerCount]);
    return out;
}


std::vector<std::vector<std::tuple<int, int, int>>> getCornersInImageArray(
								 std::vector<cv::Mat> images,
								 int threshold,
								 int cornerCount){

  std::vector<std::vector<std::tuple<int, int, int>>> corners;

  int blockSize = 2;
  int aperatureSize = 3;
  double k = 0.04;
  int winsize = 11;

  for (std::vector<cv::Mat>::iterator image =  images.begin();
       image != images.end(); ++image){

    // Create placeholder and apply harris corner detector
    Mat dst = Mat::zeros((*image).size(), CV_32FC1);
    cornerHarris(*image, dst, blockSize, aperatureSize, k, BORDER_DEFAULT);

    // Normalize
    normalize(dst, dst, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst, dst);
    dst.convertTo(dst, CV_8U);

    // Apply non-max suppresion
    Mat cornerImg = nonMaxSuppress(dst, winsize);

    std::vector<std::tuple<int, int, int>> cornerCordsAndMag =
	  getCornerCordsAndMag(cornerImg, threshold, cornerCount);

    corners.push_back(cornerCordsAndMag);

  }

  return corners;
}


/**
 *  Build the templates and store information about it in the structure called Template
 *   defined in this file.
 *
 *   Return: A vector with two elements
 *   First elemnt - Vector of templates from the first image
 *   Second elemnt -Vector of templates from the second image
 */
std::vector<std::vector<Template>> createPatches(std::vector<cv::Mat> images,
				   std::vector<std::vector<std::tuple<int, int, int>>> corners){

  std::vector<std::vector<Template>> patches;

  for(int i=0; i<images.size(); i++){

    std::vector<Template> patchesFromImage;
    cv::Mat image = images[i];

    std::vector<std::tuple<int, int, int>> cornersOfImage = corners[i];

    for(int j=0; j< cornersOfImage.size(); j++){

      std::tuple<int, int, int> pt = cornersOfImage[j];

      int _y = std::get<1>(pt);
      int _x = std::get<2>(pt);

      // +20 for the padding
      // was minus 2 with patch size 5
      int y = _y - 9 + 20;
      int x = _x - 9 + 20;

      //cout<<y <<"\t"<<x<<"\n";

      cv::Rect patchRegion(x, y, 21, 21);
      cv::Mat patch  = image(patchRegion);

      Template t;
      t.x = _x;
      t.y = _y;
      t.patch = patch;


      patchesFromImage.push_back(t);

    }

    patches.push_back(patchesFromImage);
  }

  return patches;

}


int intensity(cv::Mat image, int i, int j){

  Scalar intensity  = image.at<uchar>(i,j);
  int pixel_value = intensity.val[0];
  return pixel_value;
}


cv::Mat normalize(cv::Mat patch){
    patch.convertTo(patch, CV_64F);
    Mat normedPatch = patch / sqrt(cv::sum(patch.mul(patch))[0]);

  return normedPatch;
}



bool acompare(PatchPair lhs, PatchPair rhs) { return lhs.correlation > rhs.correlation; }



/**
 *
 *  Compare the patches from both images and return a PatchPair for every combination.
 *
 *   Returns a vector PatchPair struct - defined in this file
 *
 *   Patchpair contains co-ordinates of every patch used in the combination
 *   and the strength of the correlation of the pair.
 *
 */
std::vector<PatchPair> comparePatches(std::vector<Template> patches_first_image,
				      std::vector<Template> patches_second_image){

  /*
    Add normalized patches to the structure
  */
  for(int i=0;i<patches_first_image.size();i++){
    patches_first_image[i].normalized_patch = normalize(patches_first_image[i].patch);
  }


  for(int i=0;i<patches_second_image.size();i++){
    patches_second_image[i].normalized_patch = normalize(patches_second_image[i].patch);
  }


  /*
    Multiply the corresponding elements for each combination
  */
  std::vector<PatchPair> correlations;


  for(int i=0;i< patches_first_image.size();i++){
    for(int j=0;j<  patches_second_image.size();j++){

      cv::Mat mult = (patches_first_image[i].normalized_patch).
	mul(patches_second_image[j].normalized_patch);

      cv::Scalar _correlation = cv::sum(mult);

      PatchPair patchpair;
      patchpair.normalized_patch1 = patches_first_image[i].normalized_patch;
      patchpair.normalized_patch2 = patches_second_image[j].normalized_patch;

      patchpair.coordinate1[1] = patches_first_image[i].x;
      patchpair.coordinate1[0] = patches_first_image[i].y;
      patchpair.coordinate2[1] = patches_second_image[j].x;
      patchpair.coordinate2[0] = patches_second_image[j].y;

      patchpair.correlation = _correlation[0];

      correlations.push_back(patchpair);
    }
  }

  std::sort(correlations.begin(), correlations.end(), acompare);
  return correlations;

}



std::vector<PatchPair> removeDuplicateCorrespondences(std::vector<PatchPair> pairs){

  std::vector<PatchPair> non_duplicate_correspondences;
  std::vector<std::vector<int>> mapped_coordinates_image1;
  std::vector<std::vector<int>> mapped_coordinates_image2;

  for(int i=0; i<pairs.size(); i++){

    std::vector<int> co_ord1;
    co_ord1.push_back(pairs[i].coordinate1[0]);
    co_ord1.push_back(pairs[i].coordinate1[1]);

    std::vector<int> co_ord2;
    co_ord2.push_back(pairs[i].coordinate2[0]);
    co_ord2.push_back(pairs[i].coordinate2[1]);

    bool mapped = false;

    if(std::find(mapped_coordinates_image1.begin(),
		 mapped_coordinates_image1.end(), co_ord1) !=
       mapped_coordinates_image1.end()){
      mapped = true;
    }

    if(std::find(mapped_coordinates_image2.begin(),
		 mapped_coordinates_image2.end(), co_ord2) !=
       mapped_coordinates_image2.end()){
      mapped = true;
    }

    if (! mapped){
      non_duplicate_correspondences.push_back(pairs[i]);
      mapped_coordinates_image1.push_back(co_ord1);
      mapped_coordinates_image2.push_back(co_ord2);
    }

  }

  return non_duplicate_correspondences;
}
