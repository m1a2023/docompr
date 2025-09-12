#include <iostream>
#include <string>

#include "gzstream.h"
#include "protos/file.grpc.pb.h"
#include "protos/file.pb.h"
#include <grpcpp/grpcpp.h>

#include "Compressor.hpp"

int main(int argc, char* argv[]) {

    std::cout << argc << std::endl;

    for (auto i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }

    std::string addr{"0.0.0.0:50051"};
    Compressor compressor;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&compressor);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();

    return 0;
}