#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <iostream>
#include <memory>
#include <string>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

// gRPC 내용 구현
class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    // 간단한 Hello World 처리
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
};

void RunServer(uint16_t port) {
  std::string server_address = "0.0.0.0:" + std::to_string(port);
  GreeterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);

  ServerBuilder builder;

  // 인증 메커니즘은 없게 설정
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

  // 통신할 인스턴스로 "service"를 등록
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer(8080);
  return 0;
}
