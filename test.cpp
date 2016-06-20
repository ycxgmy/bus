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

  Mat x = Mat::zeros(100,100, CV_8UC1);
  putText(x, std::to_string(getTickCount()),Point(0,50), 1, 1, Scalar(255,255,255));
  imwrite("tmp.jpg", x);
  ss->alarm_face("tmp.jpg", 0.9);
  cout << "x" << endl;
  //ss->alarm_face();
  delete ss;
  return 0;
  /*{
    for (int i = 0; i < sizeof(N_ERROR)/sizeof(N_ERROR[0]); ++i)
      cout << N_ERROR[i] << " " << S_ERROR[i] << endl;
    for (int i = 0; i < sizeof(N_ALARM)/sizeof(N_ALARM[0]); ++i)
      cout << N_ALARM[i] << " " << S_ALARM[i] << endl;
    return 0;
    }*/
  Protocal *pp = new Protocal;
  Json json;

  // login
  int code = pp->login("test", "123456", 1, json);
  //cout << json.dump() << endl;
  if (code != N_ERROR[0]) {
    cerr << pp->p_error(code) << ", more info: \n" << json.dump() << endl;
    return -1;
    }

  int resultCode = json["resultCode"].int_value();
  string token = json["token"].string_value();
  string tokenExpires = json["tokenExpires"].string_value();
  string userName = json["userName"].string_value();
  string nickName = json["nickName"].string_value();
  int userId = json["userId"].int_value();

  // connt
  code = pp->connt(token, userId, json);
  //cout << json.dump() << endl;
  if (code != N_ERROR[0]) {
    cerr << pp->p_error(code) << ", more info: \n" << json.dump() << endl;
  } else {
    resultCode = json["resultCode"].int_value();
    token = json["token"].string_value();
    tokenExpires = json["tokenExpires"].string_value();
  }

  // alarm
  // alarm.face
  string alarmRecordDateTime = "2016-5-19 14:13:59";
  Json o = Json::object {
    {"plateNumber","67890"},
    {"province","四川省"},
    {"city","绵阳市"},
    {"county","高新区"},
    {"busRoute","801"},
    {"driverName","王王"},
    {"eventTime",alarmRecordDateTime},
    {"precisions",0.9},
  };
  code = pp->alarm(token, FACE, alarmRecordDateTime, o, userId, json);
  if (code != N_ERROR[0]) {
    cerr << pp->p_error(code) << ", more info: \n" << json.dump() << endl;
    //return -2;
  }
  resultCode = json["resultCode"].int_value();
  //token = json["token"].string_value();
  int alarmRecordId = json["alarmRecordId"].int_value();
  userId = json["userId"].int_value();

  // alarm.phone
  code = pp->alarm(token, PHONE, alarmRecordDateTime, o, userId, json);
  if (code != N_ERROR[0]) {
    cerr << pp->p_error(code) << ", more info: \n" << json.dump() << endl;
    //return -2;
  }
  resultCode = json["resultCode"].int_value();
  //token = json["token"].string_value();
  alarmRecordId = json["alarmRecordId"].int_value();
  userId = json["userId"].int_value();
  int behaviourAlarmId = json["contents"].int_value();

  // upload file info
  //code = pp->upload_file_info(token, userId, "demo.jpg", "7717d9e7dc17d085fe9e012f06558699", 13806, 13806, 1, "jpg", alarmRecordId, 1, json);
  //code = pp->upload_file_info("12ea66ce789e44a0a15292c8ba6ae92b", 2, "demo.jpg", "7717d9e7dc17d085fe9e012f06558699", 13806, 13806, 1, "jpg", 1, 37, json);
  //return 0;
  //token = "test_token_10001";  userId = 2;  behaviourAlarmId = 1;  alarmRecordId = 37;
  string filename = "6.png";
  code = pp->upload_file(token, userId, filename, behaviourAlarmId, alarmRecordId, json);
  //cout << json.dump() << endl;
  if (code != N_ERROR[0]) {
    cerr << pp->p_error(code) << ", more info: \n" << json.dump() << endl;
    return -2;
  }

  delete pp;
  return 0;
}
