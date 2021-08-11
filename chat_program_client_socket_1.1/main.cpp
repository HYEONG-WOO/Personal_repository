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

	memset(buff, 0, sizeof(buff)/sizeof(char));
	WSADATA wsaData;	//	�������� socket���� ��� ���
	WSAStartup(MAKEWORD(2, 2), &wsaData);	//	socket���� ��� ��� ����

	// ���� �����	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

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
		char buff[100] = { 0 };
	while (true) {
		memset(buff, 0, sizeof(buff)/sizeof(char));
		if (recv(serv_sock, buff, sizeof(buff) / sizeof(char), 0) > 0) {		
		cout << buff << endl;
		}else {}
	}
}

int main() {

	init();

	// ������ ����	
	connect(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

	char N_name[10] = { 0 };
	cout << "�г����� �Է��ϼ��� :" << " ";
	cin >> N_name;
	// ������ ����	
	thread recv_msg_t1(recv_msg, serv_sock);

	//send(serv_sock, buf, sizeof(buf), 0);

	// ������ �۽�
	char send_buff[100] = { 0 };	
	char add_buff[100] = { 0 };
	while (true) {
		memset(send_buff, 0, sizeof(send_buff)/sizeof(char));
		cout << ">>";		
		cin.getline(send_buff, sizeof(send_buff)/sizeof(char));
		sprintf(add_buff, "%s : %s", N_name, send_buff);
		send(serv_sock, add_buff, sizeof(add_buff)/ sizeof(char), 0);
	}

	// ������ �ڿ� ����
	recv_msg_t1.join();
	closesocket(serv_sock);
	WSACleanup();

	return 0;
}