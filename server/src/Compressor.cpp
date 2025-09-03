#include "Compressor.hpp"


grpc::Status Compressor::CompressSmallFile(grpc::ServerContext* context,
    const compr::SmallFileRequest* request, compr::FileResponse* response) {
    
    
}

bool Compressor::Compress(const compr::FileMeta& meta, const compr::FileChunk& chunk) const {
    constexpr auto _SIZE = 8192; 
    auto full_path = meta.path() + meta.title();
    
    try {
        auto compress = ogzstream { full_path.c_str() };

        for (auto i = 0; i < chunk.data().length(); i += _SIZE) {
            auto data = chunk.data().substr(i, _SIZE);
            compress.write(data.c_str(), _SIZE);
        }
    
        compress.close();
        return true;
    } catch (std::exception) {
        return false;
    }
}

void Compressor::WriteFile(const compr::FileMeta& meta, const compr::FileChunk& chunk) const 
{
    constexpr int _SIZE = 2048; 

    const auto full_path = meta.path() + meta.title();
    auto file = std::fstream(full_path, std::ios::out | std::ios::binary);

    for (auto i = 0; i < chunk.data().length(); i += _SIZE) {
        auto c = chunk.data().substr(i, _SIZE);
        file.write(c.c_str(), _SIZE);
    }

    file.close();
}