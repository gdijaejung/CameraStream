
#include "streamingsender.h"

using namespace cv;
using namespace cvproc;
using namespace std;


cStreamingSender::cStreamingSender() 
	: m_isCompressed(false)
	, m_jpgCompressQuality(40)
	, m_sndBuffer(NULL)
	, m_state(NOTCONNECT)
{
}

cStreamingSender::~cStreamingSender()
{
	SAFE_DELETEA(m_sndBuffer);
}


bool cStreamingSender::Init(const int port,
	const bool isConvertGray, const bool isCompressed, const int jpgQuality)
{
	m_isUDP = false;
	m_isConvertGray = isConvertGray;
	m_isCompressed = isCompressed;
	m_jpgCompressQuality = jpgQuality;
	m_tcpServer.SetListener(this);

	for (auto &udp : m_udpClient)
		udp.Close();
	m_tcpServer.Close();
	memset(m_udpUsed, 0, sizeof(m_udpUsed));

	if (!m_tcpServer.Init(port, g_maxStreamSize, 10, 10))
		return false;

	m_state = READY;

	if (m_gray.empty())
		m_gray = Mat(480, 640, CV_8UC1);

	if (m_compBuffer.capacity() == 0)
		m_compBuffer.reserve(307200);

	if (!m_sndBuffer)
		m_sndBuffer = new BYTE[g_maxStreamSize];

	return true;
}


void cStreamingSender::Close()
{
	for (auto &udp : m_udpClient)
		udp.Close();
	m_tcpServer.Close();
}


// 프로토콜을 확인해서 UDP/TCP 전송을 판단한다.
void cStreamingSender::CheckPacket()
{
	RET(!m_tcpServer.IsConnect());

	network::sPacket packet;
	if (m_tcpServer.m_recvQueue.Front(packet))
	{
		// 프로토콜 검사.
		if ((unsigned int)packet.actualLen >= sizeof(sStreamingProtocol))
		{

			sStreamingProtocol *data = (sStreamingProtocol*)packet.buffer;
			switch (data->protocol)
			{
			case 100: // udp 프로토콜 설정 패킷
				//m_users[packet.sock] = data->type;

				if (data->type == 1)  // udp라면, 소켓 생성
				{

					// 빈 udp 소켓을 찾는다.
					int selectIdx = -1;
					const int size = sizeof(m_udpUsed) / sizeof(bool);
					for (int i = 0; i < size; ++i)
					{
						if (!m_udpUsed[i])
						{
							selectIdx = i;
							break;
						}
					}

					if (selectIdx >= 0)
					{
						char *ip = inet_ntoa(*(in_addr*)&data->uip);
						cout << "select udp index " << selectIdx << endl;
						m_udpClient[selectIdx].SetMaxBufferLength(307200);
						if (m_udpClient[selectIdx].Init(ip, data->port))
						{
							m_udpUsed[selectIdx] = true;
						}
						else
						{
							cout << "udp init error " << ip << ", " << data->port << endl;
							selectIdx = -1;
						}
					}

					m_users[packet.sock] = selectIdx;
				}
				break;
			}

		}

		m_tcpServer.m_recvQueue.Pop(); // 처리한 패킷은 제거
	}

}


// 세션이 제거될 때 호출.
void cStreamingSender::RemoveSession(const SOCKET remoteSock)
{
	auto it = m_users.find(remoteSock);
	if (m_users.end() != it)
	{
		const int udpIdx = it->second;
		if (udpIdx >= 0)
		{
			cout << "udp socket close" << endl;
			m_udpClient[udpIdx].Close();
			m_udpUsed[udpIdx] = false;
		}
	}
}


// 세션이 추가될 때 호출.
void cStreamingSender::AddSession(const SOCKET remoteSock)
{

}


// image는 흑백 영상이어야 한다.
// 크기는 640/480 이어야 한다.
int cStreamingSender::Send(const cv::Mat &image)
{
	if (READY == m_state)
	{
		// 이미지를 변조한다.
		uchar *data = image.data;
		int buffSize = image.total() * image.elemSize();

		// Gray Scale
		if (m_isConvertGray)
		{
			cvtColor(image, m_gray, CV_RGB2GRAY);
			data = m_gray.data;
			buffSize = m_gray.total() * m_gray.elemSize();
		}

		// 압축
		if (m_isCompressed)
		{
			vector<int> p(3);
			p[0] = CV_IMWRITE_JPEG_QUALITY;
			p[1] = m_jpgCompressQuality;
			p[2] = 0;
			cv::imencode(".jpg", m_isConvertGray ? m_gray : image, m_compBuffer, p);
			data = (uchar*)&m_compBuffer[0];
			buffSize = m_compBuffer.size();
		}

		return SendImage(data, buffSize, m_isConvertGray, m_isCompressed);
	}
	else if (SPLIT == m_state)
	{
		if (SendSplit())
		{
			m_state = READY;
			++m_chunkId;
			if (m_chunkId > 100)
				m_chunkId = 0;
		}
	}

	return 0;
}


