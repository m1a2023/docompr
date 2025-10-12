#include "CompressorImpl.hpp"


grpc::Status Compressor::CompressSmallFile(
    grpc::ServerContext*            context,
    const compr::SmallFileRequest*  request, 
    compr::FileResponse*            response
) {
    if (!Compress(request->meta(), request->chunk())) {
        response->set_code(500);
        response->set_message("Could not compress object");
        
        return grpc::Status::CANCELLED;
    }   

    response->set_code(200);
    response->set_message(
        "Got file: " 
        + request->meta().path() 
        + request->meta().title()
    );
    
    return grpc::Status::OK;
}

bool Compressor::Compress(
    const compr::FileMeta&  meta, 
    const compr::FileChunk& chunk
) const {
    using namespace compressor;
    using namespace std;

    B2ZlibCompressor compr;

    try {
        string com = chunk.data();
        const auto compressed = compr.compress(com);

        auto new_title = "com_" + meta.title();
        std::ofstream os{meta.path() + new_title};
        os.seekp(chunk.offset());

        os.write(compressed.data(), compressed.length());
    } catch (const std::exception& e) {
        cout << e.what() << endl;
    }

    return true;
}