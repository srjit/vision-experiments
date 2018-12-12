# include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


void imageCorrespondences(Mat im1,
			  Mat im2,
			  std::vector<PatchPair> mappings,
			  std::vector<std::vector<std::tuple<int, int, int>>> corners){

  // Test visualize correspondences
  Mat combined;
  Mat foo;
  hconcat(im1, im2, combined);
  cvtColor(combined, foo, COLOR_GRAY2BGR);   


  // Draw Lines
  for (int t = 0; t<mappings.size(); ++t){
    int r1 = mappings[t].coordinate1[0];
    int c1 = mappings[t].coordinate1[1];
    int r2 = mappings[t].coordinate2[0];
    int c2 = mappings[t].coordinate2[1];

    int r = 0 + ( std::rand() % ( 255 - 0 + 1 ) );
    int g = 0 + ( std::rand() % ( 255 - 0 + 1 ) );
    int b = 0 + ( std::rand() % ( 255 - 0 + 1 ) );    


    line(foo, Point(c1, r1), Point(c2+im1.cols, r2), CV_RGB(r, g, b), 1, 8);    
  }

  // Draw Corners on Image 1
  for (int t = 0; t < corners[0].size(); ++t){
    int r = get<1>(corners[0][t]);
    int c = get<2>(corners[0][t]);
    circle(foo, Point(c,r), 4.0, Scalar(0), 1, 8);    
  }

  // Draw Corners on Image 2
  for (int t = 0; t < corners[1].size(); ++t){
    int r = get<1>(corners[1][t]);
    int c = get<2>(corners[1][t]) + im1.cols;
    circle(foo, Point(c,r), 4.0, Scalar(0), 1, 8);    
  }

  // Display test frame
  // namedWindow( "Sample Lines", WINDOW_AUTOSIZE );
  // Mat sampleLines = foo;
  //  imshow("Sample Lines", foo);
  //  waitKey(0);

}
