//
// Protocol
//	- (byte) chunk size (이미지 용량 때문에 나눠진 데이타 청크의 갯수)
//	- (byte) chunk index
//	- (byte) gray 0 : 1
//	- (byte) compressed 0 : 1
//	- (int) image size
//
// 2016-02-09
//		- linux 작업
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
		cv::Mat m_finalImage;		// 최종 이미지
		BYTE *m_rcvBuffer;
	};
}
