# ��װ
## ��װ���л��� vc_redist.x64

# ʹ��
## ʹ��Ĭ�ϲ�������
˫��bus.exe
## ����������
��ʼ->����->cmd.exe
cd path\to\bus
bus.exe 67890 �Ĵ�ʡ ������ ������ 801 ���� test 123456 1 demo.jpg
�����ֱ�Ϊ������ ʡ �� �� ����·�� ����˾�� ��¼�˺� ��¼���� ������Ϣ ͼƬ
���ϲ������Բ���ʡ�ԣ���˳���ܸı䡣
�������ͣ�
������Ϣ 1 ��ʾ��ӡϸ�� 0 ��ʾ����ӡϸ��


���������֣�
string plateNumber = "67890"; if (argc > 1) plateNumber = argv[1];
string province = "�Ĵ�ʡ";   if (argc > 2) province = argv[2];
string city = "������";       if (argc > 3) city = argv[3];
string county = "������";     if (argc > 4) county = argv[4];
string busRoute = "801";      if (argc > 5) busRoute = argv[5];
string driverName = "����";   if (argc > 6) driverName = argv[6];
string username = "test";     if (argc > 7) username = argv[7];
string password = "123456";   if (argc > 8) password = argv[8];
string verbose = "1";         if (argc > 9) verbose = argv[9];
string imagefile = "";        if (argc > 10) imagefile = argv[10];
