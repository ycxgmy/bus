# 安装
## 安装运行环境 vc_redist.x64

# 使用
## 使用默认参数运行
双击bus.exe
## 带参数运行
开始->运行->cmd.exe
cd path\to\bus
bus.exe 67890 四川省 绵阳市 高新区 801 王王 test 123456 1 demo.jpg
参数分别为：车牌 省 市 县 公交路数 公交司机 登录账号 登录密码 调试信息 图片
以上参数可以部分省略，但顺序不能改变。
参数解释：
调试信息 1 表示打印细节 0 表示不打印细节


参数处理部分：
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
