//
// 스트리밍에 관련된 전역을 선언한다.
//
#pragma once


namespace cvproc
{
	// 이미지 전송 프로토콜
	struct sStreamingData
	{
		BYTE id;				// streaming id (같은 아이디끼리 데이타를 합친 후에 출력한다.)
		BYTE chunkSize;	// chunk size (이미지 용량 때문에 나눠진 데이타 청크의 갯수)
		BYTE chunkIndex;	// chunk index
		BYTE isGray;		// gray 0 : 1
		BYTE isCompressed;	// jpeg compressed 0 : 1
		int imageBytes;			// image size (byte unit)
		BYTE *data;				// image data
	};


	// 리시버와 센더간에 통신을 수립하기 위해 쓰이는 프로토콜
	struct sStreamingProtocol
	{
		BYTE protocol;		// 프로토콜 타입
									// 현재 프로토콜은 하나밖에 없으며, Receiver의 IP와 포트를
									// 알려주기 위한 용도로 쓰인다. protocol = 100
		BYTE type;			// udp=1, tcp=0
		BYTE ip1, ip2, ip3, ip4;	// ip1.ip2.ip3.ip4 = ipv4
		unsigned int uip;
		int port;
	};


	const static int g_maxStreamSize = (int)pow(2, 15) - 1;
}
