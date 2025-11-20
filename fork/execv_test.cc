#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <fcntl.h>      // open()
#include <sys/wait.h>


int main(int argc, char** argv) {
  auto startup = std::chrono::steady_clock::now();
  std::ofstream log_file("./out.txt", std::ios::app | std::ios::out);

  if(argc < 3) {
    std::cout << "require more than 2 arguments." << std::endl;
    std::cout << "./client <count> <sleep_us>" << std::endl;
    return -1;
  }

  // test 반복 인자 입력
  int count = std::stoi(argv[1]);
  int sleep_us = std::stoi(argv[2]);

  for(int i=0;i<count;i++) {
    auto start = std::chrono::steady_clock::now();
  
    pid_t pid = fork();

    if (pid < 0) {
        return 1;
    } 
    else if (pid == 0) {
        // [자식 프로세스]
        
        // 1. /dev/null 파일을 쓰기 전용으로 엽니다.
        int devNull = open("/dev/null", O_WRONLY);
        
        if (devNull == -1) {
            perror("open /dev/null failed");
            exit(1);
        }

        // 2. dup2(old, new): devNull 파일 디스크립터를 STDOUT(1번) 자리로 복사합니다.
        // 이제 printf나 echo가 출력하는 모든 내용은 devNull로 들어갑니다.
        dup2(devNull, STDOUT_FILENO);
        
        // 3. 복사했으니 원본 devNull 핸들은 닫습니다.
        close(devNull);

        // 4. 이제 echo를 실행합니다.
        const char* path = "/bin/echo";
        char* args[] = { (char*)"/bin/echo", (char*)"hello", (char*)"world", NULL };
        
        execv(path, args);
        exit(1); // execv 실패 시 종료
    } 
    else {
        // [부모 프로세스]
        int status;
        wait(&status);
    }

    auto end = std::chrono::steady_clock::now();
    log_file << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
  }
  std::cout <<  std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startup).count() << std::endl;

  return 0;
}
