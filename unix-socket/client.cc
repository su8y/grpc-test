#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <chrono>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  std::string SayHello(const std::string& user) {
    // request
    HelloRequest request;
    request.set_name(user);

    // response
    HelloReply reply;

    // client context
    ClientContext context;

    // rpc 호출
    Status status = stub_->SayHello(&context, request, &reply);

    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
  auto startup = std::chrono::steady_clock::now();
  std::ofstream log_file("./out.txt", std::ios::app | std::ios::out);

  if(argc < 3) {
    std::cout << "require more than 2 arguments." << std::endl;
    std::cout << "./client <count> <sleep_us>" << std::endl;
    return -1;
  }

  std::string socket = "unix:/tmp/helloworld.sock";
  GreeterClient greeter(
      grpc::CreateChannel(socket, grpc::InsecureChannelCredentials()));

  // test 반복 인자 입력
  int count = std::stoi(argv[1]);
  int sleep_us = std::stoi(argv[2]);

  for(int i=0;i<count;i++) {
    auto start = std::chrono::steady_clock::now();
  
    std::string user = "world" + std::to_string(i);
    std::string reply = greeter.SayHello(user);

    auto end = std::chrono::steady_clock::now();
    log_file << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
  }
  std::cout <<  std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startup).count() << std::endl;

  return 0;
}
