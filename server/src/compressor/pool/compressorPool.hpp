#pragma once
#include <vector>
#include <memory>
#include <mutex>

#include "../compressor.hpp"

namespace compressor { namespace pool {
/**
 * Default pool size
 */
constexpr size_t POOL_SIZE = 10;

/**
 * Interface ICompressorPool
 * Inherited interface whose implementation 
 * must be a singleton
 */
class ICompressorPool {
    public: 
        virtual ~ICompressorPool() = default;
        virtual std::shared_ptr<ICompressor> acquire() = 0;
        virtual void release(std::shared_ptr<ICompressor> compressor) = 0;
        virtual size_t size() const = 0;
};
} // endof pool 
}  // endof compressor