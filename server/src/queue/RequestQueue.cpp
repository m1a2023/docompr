#include "RequestQueue.hpp"

namespace requests {

inline constexpr RequestType
operator^(RequestType a, RequestType b) {
    return static_cast<RequestType> (
        static_cast<int>(a) ^ static_cast<int>(b)
    );
};


Request::Request(
    const std::string       &uuid,
    const compr::FileMeta   &meta,
    const compr::FileChunk  &chunk,
    const RequestType type  = RequestType::CHUNK ^ RequestType::META 
): _uuid(uuid), _type(type)
{
    add(meta);
    add(chunk);
};

Request::Request(
    const std::string       &uuid,
    const compr::FileMeta   &meta,
    const RequestType       type = RequestType::META 
): _uuid(uuid), _type(type) 
{
    add(meta);
}

Request::Request(
    const std::string       &uuid,
    const compr::FileChunk  &chunk,
    const RequestType       type = RequestType::CHUNK
): _uuid(uuid), _type(type) 
{
    add(chunk);
}

void Request::add(const compr::FileMeta &meta) {
    _meta.set_title(meta.title());
    _meta.set_path(meta.path());
    _meta.set_hash(meta.hash());

    _type = _type ^ RequestType::META;
}

void Request::add(const compr::FileChunk &chunk) {
    _chunks.push_back(chunk);

    if (_type != RequestType::CHUNK) {
        _type = _type ^ RequestType::CHUNK;
    }
}

inline const std::string&       Request::uuid() const { return _uuid; }
inline const compr::FileMeta&   Request::meta() const { return _meta; }
inline const std::vector<compr::FileChunk>&  Request::chunks() const { return _chunks; }
inline const RequestType Request::type() const { return _type; }
inline bool Request::is_ready() const { return _type == RequestType::FULL; }


RequestQueue& RequestQueue::getInstance() {
    RequestQueue rq;
    return rq;
}

inline bool RequestQueue::is_empty() const { return size() == 0; }
inline size_t RequestQueue::size() const { return _deque.size(); }

std::unique_ptr<Request> RequestQueue::acquire() {
    if (is_empty()) { 
        return nullptr;
    }

    auto first_uuid = _deque.front()->uuid();

    do {
        auto req = std::move(_deque.front());
        _deque.pop_front();

        if (req->is_ready()) {
            return req;
        }
        
        if (first_uuid == req->uuid()) {
            return nullptr;
        }
    } while (true);
}

void RequestQueue::push(std::unique_ptr<Request> req) {
    
    for (auto r = _deque.begin(); r != _deque.end(); r++) {
        if ((*r)->uuid() == req->uuid()) {
            switch (req->type()) {
                case RequestType::FULL: 
                    break;
                case RequestType::CHUNK:
                    (*r)->add(req->chunks());
                    break;
                case RequestType::META:
                    break;
            }

            return;
        }
    }

    _deque.push_back(std::move(req));
}

} // endof requests
