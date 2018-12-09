#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

cv::Mat constructPointsMatrix(std::vector<PatchPair> mappings){


  /**
   *  Constrcut the m * 9 matrix A
   *
   */
  std::vector<std::vector<float>> _A;
    
  for(int i=0; i<mappings.size(); i++){


    std::vector<float> row;

    PatchPair pair = mappings[i];

    int x1 = pair.coordinate1[0];
    int y1 = pair.coordinate1[1];

    int x1_prime = pair.coordinate2[0];
    int y1_prime = pair.coordinate2[1];

    row.push_back(x1 * x1_prime);
    row.push_back(x1 * y1_prime);
    row.push_back(x1);
    row.push_back(y1 * x1_prime);
    row.push_back(y1 * y1_prime);
    row.push_back(y1);
    row.push_back(x1_prime);
    row.push_back(y1_prime);
    row.push_back(1);

    _A.push_back(row);
  }


  cv::Mat A(_A.size(), _A.at(0).size(), CV_32FC1);

  for(int i=0; i< A.rows; i++){
    for(int j=0; j< A.cols; j++){
       A.at<float>(i, j) = _A[i][j];
    }
  }


  return A;
  
  
}



cv::Mat get_fundemental_matrix_after_ransac(std::vector<PatchPair> mappings){

  int point_count = mappings.size();

  vector<Point2f> points1(point_count);
  vector<Point2f> points2(point_count);

  for( int i = 0; i < point_count; i++ ){

    PatchPair pair = mappings[i];    

    int x1 = pair.coordinate1[0];
    int y1 = pair.coordinate1[1];

    int x1_prime = pair.coordinate2[0];
    int y1_prime = pair.coordinate2[1];    

    cv::Point2f pt1(x1, y1);
    cv::Point2f pt2(x1_prime, y1_prime);

    points1.push_back(pt1);
    points2.push_back(pt2);

  }

  cv::Mat fundamental_matrix = findFundamentalMat(points1, points2, FM_RANSAC);
  return fundamental_matrix;
    

}
