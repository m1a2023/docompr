#pragma once
#include <boost/asio.hpp>

#include <unordered_map>
#include <utility>
#include <string>

#include "../writer/writer.hpp"
#include "../compressor/compressor.hpp"

namespace processor {

class QueueProcessor {
    /*
      processing pair;
      minimal processing unit
     */
    using proc_pair = std::pair<
        std::unique_ptr<compressor::ICompressor>, 
        std::unique_ptr<TempFileHandler>
    >;

public:
    static QueueProcessor& get_instance();
    void queue_job(); 

private:
    QueueProcessor() = default;
    QueueProcessor(QueueProcessor &processor) = delete;
    QueueProcessor& operator=(QueueProcessor &processor) = delete;

private:
    /* manages threads that process pairs */ 
    boost::asio::thread_pool _pool;
    /**
     * Processor must manage internal _table;
     * 
     * CompressorWriterTable _table;
     */
    std::unordered_map<std::string, proc_pair> _table;
};

}