// 용량이 큰 데이타면, 분리해서 전송한다.
int cStreamingSender::SendImage(const BYTE *data, const int len,
	const bool isGray, const bool isCompressed)
{
	const int totalSize = len + sizeof(sStreamingData);

	if (totalSize < g_maxStreamSize)
	{
		sStreamingData *packet = (sStreamingData*)m_sndBuffer;
		packet->id = 0;
		packet->chunkSize = 1;
		packet->chunkIndex = 0;
		packet->imageBytes = len;
		packet->isGray = isGray ? 1 : 0;
		packet->isCompressed = isCompressed ? 1 : 0;
		packet->data = m_sndBuffer + sizeof(sStreamingData);
		memcpy(packet->data, data, len);

		for (auto session : m_tcpServer.m_sessions)
		{
			auto it = m_users.find(session.socket);
			int udpIdx = -1;
			if (it != m_users.end())
				udpIdx = it->second;

			if (udpIdx >= 0)
			{
				m_udpClient[udpIdx].SendData((BYTE*)packet, sizeof(sStreamingData) + len);
			}
			else
			{
				m_tcpServer.m_sendQueue.Push(session.socket, (BYTE*)packet, sizeof(sStreamingData) + len);
			}
		}

		return 1;
	}
	else
	{
		const int sizePerChunk = g_maxStreamSize - sizeof(sStreamingData); // size per chunk
		m_chunkCount = (len / sizePerChunk);
		if ((len % sizePerChunk) != 0)
			++m_chunkCount;

		m_sendChunkIndex = 0;
		m_sendBytes = 0;
		m_imagePtr = (BYTE*)data;
		m_imageBytes = len;
		SendSplit();

		m_state = SPLIT;
		return m_chunkCount;
	}
}


// 모든 청크를 전송했다면 true를 리턴한다.
bool cStreamingSender::SendSplit()
{
	const int sizePerChunk = g_maxStreamSize - sizeof(sStreamingData); // size per chunk

	sStreamingData *packet = (sStreamingData*)m_sndBuffer;
	packet->id = (unsigned char)m_chunkId;
	packet->chunkSize = m_chunkCount;
	packet->chunkIndex = m_sendChunkIndex;
	packet->imageBytes = m_imageBytes;
	packet->isGray = m_isConvertGray ? 1 : 0;
	packet->isCompressed = m_isCompressed ? 1 : 0;
	packet->data = m_sndBuffer + sizeof(sStreamingData);

	int copyLen = 0;
	if (m_sendBytes + sizePerChunk > m_imageBytes)
	{
		copyLen = m_imageBytes%sizePerChunk;
		memcpy(packet->data, m_imagePtr + m_sendChunkIndex * sizePerChunk, copyLen);
	}
	else
	{
		copyLen = sizePerChunk;
		memcpy(packet->data, m_imagePtr + m_sendChunkIndex * sizePerChunk, copyLen);
	}

	for (auto session : m_tcpServer.m_sessions)
	{
		auto it = m_users.find(session.socket);
		int udpIdx = -1;
		if (it != m_users.end())
			udpIdx = it->second;

		if (udpIdx >= 0)
		{
			m_udpClient[udpIdx].SendData((BYTE*)packet, sizeof(sStreamingData) + copyLen);
		}
		else
		{
			m_tcpServer.m_sendQueue.Push(session.socket, (BYTE*)packet, sizeof(sStreamingData) + copyLen);
		}
	}

	m_sendBytes += sizePerChunk;
	++m_sendChunkIndex;

	if (m_sendBytes >= m_imageBytes)
		return true;

	return false;
}


bool cStreamingSender::IsConnect(const bool isUdp)
{
//  	if (isUdp)
// 		return m_udpClient.IsConnect();
	return m_tcpServer.IsConnect();
}
