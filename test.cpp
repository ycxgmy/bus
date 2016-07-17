#include "session.h"
#include <iostream>
using namespace std;
#include <opencv2/opencv.hpp>
using cv::Mat;
char * mat2jpgbuf(const cv::Mat &m, int &buflen) {
  vector<uchar> buf;
  cv::imencode(".jpg", m, buf, std::vector<int>());
  buflen = buf.size();
  char *p = new char[buflen];
  memcpy(p, &buf[0], buflen);
  return p;
}
int main(int argc, char *argv[]) {
  Session *ss = new Session("67890"
			    ,"四川省"
			    ,"绵阳市"
			    ,"高新区"
			    ,"801"
			    ,"王王"
			    );

  for (int i = 0; i < 1; ++i) {
    Mat x = Mat::zeros(100,100, CV_8UC1);
	putText(x, std::to_string(cv::getTickCount()), cv::Point(0, 50), 1, 1, cv::Scalar(255, 255, 255));

    int buflen;  char *pp = mat2jpgbuf(x, buflen);
    ss->alarm_face(pp, buflen, 0.9);
  }
  delete ss;
  return 0;
}
