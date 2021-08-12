#include <iostream>
#include <thread>
#include <chrono>
#include <WinSock2.h> // 윈도우 소켓프로그래밍 헤더 파일
#pragma comment(lib, "ws2_32") // 윈도우 소켓 프로그래밍 기능 제공 라이브러리

using namespace std;
using std::this_thread::sleep_for;

char buff[100]; // 메시지를 저장할 임시 배열
SOCKET serv_sock; // 서버 소켓
SOCKET cli_sock[5]; // 클라이언트 소켓
sockaddr_in serv_addr; // 서버 주소(IP)
sockaddr_in cli_addr; // 클라이언트 주소(IP)


void init() {
	memset(buff, 0, sizeof(buff) / sizeof(char)); // buff의 모든 영역을 0으로 초기화
	//buff[0] = '\0';
	WSADATA wsaData; // 윈도우의 socket관련 기능들을 사용할 수 있게 해줌.
	WSAStartup(MAKEWORD(2, 2), &wsaData); // 윈도우의 socket관련 기능 사용 시작
	// 서버 소켓 만들기
	// socket(AF_INET, SOCK_STREAM, 0) ip v4, TCP/UDP, 0
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	// 소켓 얻는 과정에서 에러가 나면 에러처리
	if (serv_sock == SOCKET_ERROR) {
		cout << "Socket Error!!" << endl;
		return;
	}
	else {}

	serv_addr.sin_family = AF_INET; // 주소체계
	serv_addr.sin_port = htons(9999); // 서버소켓 포트번호 설정, 2바이트(0~65535 설정가능)
	serv_addr.sin_addr.S_un.S_addr = ADDR_ANY; // ADDR_ANY : 자신의 IP사용

	// 소켓 바인딩(소켓 장착)
	// bind(서버소켓, 서버주소, 서버주소크기)
	bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

	// 리슨 -> 요청 대기
	// listen(서버소켓, 최대처리개수);
	listen(serv_sock, 5);

}

// 모든 클라이언트에게 메시지 뿌리는 함수.
void send_msg(char* msg, int client, int len) {
	/*int num = 0;
	while (msg[num] != '\0') {
		num = num + 1;
	}*/
	for (int i = 0; i < 5; i++) {
		if (client != cli_sock[i]) {
			//sprintf(buf, "id[%d] : %s\n\0", 닉네임, msg);
			send(cli_sock[i], msg, len, 0);
		}
		else {}

	}
}

void recv_msg(SOCKET cli_sock) {
	while (true) {
		char recv_buff[100] = { 0 };
		//mutex(lock)
		// 클라이언트가 입력한 메시지를
		if (recv(cli_sock, recv_buff, sizeof(recv_buff) / sizeof(char), 0) > 0) {
			cout << cli_sock << " " << recv_buff << endl;
			send_msg(recv_buff, cli_sock, sizeof(recv_buff) / sizeof(char)); // 모든 클라이언트에게 뿌린다.
		}
		else {}
		//mutex(unlock)
	}
}

void accept_work() {

	int len = sizeof(cli_addr);
	for (int i = 0; i < 5; i++) {
		cout << "대기.." << endl;
		cli_sock[i] = accept(serv_sock, (sockaddr*)&cli_addr, &len); // 최대 5개의 클라이언트 소켓을 연결하고 저장

		// 접속 환영 인사 전송			
		cout << cli_addr.sin_addr.S_un.S_addr << " 접속" << endl;
		strcpy(buff, "welcome to my server");
		send(cli_sock[i], buff, sizeof(buff) / sizeof(char), 0);

		if (cli_sock[i] == SOCKET_ERROR) {
			cout << "accept error!!" << endl;
			return;
		}
		else {}

		// 데이터 수신 스레드
		thread t1(recv_msg, cli_sock[i]);
		sleep_for(chrono::milliseconds(1000));
		t1.join(); // 쓰레드 분리
	}
}

int main() {

	init();

	// 연결 대기.. 연결 
	thread t2(accept_work);

	t2.join();
	// 마무리 자원 해제
	for (int i = 0; i < 5; i++) {
		closesocket(cli_sock[i]);
	}
	closesocket(serv_sock);
	WSACleanup();

	return 0;
}