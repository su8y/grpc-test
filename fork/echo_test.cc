#include <iostream>
#include <string>
#include <fstream>
#include <chrono>


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
  
    system("echo hello world > /dev/null");

    auto end = std::chrono::steady_clock::now();
    log_file << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
  }
  std::cout <<  std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startup).count() << std::endl;

  return 0;
}
