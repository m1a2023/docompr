#include "RequestQueue.hpp"

namespace requests {

inline constexpr RequestType
operator^(RequestType a, RequestType b) {
    return static_cast<RequestType> (
        static_cast<int>(a) ^ static_cast<int>(b)
    );
};


PoorRequest::PoorRequest(
    const std::string &_uuid, 
    std::unique_ptr<compr::FileMeta>  _meta,
    std::unique_ptr<compr::FileChunk> _chunk
)
    : _uuid(std::move(_uuid)), _type(RequestType::FULL), 
    _meta(std::move(_meta)), _chunk(std::move(_chunk))  
{ };


PoorRequest::PoorRequest(
    const std::string &_uuid, 
    std::unique_ptr<compr::FileMeta> _meta
) 
    : _uuid(std::move(_uuid)), _type(RequestType::META), 
    _meta(std::move(_meta)) 
{ };

PoorRequest::PoorRequest(
    const std::string &uuid, 
    std::unique_ptr<compr::FileChunk> _chunk
)
    : _uuid(std::move(_uuid)), _type(RequestType::FULL), 
        _chunk(std::move(_chunk))  
{ };


Request::Request(std::unique_ptr<PoorRequest> request)
: _uuid(std::move(request->_uuid)), _type(request->_type),
    _meta(std::move(request->_meta)) 
{ 
    _chunks.push_back(std::move(request->_chunk));
}

void Request::add(std::unique_ptr<compr::FileMeta> meta) {
    _meta = std::move(meta);
    _type = _type ^ RequestType::META;
}

void Request::add(std::unique_ptr<compr::FileChunk> chunk) {
    _chunks.push_back(std::move(chunk));

    if (_type != RequestType::CHUNK) {
        _type = _type ^ RequestType::CHUNK;
    }
}

inline const std::string&       Request::uuid() const { return _uuid; }
inline const compr::FileMeta   Request::meta() const { return *_meta; }
inline const std::vector<std::unique_ptr<compr::FileChunk>>  
Request::chunks() const { return _chunks; }
inline const RequestType Request::type() const { return _type; }
inline bool Request::is_ready() const { return _type == RequestType::FULL; }



RequestQueue& RequestQueue::get_instance() {
    RequestQueue rq;
    return rq;
}

inline bool RequestQueue::is_empty() const { return size() == 0; }
inline size_t RequestQueue::size() const { return _deque.size(); }

std::shared_ptr<Request> RequestQueue::acquire() {
    if (is_empty()) { 
        return nullptr;
    }

    auto first_uuid = _deque.front()->uuid();
    do {
        auto req = std::move(_deque.front());
        _deque.pop_front();

        if (req->is_ready()) {
            return std::make_shared<Request>(req);
        }
        
        if (first_uuid == req->uuid()) {
            return nullptr;
        }
    } while (true);
}

void RequestQueue::release(std::shared_ptr<Request> req) {
    for (auto it = _deque.begin(); it != _deque.end(); ++it) {
        if (req->uuid() == (*it)->uuid()) {
            it->release();
            _deque.erase(it);
        }
    }
}

inline observer::Subject& RequestQueue::subject() { return _subject; }

void RequestQueue::push(std::unique_ptr<PoorRequest> req) {
    for (auto r = _deque.begin(); r != _deque.end(); ++r) 
    {
        if ((*r)->uuid() == req->_uuid) 
        {
            switch (req->_type) 
            {
                case RequestType::META:
                    (*r)->add(std::move(req->_meta));
                    break;
                case RequestType::CHUNK:
                    (*r)->add(std::move(req->_chunk));
                    break;
                case RequestType::FULL:
                    _deque.push_back(
                        std::make_unique<Request>(
                            std::move(req))
                        );
                    break;
            }

            if ((*r)->is_ready()) {
                _subject.notify();
            }
        }
    }
}

} // endof requests
