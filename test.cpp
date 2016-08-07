#include "session.h"
#include <iostream>
using namespace std;
#include <opencv2/opencv.hpp>
using cv::Mat;
#include "Uart.h"
#include "Upss.h"
#include "AlarmCheck.h"
char * mat2jpgbuf(const cv::Mat &m, int &buflen) {
  vector<uchar> buf;
  cv::imencode(".jpg", m, buf, std::vector<int>());
  buflen = buf.size();
  char *p = new char[buflen];
  memcpy(p, &buf[0], buflen);
  return p;
}
int main(int argc, char *argv[]) {
	string plateNumber = "67890"; if (argc > 1) plateNumber = argv[1];
	string province = "四川省";   if (argc > 2) province = argv[2];
	string city = "绵阳市";       if (argc > 3) city = argv[3];
	string county = "高新区";     if (argc > 4) county = argv[4];
	string busRoute = "801";      if (argc > 5) busRoute = argv[5];
	string driverName = "王王";   if (argc > 6) driverName = argv[6];
	string username = "test";     if (argc > 7) username = argv[7];
	string password = "123456";   if (argc > 8) password = argv[8];
	string verbose = "1";         if (argc > 9) verbose = argv[9];
	string imagefile = "";        if (argc > 10) imagefile = argv[10];
  Session ss(plateNumber
			    , province
			    , city
			    , county
			    , busRoute
			    , driverName
				, username
				, password
				, std::stoi(verbose)
			    );
  AlarmCheck ac(40, 5);
  Upss up(&ac, &ss);
  Uart u(3, &up);

  /*for (int i = 0; i < 1; ++i) {
	  Mat x;
	  x = cv::imread(imagefile);
	  if (x.empty()) {
		  x = Mat::zeros(100, 100, CV_8UC1);
	  }
	putText(x, std::to_string(cv::getTickCount()), cv::Point(0, 50), 1, 1, cv::Scalar(255, 255, 255));

    int buflen;  char *pp = mat2jpgbuf(x, buflen);
    ss.alarm_face(pp, buflen, 0.9);
  }*/
  getchar();
  //delete ss;
  return 0;
}
