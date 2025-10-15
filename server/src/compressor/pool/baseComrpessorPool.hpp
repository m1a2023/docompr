#pragma once
#include "compressorPool.hpp"
#include "../compressor.hpp"


namespace compressor { namespace pool {
    
    template <typename _ComprType, int _pool_size = POOL_SIZE>
    class BaseCompressorPool final : public ICompressorPool {
        public:
            static BaseCompressorPool& get_instance();
            std::unique_ptr<ICompressor> acquire() override;
            void release(std::unique_ptr<ICompressor> compressor) override;
            size_t size() const override;

            void resize(size_t new_size);

        private:
            BaseCompressorPool();
            BaseCompressorPool(const BaseCompressorPool&) = delete;
            BaseCompressorPool& operator=(const BaseCompressorPool&) = delete; 

        private:
            std::vector<std::unique_ptr<ICompressor>> pool_;
            mutable std::mutex mutex_;
            size_t max_size_ = POOL_SIZE;
    };
    
    template <typename _ComprType, int _pool_size>
    BaseCompressorPool<_ComprType, _pool_size>::BaseCompressorPool() {
        pool_.reserve(_pool_size); 
    }

    template <typename _ComprType, int _pool_size>
    BaseCompressorPool<_ComprType, _pool_size>& 
    BaseCompressorPool<_ComprType, _pool_size>::get_instance() {
        static BaseCompressorPool instance;
        return instance;
    }

    template <typename _ComprType, int _pool_size>
    std::unique_ptr<ICompressor> pool::BaseCompressorPool<_ComprType, _pool_size>::acquire() {
        std::lock_guard<std::mutex> lock(mutex_);

        if (!pool_.empty()) {
            auto com = pool_.back();
            pool_.pop_back();
            return std::move(com);
        }

        if (pool_.size() < max_size_) {
            return std::make_unique<_ComprType>();
        } 

        return nullptr;
    }

    template <typename _ComprType, int _pool_size>
    void BaseCompressorPool<_ComprType, _pool_size>::release(std::unique_ptr<ICompressor> compressor) {
        if (!compressor) return;

        std::lock_guard<std::mutex> lock(mutex_);
        if (pool_.size() < static_cast<size_t>(_pool_size)) {
            pool_.push_back(std::move(compressor));
        }
    }

    template <typename _ComprType, int _pool_size>
    size_t BaseCompressorPool<_ComprType, _pool_size>::size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return pool_.size();
    }

    template <typename _ComprType, int _pool_size>
    void BaseCompressorPool<_ComprType, _pool_size>::resize(size_t new_size) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (max_size_ < new_size) {
            max_size_ = new_size;
            pool_.resize(new_size);
        }
    }
} }