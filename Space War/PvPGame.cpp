#include "PvPGame.h"

#include "stdafx.h"

PvPGame::PvPGame()
	: workAsServer(true),
	sock(INVALID_SOCKET),
	servSock(INVALID_SOCKET)
{

}

PvPGame::~PvPGame()
{
}

void PvPGame::init()
{
	WSAStartup( MAKEWORD(2,0), &WSStartData);
	ZeroMemory( &sockAddr, sizeof(sockAddr) );
	sock = socket(AF_INET, SOCK_STREAM, 0);
}

int PvPGame::connectToGame(const char* in_addr)
{
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(in_addr);
	sockAddr.sin_port = htons(SERVER_PORT);
	workAsServer = false;
	return connect(sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
}

int PvPGame::bindTo(DWORD address)
{
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = htonl(address);
	sockAddr.sin_port = htons( SERVER_PORT );

	return bind(sock, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
}

int PvPGame::beginListen()
{
	return listen(sock, 2);
}

int PvPGame::acceptConnection()
{
	int addr_len = sizeof(sockAddr);
	servSock = accept(sock, (struct sockaddr*)&sockAddr, &addr_len);
	if( servSock==INVALID_SOCKET ) return 1;
	else return 0;
}

int PvPGame::createGame()
{
	int exitCode = 0;
	exitCode = bindTo(INADDR_ANY);
	exitCode = beginListen();
	exitCode = acceptConnection();
	workAsServer = true;
	return exitCode;
}

int PvPGame::sendState(int x, int y, int *result)
{
	DWORD disconnect = 0;
	LONG32 nRemainRecv = 0, nXfer, nRemainSend;
	LPBYTE pBuffer;

	//Send my bomb to enemy. Hope to hit him.
	nRemainSend = sizeof(SOCK_MSG);
	SOCK_MSG msg;
	msg.i = x;
	msg.j = y;
	pBuffer = (LPBYTE)&msg;
	while( nRemainSend > 0 && !disconnect )
	{
		if( workAsServer )
		{
			nXfer = send(servSock, (char*)pBuffer, nRemainSend, 0);
		}
		else
		{
			nXfer = send(sock, (char*)pBuffer, nRemainSend, 0);
		}
		if( nXfer == SOCKET_ERROR ) return 1;
		disconnect = (nXfer==0);
		nRemainSend -= nXfer;
		pBuffer += nXfer;
	}

	//Receive result: did I hit him or not? boolean value
	nRemainRecv = sizeof(int);
	int val;
	pBuffer = (LPBYTE)&val;
	disconnect = 0;
	while( nRemainRecv > 0 && !disconnect )
	{
		if( workAsServer )
		{
			nXfer = recv( servSock, (char*)pBuffer, nRemainRecv, 0);
		}
		else
		{
			nXfer = recv( sock, (char*)pBuffer, nRemainRecv, 0);
		}
		disconnect = (nXfer==0);
		nRemainRecv -= nXfer;
		pBuffer += nXfer;
	}

	*result = val;
	return 0;
}
	
int PvPGame::receiveState(int *x, int *y)
{
	DWORD disconnect = 0;
	LONG32 nRemainRecv = 0, nXfer, nRemainSend = 0;
	LPBYTE pBuffer;

	nRemainRecv = sizeof(SOCK_MSG);
	SOCK_MSG msg;
	pBuffer = (LPBYTE)&msg;

	//Enemy's bomb is arriving. Hold on!
	while( nRemainRecv > 0 && !disconnect )
	{
		if( workAsServer )
		{
			nXfer = recv(servSock, (char*)pBuffer, nRemainRecv, 0);
		}
		else
		{
			nXfer = recv(sock, (char*)pBuffer, nRemainRecv, 0);
		}
		disconnect = (nXfer==0);
		nRemainRecv -= nXfer;
		pBuffer += nXfer;
	}
	*x = msg.i;
	*y = msg.j;
	//Check if he hit us? 
	int result = 0;
	int ship = map->at(msg.i).at(msg.j);
	switch( ship )
	{
	case -1:
		result = 0; //He missed! Ha! Looser!
		break;
	case -2:
		result = 0; //Already used location! Dumbass!
		break;
	default:
		if( ship >=0 && ship < ships->size() )
		{
			result = true;	//He hit us! Bastard!
			map->at(msg.i).at(msg.j) = -2; //Location bombed!
		}
		else
		{
			result = false; //undefined location
		}
		break;
	}
	
	//Let him know
	nRemainSend = sizeof(bool);
	pBuffer = (LPBYTE)&result;
	disconnect = 0;
	while( nRemainSend > 0 && !disconnect )
	{
		if( workAsServer )
		{
			nXfer = send(servSock, (char*)pBuffer, nRemainSend, 0);
		}
		else
		{
			nXfer = send(sock, (char*)pBuffer, nRemainSend, 0);
		}
		if( nXfer == SOCKET_ERROR ) return 1;
		disconnect = (nXfer==0);
		nRemainSend -= nXfer;
		pBuffer += nXfer;
	}
	return 0;
}

void PvPGame::close()
{
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	sock = INVALID_SOCKET;
	closesocket(servSock);
	servSock = INVALID_SOCKET;
	WSACleanup();
}