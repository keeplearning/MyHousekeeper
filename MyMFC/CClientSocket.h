#pragma once
class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();
	//初始化客户端套接字，并尝试链接服务器
	bool ConnectServer(CHAR* szIp, WORD port);
	//负责创建线程用来接收服务器的信息
	TCHAR *Recv();
	bool Close();
public:
	SOCKET m_socket;
	TCHAR m_bufRecv[2048];
};

