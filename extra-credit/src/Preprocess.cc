#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dirent.h>
#include <string>

using namespace cv;

std::vector<std::string> get_files(const char* path){

  /**
   *  Get all the image files from a path
   */

  DIR *dir;
  struct dirent *ent;

  std::vector<std::string> images;

  std::string dirname = path;
  if ((dir = opendir (path)) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      std::string file_name = ent->d_name;
      std::string full_path = dirname + "/" + file_name;

      images.push_back(full_path);
    }
    closedir (dir);
  } else {
    perror ("");
  }
  return images;
}


cv::Mat convertToGrayscale(cv::Mat color_image) {

  /**
   * Convert image to grayscale
   */
    cv::Mat greyMat;
    cv::cvtColor(color_image, greyMat, cv::COLOR_BGR2GRAY);
    return greyMat;

}

std::vector<cv::Mat> convertAllToGrayscale(std::vector<cv::Mat> color_images) {

    /**
     * Convert images to grayscale
     */
    std::vector<cv::Mat> gray_images;
    for (auto image = color_images.begin(); image != color_images.end(); ++image){
        cv::Mat gray;
        cv::cvtColor(*image, gray, cv::COLOR_BGR2GRAY);
        gray.convertTo(gray, CV_8U);
        //gray.convertTo(gray, CV_32S);
        gray_images.push_back(gray);
    }
    return gray_images;

}


std::vector<cv::Mat> addPadding(std::vector<cv::Mat> gray_images) {

    std::vector<cv::Mat> padded_images;
    for (auto image = gray_images.begin(); image != gray_images.end(); ++image){
      Mat padded;
      int pad = 20;
      copyMakeBorder(*image, *image, pad, pad, pad, pad, BORDER_CONSTANT);
      padded_images.push_back(*image);

    }
    return padded_images;
}


// std::vector<cv::Mat> addPadding2(std::vector<cv::Mat> gray_images) {

//     std::vector<cv::Mat> padded_images;
//     for (auto image = gray_images.begin(); image != gray_images.end(); ++image){
//       Mat padded;
//       int pad = 10;
//       copyMakeBorder(*image, *image, pad, pad, pad, pad, BORDER_CONSTANT);
//       padded_images.push_back(*image);

//     }
//     return padded_images;
// }


std::vector<cv::Mat> get_grayscale_images(std::vector<std::string> filenames){

  /**
   * Read the files as grayscale images and return
   * a vector of grayscale matrices that represents them
   *
   */
  std::vector<cv::Mat> grayimages;
  cv::Mat image;

  for(auto file = filenames.begin(); file != filenames.end(); ++file) {
    image = imread(*file);
    if ( image.empty() ){
      continue;
    }

    cv::Mat grayImage = convertToGrayscale(image);
    cv::Mat convertedGrayImage;
    grayImage.convertTo(convertedGrayImage, CV_8U);
    grayimages.push_back(convertedGrayImage);
  }

  return grayimages;

}

std::vector<cv::Mat> get_color_images(std::vector<std::string> filenames,
				      int scaleX, int scaleY){

  /**
   *Read RGB images and store in vector
   *
   */
    std::vector<cv::Mat> colorimages;
    cv::Mat image;

    for(auto file = filenames.begin(); file != filenames.end(); ++file) {
        image = imread(*file);
        if ( image.empty() ){
            continue;
        }
	cv::resize(image, image, cv::Size(), scaleX, scaleY);
        colorimages.push_back(image);
    }

    return colorimages;

}
