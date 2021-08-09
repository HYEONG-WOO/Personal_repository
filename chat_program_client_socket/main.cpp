#include <iostream>
#include <thread>
#include <WinSock2.h> 
#pragma comment(lib, "ws2_32") 

using namespace std;

char buff[100]; // 메시지를 저장할 임시 배열
SOCKET serv_sock; // 서버 소켓
SOCKET cli_sock; // 클라이언트 소켓
sockaddr_in serv_addr; // 서버 주소
sockaddr_in cli_addr; // 클라이언트 주소

void init() {

	memset(buff, 0, sizeof(buff));
	WSADATA wsaData;	//	윈도우의 socket관련 기능 사용
	WSAStartup(MAKEWORD(2, 2), &wsaData);	//	socket관련 기능 사용 시작

	// 소켓 만들기	
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);

	// 소켓 생성시 에러가 나면 에러처리
	if (serv_sock == SOCKET_ERROR) {
		cout << "Socket Error!!" << endl;
		return;
	}

	serv_addr.sin_family = AF_INET; // 주소체계
	serv_addr.sin_port = htons(9999); // 서버소켓 포트번호 설정, 2바이트(0~65535 설정가능)
	serv_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // 접속할 서버 주소, localhost
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

	// 서버에 접속	
	connect(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

	// 데이터 수신	
	thread recv_msg_t1(recv_msg, serv_sock);

	// 데이터 송신
	char send_buff[100] = { 0 };

	while (true) {
		memset(send_buff, 0, sizeof(send_buff));
		cin.getline(send_buff, sizeof(send_buff));
		send(serv_sock, send_buff, sizeof(send_buff), 0);
	}

	// 마무리 자원 해제
	recv_msg_t1.join();
	closesocket(serv_sock);
	WSACleanup();

	return 0;
}