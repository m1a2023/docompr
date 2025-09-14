#include "CompressorImpl.hpp"


grpc::Status Compressor::CompressSmallFile(grpc::ServerContext* context,
    const compr::SmallFileRequest* request, compr::FileResponse* response) {
    
    if (!Compress(request->meta(), request->chunk())) {
        response->set_code(500);
        response->set_message("Could not compress object");
        return grpc::Status::CANCELLED;
    }   

    response->set_code(200);
    auto msg = "Got file: " + request->meta().path() + request->meta().title();
    response->set_message(msg);
    return grpc::Status::OK;
}

bool Compressor::Compress(const compr::FileMeta& meta, const compr::FileChunk& chunk) const {
    constexpr auto _SIZE = 8192; 
    auto full_path = meta.path() + meta.title() + ".zip";
    
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