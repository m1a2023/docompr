#pragma once

#include <queue>
#include <string>
#include <vector>
#include <optional>

#include "protos/file.pb.h"
#include "protos/file.grpc.pb.h"

#include "../observer/observer.hpp"

namespace requests {

/**
 *  Type of request transfer information;
 */
enum class RequestType: int {
    META = 1,
    CHUNK,
    FULL
} _type;

inline constexpr RequestType
operator^(RequestType a, RequestType b);


/* 
    In general a DTO object;
    Smallest interpretation of request;
*/
class PoorRequest {
public:
    PoorRequest(
        const std::string       &_uuid,
        std::unique_ptr<compr::FileMeta>   _meta,
        std::unique_ptr<compr::FileChunk>  _chunk
    );
    PoorRequest(
        const std::string       &_uuid,
        std::unique_ptr<compr::FileMeta>   _meta
    );
    PoorRequest(
        const std::string       &_uuid,
        std::unique_ptr<compr::FileChunk>  _chunk
    );

    RequestType       _type;
    std::string       _uuid;
    std::unique_ptr<compr::FileMeta>   _meta;
    std::unique_ptr<compr::FileChunk>  _chunk;
};


/**
 *  Object that represents general request,
 * that will be processed in request queue; 
 */
class Request {

public:
    Request(std::unique_ptr<PoorRequest> request);

    inline const std::string&     uuid() const;
    inline const compr::FileMeta  meta() const;
    inline const 
    std::vector<std::unique_ptr<compr::FileChunk>> chunks() const;
    inline const RequestType type() const;

    inline void add(std::unique_ptr<compr::FileMeta> meta);
    inline void add(std::unique_ptr<compr::FileChunk> chunk);

    inline bool is_ready() const;

private:
    std::string                        _uuid;
    std::unique_ptr<compr::FileMeta>                _meta;
    std::vector<std::unique_ptr<compr::FileChunk>>  _chunks;

    RequestType         _type;
};


/**
 * represents singletone queue that keeps requests;
 * takes PoorRequest;
 * acquires Request;
 */
class RequestQueue {
public:
    static RequestQueue& get_instance();
    std::shared_ptr<Request> acquire();
    void push(std::unique_ptr<PoorRequest> req);

    inline bool is_empty() const;
    inline size_t size() const;

    void release(std::shared_ptr<Request> req);

    inline observer::Subject& subject();

private:
    RequestQueue();
    RequestQueue(const RequestQueue&) = delete;
    RequestQueue& operator=(const RequestQueue&) = delete;

private: 
    std::deque<std::unique_ptr<Request>> _deque;
    observer::Subject _subject;
};

}
