#include <iostream>
#include <string>

#include "gzstream.h"
#include "protos/file.grpc.pb.h"
#include "protos/file.pb.h"
#include <grpcpp/grpcpp.h>

#include "CompressorImpl.hpp"
#include "queue/RequestQueue.hpp"
#include "observer/observer.hpp"

int main(int argc, char* argv[]) {

    { // Linking observer and subject
    auto &_queue = requests::RequestQueue::get_instance();
    auto obs = std::make_shared<observer::QueueObserver>();
    _queue.subject().attach(obs.get());
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