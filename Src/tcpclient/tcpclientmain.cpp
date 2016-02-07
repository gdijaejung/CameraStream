//
// 2016-02-06, jjuiddong
//
// linux tcp client 
// 

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char **argv)
{
	int ssock;
	struct sockaddr_in server_addr;
	char buf[BUFSIZ] = "Hello world";

	if (argc < 2)
	{
		cout << "Usage : " << argv[0] << " <IP_ADDRESS> <PORT>" << endl;
		return -1;
	}

	if ((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		return -1;
	}

	cout << "tcp client start.. " << endl;

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if (connect(ssock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("connect()");
		return -1;
	}

	cout << "tcp client connect success.. " << endl;

	fgets(buf, BUFSIZ, stdin);
	if (write(ssock, buf, BUFSIZ) <= 0)
	{
		perror("write()");
		return -1;
	}

	bzero(buf, BUFSIZ);
	if (read(ssock, buf, BUFSIZ) <= 0)
	{
		perror("read()");
		return -1;
	}

	cout << "Received data : " << buf << endl;

	close(ssock);

	return 0;
}
