#include <iostream>
#include <string>

#include "gzstream.h"
#include "protos/file.grpc.pb.h"
#include "protos/file.pb.h"
#include <grpcpp/grpcpp.h>

#include "CompressorImpl.hpp"

int main(int argc, char* argv[]) {

    std::string addr{"0.0.0.0:50051"};
    Compressor compressor;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&compressor);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();

    return 0;
}