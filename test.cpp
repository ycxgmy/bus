#include <opencv2/opencv.hpp>
using namespace cv;
#include "protocal.h"
#include <iostream>
using namespace std;
int main(int argc, char *argv[]) {
  Session *ss = new Session("67890"
			    ,"四川省"
			    ,"绵阳市"
			    ,"高新区"
			    ,"801"
			    ,"王王"
			    );

  for (int i = 0; i < 10; ++i) {
    Mat x = Mat::zeros(100,100, CV_8UC1);
    putText(x, std::to_string(getTickCount()),Point(0,50), 1, 1, Scalar(255,255,255));
    imwrite(to_string(i)+"tmp.jpg", x);
    ss->alarm_face(to_string(i)+"tmp.jpg", 0.9);
  }
  delete ss;
  return 0;
}
