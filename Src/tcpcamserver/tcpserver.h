//
// 2015-11-28, jjuiddong
//
// TCP/IP ���������� �̿��ؼ� ����ϴ� ��ü��.
// �ִ��� �����ϰ� �������.
//
// 2016-02-06
//		- gcc �� �۾�
//
#pragma once

#include "network.h"
#include "packetqueue.h"


namespace network
{
	class cTCPServer
	{
	public:
		cTCPServer();
		virtual ~cTCPServer();

		bool Init(const int port, const int packetSize = 512, const int maxPacketCount = 10, const int sleepMillis = 30);
		void Close();
		bool IsConnect() const;
		int MakeFdSet(OUT fd_set &out);
		bool AddSession(const SOCKET remoteSock);
		void RemoveSession(const SOCKET remoteSock);


		SOCKET m_svrSocket;
		vector<sSession> m_sessions;
		int m_port;
		bool m_isConnect;
		int m_maxBuffLen;
  		cPacketQueue m_sendQueue;
  		cPacketQueue m_recvQueue;

// 		HANDLE m_handle;
		pthread_t m_handle;
 		//DWORD m_threadId;
 		pthread_mutex_t m_criticalSection;
		bool m_threadLoop;
		int m_sleepMillis;
	};


	inline bool cTCPServer::IsConnect() const { return m_isConnect; }
}
