#include "CompressorImpl.hpp"


grpc::Status Compressor::CompressSmallFile(
    grpc::ServerContext*            context,
    const compr::SmallFileRequest*  request, 
    compr::FileResponse*            response
) {
    auto _meta = std::make_unique<compr::FileMeta>(request->meta());
    auto _chunk = std::make_unique<compr::FileChunk>(request->chunk());

    if (!Compress(request->uuid(), std::move(_meta), std::move(_chunk))) {
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
    const std::string& uuid,
    std::unique_ptr<compr::FileMeta> meta, 
    std::unique_ptr<compr::FileChunk> chunk 
) const {
    using namespace compressor;
    using namespace std;

    // B2ZlibCompressor compr;
    // try {
    //     string com = chunk.data();
    //     const auto compressed = compr.compress(com);

    //     auto new_title = "com_" + meta.title();
    //     std::ofstream os{meta.path() + new_title};
    //     os.seekp(chunk.offset());

    //     os.write(compressed.data(), compressed.length());
    // } catch (const std::exception& e) {
    //     cout << e.what() << endl;
    // }
   
    auto req = make_unique<requests::PoorRequest>(
        uuid, std::move(meta), std::move(chunk)
    );
    requests::RequestQueue::get_instance().push(std::move(req));

    return true;
}