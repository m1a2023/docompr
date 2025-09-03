#include "gzstream.h"
#include "protos/file.grpc.pb.h"
#include "protos/file.pb.h"
#include <grpcpp/grpcpp.h>

#include <string>

class Compressor final : public compr::Compressor::Service {

public:
    grpc::Status CompressSmallFile(grpc::ServerContext* context,
        const compr::SmallFileRequest* request, compr::FileResponse* response);
    
    grpc::Status CompressLargeFile(grpc::ServerContext* context,
        const grpc::ServerReader<compr::FileRequest>* reader, compr::FileResponse* response);

private:
    void WriteFile(const compr::FileMeta& meta, const compr::FileChunk& chunk) const;	
	bool Compress(const compr::FileMeta& meta, const compr::FileChunk& chunk) const;

	inline ogzstream* open_file() const;
};
