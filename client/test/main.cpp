#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <array>

#include "gzstream.h"
#include "protos/file.grpc.pb.h"
#include "protos/file.pb.h"
#include <grpcpp/grpcpp.h>

int main(int argc, char** argv) {
    using namespace std;
    namespace fs = filesystem;

    if (argc < 3) exit(1);

    string path{argv[1]}, name{argv[2]}, hash = "11111";
    auto filepath = fs::absolute(fs::path(path + "/" + name));
    if (!fs::exists(filepath)) {
        cout << "File doesn't exist" << endl;
    }
     
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    std::unique_ptr<compr::Compressor::Stub> stub = compr::Compressor::NewStub(channel);

    constexpr auto READ_SIZE = 256 * 256 * 128;
    array<char, READ_SIZE> data;
   
    std::ifstream ifs {filepath, std::ios::binary};
    
    if (!ifs) {
        std::cout << "error file opening" << std::endl;
        std::cout << "Filepath: " << filepath << std::endl;
    }

    auto filesize = fs::directory_entry(filepath).file_size();
    {
        for (auto i = 0; i < filesize; i += READ_SIZE) 
        {
            ifs.read(data.data(), data.size());

            auto meta = new compr::FileMeta; 
            {
                meta->set_title(name);
                meta->set_path(path);
                meta->set_hash(hash);
            }
            
            auto chunk = new compr::FileChunk; 
            {
                chunk->set_offset(i);
                auto bytes_read = ifs.gcount();
                chunk->set_data(data.data(), bytes_read);
            }

            compr::SmallFileRequest req;
            {
                req.set_allocated_meta(meta);
                req.set_allocated_chunk(chunk);
            }

            compr::FileResponse res;
            grpc::ClientContext context;
            grpc::Status status = stub->CompressSmallFile(&context, req, &res);
            {
                cout << "code: " << res.code() << endl
                    << "msg: " << res.message() << endl;
            }
        }
    }
   
    return 0;
}