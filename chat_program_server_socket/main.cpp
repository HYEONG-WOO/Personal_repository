#include <iostream>
#include <thread>
#include <WinSock2.h> 
#pragma comment(lib, "ws2_32") 

using namespace std;

char buff[100];	//	 메시지를 저장할 임시 배열
SOCKET serv_sock;	//	서버 소켓
SOCKET cli_sock[5];	//	클라이언트 소켓
sockaddr_in serv_addr;	//	서버 주소
sockaddr_in cli_addr;	//	클라이언트 주소

void init() {
	memset(buff, 0, sizeof(buff));
	WSADATA wsaData;	//	윈도우의 socket관련 기능 사용
	WSAStartup(MAKEWORD(2, 2), &wsaData);	//	socket관련 기능 사용 시작

	// 서버 소켓 만들기
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);	//	SOCK_STREAM : 연결지향형소켓

	// 소켓 생성시 에러가 나면 에러처리
	if (serv_sock == SOCKET_ERROR) {
		cout << "Socket Error!!" << endl;
		return;
	}

	serv_addr.sin_family = AF_INET;	//	주소체계
	serv_addr.sin_port = htons(9999);	//	서버소켓 포트번호 설정, 2바이트(0~65535 설정가능)
	serv_addr.sin_addr.S_un.S_addr = ADDR_ANY; // 자신의 IP사용

	//	소켓 바인딩(소켓 장착)
	//	bind(서버소켓, 서버주소, 서버주소크기);
	bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

	//	리슨
	//	listen(서버소켓, 최대처리개수);
	listen(serv_sock, 5);
}

void send_msg(char* msg) {
	for (int i = 0; i < 5; i++) {
		send(cli_sock[i], msg, sizeof(buff), 0);
	}
}
void recv_msg(SOCKET cli_sock) {
	while (true) {
		//	recv(데이터를 받을 소켓, 받을 데이터, 받을 테이터의 크기, 0);
		char recv_buff[100] = { 0 };
		recv(cli_sock, recv_buff, sizeof(recv_buff), 0);
		send_msg(recv_buff);	//	모든 클라이언트에게 데이터 전송
	}
}


void accept_work() {
	//	수신 
	int len = sizeof(cli_addr);
	for (int i = 0; i < 5; i++) {
		cout << "접속 대기중.." << endl;
		//	acppect(서버소켓, 클라이언트 주소, 클라이언트 주소길이);
		cli_sock[i] = accept(serv_sock, (sockaddr*)&cli_addr, &len);
		//	데이터 전송	
		cout << cli_addr.sin_addr.S_un.S_addr << "접속" << endl;
		strcpy(buff, "welcome to my server");
		//	send(데이터를 보낼 소켓, 보낼 데이터, 보낼 테이터의 크기, 0);
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

	// 마무리, 자원 해제
	for (int i = 0; i < 5; i++) {
		closesocket(cli_sock[i]);
	}
	closesocket(serv_sock);
	WSACleanup();

	return 0;
}
