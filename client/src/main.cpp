#include <iostream>
#include <string>

#include "gzstream.h"
#include "protos/file.grpc.pb.h"
#include "protos/file.pb.h"
#include <grpcpp/grpcpp.h>

int main(int argc, char* argv[]) {
    char* FILE_NAME;
    char* FILE_PATH;

    if (argc > 1) {
        // name with extension
        FILE_NAME = argv[1];
    } if (argc > 2) {
        // full path 
        FILE_PATH = argv[2];
    } else {
        // error
        return 1;
    }
    auto FILE_HASH = "123";

    /**
     * Opening example file
     */
    std::string _data; 
    std::string path {FILE_PATH};
    std::string name {FILE_NAME};
    std::string hash {FILE_HASH};
    std::ifstream ifs {path + "/" + name, std::ios::binary};
    if (!ifs.is_open()) {
        std::cout << "error file opening" << std::endl;
        std::cout 
            << "File: " 
            << path
            << " "
            << name
            << std::endl;
    }
    /**
     * rewrite to class 
     */
    ifs.seekg(0, std::ios::end);
    _data.resize(ifs.tellg());
    ifs.seekg(0);
    ifs.read(_data.data(), _data.size());
    ifs.close();

   
    /**
     * Setup request
     */
    compr::FileMeta* meta = new compr::FileMeta; 
    {
        meta->set_title(name);
        meta->set_path(path);
        meta->set_hash(hash);
    }
    
    compr::FileChunk* chunk = new compr::FileChunk; 
    {
        chunk->set_offset(0);
        chunk->set_data(_data);
    }

    compr::SmallFileRequest req;
    {
        req.set_allocated_meta(meta);
        req.set_allocated_chunk(chunk);
    }

    compr::FileResponse res;

    /**
     * Call 
     */
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    std::unique_ptr<compr::Compressor::Stub> stub = compr::Compressor::NewStub(channel);
    grpc::ClientContext context;
    grpc::Status status = stub->CompressSmallFile(&context, req, &res);

    /**
     * Output
     */
    {
        std::cout
            << "code: "
            << res.code() 
            << std::endl
            <<"msg: "
            << res.message()
            << std::endl;
    }

    return 0;
}