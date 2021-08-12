#include <iostream>
#include <thread>
#include <chrono>
#include <WinSock2.h> // ������ �������α׷��� ��� ����
#pragma comment(lib, "ws2_32") // ������ ���� ���α׷��� ��� ���� ���̺귯��

using namespace std;
using std::this_thread::sleep_for;

char buff[100]; // �޽����� ������ �ӽ� �迭
SOCKET serv_sock; // ���� ����
SOCKET cli_sock[5]; // Ŭ���̾�Ʈ ����
sockaddr_in serv_addr; // ���� �ּ�(IP)
sockaddr_in cli_addr; // Ŭ���̾�Ʈ �ּ�(IP)


void init() {
	memset(buff, 0, sizeof(buff) / sizeof(char)); // buff�� ��� ������ 0���� �ʱ�ȭ
	//buff[0] = '\0';
	WSADATA wsaData; // �������� socket���� ��ɵ��� ����� �� �ְ� ����.
	WSAStartup(MAKEWORD(2, 2), &wsaData); // �������� socket���� ��� ��� ����
	// ���� ���� �����
	// socket(AF_INET, SOCK_STREAM, 0) ip v4, TCP/UDP, 0
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	// ���� ��� �������� ������ ���� ����ó��
	if (serv_sock == SOCKET_ERROR) {
		cout << "Socket Error!!" << endl;
		return;
	}
	else {}

	serv_addr.sin_family = AF_INET; // �ּ�ü��
	serv_addr.sin_port = htons(9999); // �������� ��Ʈ��ȣ ����, 2����Ʈ(0~65535 ��������)
	serv_addr.sin_addr.S_un.S_addr = ADDR_ANY; // ADDR_ANY : �ڽ��� IP���

	// ���� ���ε�(���� ����)
	// bind(��������, �����ּ�, �����ּ�ũ��)
	bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

	// ���� -> ��û ���
	// listen(��������, �ִ�ó������);
	listen(serv_sock, 5);

}

// ��� Ŭ���̾�Ʈ���� �޽��� �Ѹ��� �Լ�.
void send_msg(char* msg, int client, int len) {
	/*int num = 0;
	while (msg[num] != '\0') {
		num = num + 1;
	}*/
	for (int i = 0; i < 5; i++) {
		if (client != cli_sock[i]) {
			//sprintf(buf, "id[%d] : %s\n\0", �г���, msg);
			send(cli_sock[i], msg, len, 0);
		}
		else {}

	}
}

void recv_msg(SOCKET cli_sock) {
	while (true) {
		char recv_buff[100] = { 0 };
		//mutex(lock)
		// Ŭ���̾�Ʈ�� �Է��� �޽�����
		if (recv(cli_sock, recv_buff, sizeof(recv_buff) / sizeof(char), 0) > 0) {
			cout << cli_sock << " " << recv_buff << endl;
			send_msg(recv_buff, cli_sock, sizeof(recv_buff) / sizeof(char)); // ��� Ŭ���̾�Ʈ���� �Ѹ���.
		}
		else {}
		//mutex(unlock)
	}
}

void accept_work() {

	int len = sizeof(cli_addr);
	for (int i = 0; i < 5; i++) {
		cout << "���.." << endl;
		cli_sock[i] = accept(serv_sock, (sockaddr*)&cli_addr, &len); // �ִ� 5���� Ŭ���̾�Ʈ ������ �����ϰ� ����

		// ���� ȯ�� �λ� ����			
		cout << cli_addr.sin_addr.S_un.S_addr << " ����" << endl;
		strcpy(buff, "welcome to my server");
		send(cli_sock[i], buff, sizeof(buff) / sizeof(char), 0);

		if (cli_sock[i] == SOCKET_ERROR) {
			cout << "accept error!!" << endl;
			return;
		}
		else {}

		// ������ ���� ������
		thread t1(recv_msg, cli_sock[i]);
		sleep_for(chrono::milliseconds(1000));
		t1.join(); // ������ �и�
	}
}

int main() {

	init();

	// ���� ���.. ���� 
	thread t2(accept_work);

	t2.join();
	// ������ �ڿ� ����
	for (int i = 0; i < 5; i++) {
		closesocket(cli_sock[i]);
	}
	closesocket(serv_sock);
	WSACleanup();

	return 0;
}