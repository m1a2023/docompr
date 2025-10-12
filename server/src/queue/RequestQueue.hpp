#include <queue>
#include <string>
#include <vector>

#include "protos/file.pb.h"
#include "protos/file.grpc.pb.h"

namespace requests {

enum class RequestType: int {
    META = 1,
    CHUNK,
    FULL
} _type;

inline constexpr RequestType
operator^(RequestType a, RequestType b);

class Request {

public:
    Request(
        const std::string       &uuid,
        const compr::FileMeta   &meta,
        const compr::FileChunk  &chunk,
        const RequestType       type = RequestType::FULL
    );
    Request(
        const std::string       &uuid,
        const compr::FileMeta   &meta,
        const RequestType       type = RequestType::META 
    );
    Request(
        const std::string       &uuid,
        const compr::FileChunk  &chunk,
        const RequestType       type = RequestType::CHUNK 
    );

    inline const std::string&      uuid() const;
    inline const compr::FileMeta&  meta() const;
    inline const std::vector<compr::FileChunk>& chunks() const;
    inline const RequestType type() const;

    inline void add(const compr::FileMeta   &meta);
    inline void add(const compr::FileChunk  &chunk);
    template <typename Container>
    inline void add(const Container& c);

    inline bool is_ready() const;

private:
    std::string                     _uuid;
    compr::FileMeta                 _meta;
    std::vector<compr::FileChunk>   _chunks;

    RequestType         _type;
};

class RequestQueue {
public:
    static RequestQueue& getInstance();
    std::unique_ptr<Request> acquire();
    void push(std::unique_ptr<Request> req);

    inline bool is_empty() const;
    inline size_t size() const;

private: 
    std::deque<std::unique_ptr<Request>> _deque;

private:
    RequestQueue();
};

}
