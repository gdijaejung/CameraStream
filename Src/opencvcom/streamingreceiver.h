//
// Protocol
//	- (byte) chunk size (�̹��� �뷮 ������ ������ ����Ÿ ûũ�� ����)
//	- (byte) chunk index
//	- (byte) gray 0 : 1
//	- (byte) compressed 0 : 1
//	- (int) image size
//
// 2016-02-09
//		- linux �۾�
//		- sever -> client
//
#pragma once

#include "../netcom/network.h"
#include "opencvcom.h"
#include "streaming.h"


namespace cvproc
{
	class cStreamingReceiver
	{
	public:
		cStreamingReceiver();
		virtual ~cStreamingReceiver();

		bool Init(const bool isUDP, const string &ip, const int bindPort);
		cv::Mat& Update();
		bool IsConnect();
		void Close();


		network::cUDPClient m_udpClient;
		network::cTCPClient m_tcpClient;
		bool m_isUDP;
		cv::Mat m_src;
		cv::Mat m_finalImage;		// ���� �̹���
		BYTE *m_rcvBuffer;
	};
}