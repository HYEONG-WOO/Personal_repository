#include <iostream>
#include <thread>
#include <WinSock2.h> 
#pragma comment(lib, "ws2_32") 

using namespace std;

char buff[100];	//	 �޽����� ������ �ӽ� �迭
SOCKET serv_sock;	//	���� ����
SOCKET cli_sock[5];	//	Ŭ���̾�Ʈ ����
sockaddr_in serv_addr;	//	���� �ּ�
sockaddr_in cli_addr;	//	Ŭ���̾�Ʈ �ּ�

void init() {
	memset(buff, 0, sizeof(buff));
	WSADATA wsaData;	//	�������� socket���� ��� ���
	WSAStartup(MAKEWORD(2, 2), &wsaData);	//	socket���� ��� ��� ����

	// ���� ���� �����
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);	//	SOCK_STREAM : ��������������

	// ���� ������ ������ ���� ����ó��
	if (serv_sock == SOCKET_ERROR) {
		cout << "Socket Error!!" << endl;
		return;
	}

	serv_addr.sin_family = AF_INET;	//	�ּ�ü��
	serv_addr.sin_port = htons(9999);	//	�������� ��Ʈ��ȣ ����, 2����Ʈ(0~65535 ��������)
	serv_addr.sin_addr.S_un.S_addr = ADDR_ANY; // �ڽ��� IP���

	//	���� ���ε�(���� ����)
	//	bind(��������, �����ּ�, �����ּ�ũ��);
	bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

	//	����
	//	listen(��������, �ִ�ó������);
	listen(serv_sock, 5);
}

void send_msg(char* msg) {
	for (int i = 0; i < 5; i++) {
		send(cli_sock[i], msg, sizeof(buff), 0);
	}
}
void recv_msg(SOCKET cli_sock) {
	while (true) {
		//	recv(�����͸� ���� ����, ���� ������, ���� �������� ũ��, 0);
		char recv_buff[100] = { 0 };
		recv(cli_sock, recv_buff, sizeof(recv_buff), 0);
		send_msg(recv_buff);	//	��� Ŭ���̾�Ʈ���� ������ ����
	}
}


void accept_work() {
	//	���� 
	int len = sizeof(cli_addr);
	for (int i = 0; i < 5; i++) {
		cout << "���� �����.." << endl;
		//	acppect(��������, Ŭ���̾�Ʈ �ּ�, Ŭ���̾�Ʈ �ּұ���);
		cli_sock[i] = accept(serv_sock, (sockaddr*)&cli_addr, &len);
		//	������ ����	
		cout << cli_addr.sin_addr.S_un.S_addr << "����" << endl;
		strcpy(buff, "welcome to my server");
		//	send(�����͸� ���� ����, ���� ������, ���� �������� ũ��, 0);
		send(cli_sock[i], buff, sizeof(buff), 0);

		if (cli_sock[i] == SOCKET_ERROR) {
			cout << "accept error!!" << endl;
			return;
		}

		thread recv_t1(recv_msg, cli_sock[i]);
		recv_t1.detach();
	}
}


int main() {

	init();

	thread send_msg_t2(accept_work);

	send_msg_t2.join();

	// ������, �ڿ� ����
	for (int i = 0; i < 5; i++) {
		closesocket(cli_sock[i]);
	}
	closesocket(serv_sock);
	WSACleanup();

	return 0;
}
