//
// ��Ʈ���ֿ� ���õ� ������ �����Ѵ�.
//
#pragma once


namespace cvproc
{
	// �̹��� ���� ��������
	struct sStreamingData
	{
		BYTE id;				// streaming id (���� ���̵𳢸� ����Ÿ�� ��ģ �Ŀ� ����Ѵ�.)
		BYTE chunkSize;	// chunk size (�̹��� �뷮 ������ ������ ����Ÿ ûũ�� ����)
		BYTE chunkIndex;	// chunk index
		BYTE isGray;		// gray 0 : 1
		BYTE isCompressed;	// jpeg compressed 0 : 1
		int imageBytes;			// image size (byte unit)
		BYTE *data;				// image data
	};


	// ���ù��� �������� ����� �����ϱ� ���� ���̴� ��������
	struct sStreamingProtocol
	{
		BYTE protocol;		// �������� Ÿ��
									// ���� ���������� �ϳ��ۿ� ������, Receiver�� IP�� ��Ʈ��
									// �˷��ֱ� ���� �뵵�� ���δ�. protocol = 100
		BYTE type;			// udp=1, tcp=0
		BYTE ip1, ip2, ip3, ip4;	// ip1.ip2.ip3.ip4 = ipv4
		unsigned int uip;
		int port;
	};


	const static int g_maxStreamSize = (int)pow(2, 15) - 1;
}