#include "gzstream.h"
#include "protos/file.grpc.pb.h"
#include "protos/file.pb.h"
#include <grpcpp/grpcpp.h>

#include <string>

#include "compressor/compressor.hpp"
#include "compressor/pool/compressorPool.hpp"
#include "compressor/pool/baseComrpessorPool.hpp"

#include "queue/RequestQueue.hpp"

class Compressor final : public compr::Compressor::Service {

public:
    grpc::Status CompressSmallFile(grpc::ServerContext* context,
        const compr::SmallFileRequest* request, compr::FileResponse* response);
    
    grpc::Status CompressLargeFile(grpc::ServerContext* context,
        const grpc::ServerReader<compr::FileRequest>* reader, compr::FileResponse* response);

private:
	bool Compress(const std::string& uuid, 
        std::unique_ptr<compr::FileMeta> meta, 
        std::unique_ptr<compr::FileChunk> chunk) const;
};
