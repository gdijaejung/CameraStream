
#include "netlauncher.h"

using namespace network;


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	network::LaunchClient(const std::string &ip, const int port, OUT SOCKET &out)
{
	// TCP/IP ��Ʈ�� ������ �����մϴ�.
	// socket(�ּ� �迭, ���� ����, ��������
	SOCKET ssock = socket(AF_INET, SOCK_STREAM, 0);
	if (ssock == INVALID_SOCKET)
	{
		//clog::Error( clog::ERROR_CRITICAL, "socket() error\n" );
		return false;
	}

	// �ּ� ����ü�� ä��ϴ�.
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	server_addr.sin_port = htons(port);
	int clen = sizeof(server_addr);

	// ������ �����մϴ�
	// connect(����, ���� �ּ�, ���� �ּ��� ����
	int nRet = connect(ssock, (struct sockaddr*)&server_addr, clen);
	if (nRet == SOCKET_ERROR)
	{
		//clog::Error( clog::ERROR_CRITICAL, "connect() error ip=%s, port=%d\n", ip.c_str(), port );
		close(ssock);
		return false;
	}

	out = ssock;

	return true;
}



//------------------------------------------------------------------------
// ���� ����
//------------------------------------------------------------------------
bool network::LaunchServer(const int port, OUT SOCKET &out)
{
	int ssock;
	if ((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		//perror("socket()");
		return false;
	}

	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);

	if (bind(ssock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		//perror("bind()");
		return false;
	}

	if (listen(ssock, 8) < 0)
	{
		//perror("listen()");
		return false;
	}

	char szBuf[256];
	int nRet = gethostname(szBuf, sizeof(szBuf));
	if (nRet == SOCKET_ERROR)
	{
		//clog::Error( clog::ERROR_CRITICAL, "gethostname() error\n" );
		close(ssock);
		return false;
	}

	out = ssock;

	return true;
}



//------------------------------------------------------------------------
// ���� ����
//------------------------------------------------------------------------
bool network::LaunchUDPServer(const int port, OUT SOCKET &out)
{
// 	// ������ �����ϰ� ������ Ȯ���մϴ�
// 	WORD wVersionRequested = MAKEWORD(1, 1);
// 	WSADATA wsaData;
// 	int nRet = WSAStartup(wVersionRequested, &wsaData);
// 	if (wsaData.wVersion != wVersionRequested)
// 	{
// 		//clog::Error( clog::ERROR_CRITICAL,  "���� ������ Ʋ�Ƚ��ϴ�\n" );
// 		return false;
// 	}
// 
// 	// socket(�ּҰ迭, ���� ����, ��������)
// 	//	SOCKET svrSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// 	SOCKET svrSocket = socket(AF_INET, SOCK_DGRAM, 0);
// 	if (svrSocket == INVALID_SOCKET)
// 	{
// 		//clog::Error( clog::ERROR_CRITICAL, "socket() error\n" );
// 		return false;
// 	}
// 
// 	// �ּ� ����ü�� ä��ϴ�.
// 	SOCKADDR_IN saServer;
// 	saServer.sin_family = AF_INET;
// 	saServer.sin_addr.s_addr = INADDR_ANY;    // ������ �����ϰ� �Ӵϴ�.
// 	saServer.sin_port = htons(port);		// �����ٿ��� ���� ��Ʈ�� �ֽ��ϴ�.
// 
// 	// ���ϰ� listensocket �� bind(����) �մϴ�.
// 	// bind(����, ���� �ּ�, �ּ� ����ü�� ����
// 	nRet = bind(svrSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr));
// 	if (nRet == SOCKET_ERROR)
// 	{
// 		//clog::Error( clog::ERROR_CRITICAL,  "bind() error port: %d\n", port );
// 		closesocket(svrSocket);
// 		return false;
// 	}
// 
// 	char szBuf[256];
// 	nRet = gethostname(szBuf, sizeof(szBuf));
// 	if (nRet == SOCKET_ERROR)
// 	{
// 		//clog::Error( clog::ERROR_CRITICAL, "gethostname() error\n" );
// 		closesocket(svrSocket);
// 		return false;
// 	}
// 
// 	out = svrSocket;

	return true;
}



//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::LaunchUDPClient(const std::string &ip, const int port, OUT sockaddr_in &sockAddr, OUT SOCKET &out)
{

	// UDP/IP ��Ʈ�� ������ �����մϴ�.
	// socket(�ּ� �迭, ���� ����, ��������
	SOCKET ssock = socket(AF_INET, SOCK_DGRAM, 0);
	if (ssock == INVALID_SOCKET)
	{
		//clog::Error( clog::ERROR_CRITICAL, "socket() error\n" );
		return false;
	}

	// �ּ� ����ü�� ä��ϴ�.
	bzero(&sockAddr, sizeof(sockaddr_in));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	sockAddr.sin_port = htons(port);
	int clen = sizeof(sockaddr_in);

	// ������ �����մϴ�
	// connect(����, ���� �ּ�, ���� �ּ��� ����
	int nRet = connect(ssock, (struct sockaddr*)&sockAddr, clen);
	if (nRet == SOCKET_ERROR)
	{
		//clog::Error( clog::ERROR_CRITICAL, "connect() error ip=%s, port=%d\n", ip.c_str(), port );
		close(ssock);
		return false;
	}

	out = ssock;

	return true;
}