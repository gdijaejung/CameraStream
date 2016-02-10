
#include <iostream>
#include "../opencvcom/streamingreceiver.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	if (argc <= 2)
	{
		cout << "commandline <ip> <port> <udp=1, tcp=0>" << endl;
		return 0;
	}

	const string ip = argv[1];
	const int port = atoi(argv[2]);
	const bool isudp = atoi(argv[3]) == 0 ? true : false;

	cout << "streaming client init... " << endl;
	cvproc::cStreamingReceiver receiver;
	if (!receiver.Init(isudp, ip, port))
	{
		cout << "connect error!!" << endl;
		return 0;
	}

	cout << "success" << endl;
	cout << "start loop" << endl;

	const string windowName = "Camera Streaming Client";
	namedWindow(windowName);

	while (1)
	{
		imshow(windowName, receiver.Update());
		cvWaitKey(0);
	}

	return 0;
}
