
namespace compressor {
    namespace pool {
        class ICompressorPool {
            public: 
                ICompressorPool() = delete;
                static ICompressorPool getInstance();
        };

    
    }
}