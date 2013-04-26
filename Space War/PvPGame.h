#ifndef PVPGAME_H
#define PVPGAME_H

#pragma comment(lib, "ws2_32.lib")

#include "BaseGame.h"

#define SockAddr struct sockaddr_in
#define SERVER_PORT 50000

struct SOCK_MSG
{
	int i;
	int j;
	int flag;
};

class PvPGame : public BaseGame
{
public:
	PvPGame();
	~PvPGame();

	virtual void init();	
	virtual int sendState(int x, int y, int *result);	//should be called in a separate thread
	virtual int receiveState(int *x, int *y);			//should be called in a separate thread
	virtual int sendResult(int result);					//should be called in a separate thread
	virtual void close();

	int connectToGame(const char* addr_in);	//should be called in a separate thread
	int createGame();						//should be called in a separate thread

protected:
	int bindTo(DWORD address);
	int beginListen();
	int acceptConnection();
protected:
	bool workAsServer;
	WSADATA WSStartData;
	SOCKET sock;
	SockAddr sockAddr;
	SOCKET servSock;
};

#endif //PVPGAME_H