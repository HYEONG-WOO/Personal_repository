#include <iostream>
#include <thread>
#include <WinSock2.h> 
#pragma comment(lib, "ws2_32") 

using namespace std;

char buff[100]; // �޽����� ������ �ӽ� �迭
SOCKET serv_sock; // ���� ����
SOCKET cli_sock; // Ŭ���̾�Ʈ ����
sockaddr_in serv_addr; // ���� �ּ�
sockaddr_in cli_addr; // Ŭ���̾�Ʈ �ּ�

void init() {

	memset(buff, 0, sizeof(buff));
	WSADATA wsaData;	//	�������� socket���� ��� ���
	WSAStartup(MAKEWORD(2, 2), &wsaData);	//	socket���� ��� ��� ����

	// ���� �����	
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);

	// ���� ������ ������ ���� ����ó��
	if (serv_sock == SOCKET_ERROR) {
		cout << "Socket Error!!" << endl;
		return;
	}

	serv_addr.sin_family = AF_INET; // �ּ�ü��
	serv_addr.sin_port = htons(9999); // �������� ��Ʈ��ȣ ����, 2����Ʈ(0~65535 ��������)
	serv_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // ������ ���� �ּ�, localhost
}

void recv_msg(SOCKET serv_sock) {
	while (true) {
		char buff[100] = { 0 };
		memset(buff, 0, sizeof(buff));
		recv(serv_sock, buff, sizeof(buff), 0);
		cout << buff << endl;
	}
}

int main() {

	init();

	// ������ ����	
	connect(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

	// ������ ����	
	thread recv_msg_t1(recv_msg, serv_sock);

	// ������ �۽�
	char send_buff[100] = { 0 };

	while (true) {
		memset(send_buff, 0, sizeof(send_buff));
		cin.getline(send_buff, sizeof(send_buff));
		send(serv_sock, send_buff, sizeof(send_buff), 0);
	}

	// ������ �ڿ� ����
	recv_msg_t1.join();
	closesocket(serv_sock);
	WSACleanup();

	return 0;
}