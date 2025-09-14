#include <vector>

namespace compressor {
    class ICompressor {
        public: 
            virtual ~ICompressor() {};
            virtual void compress(std::vector<int>) = 0;
    };

    class SimpleCompressor final : public ICompressor {
        public: 
            SimpleCompressor();
            ~SimpleCompressor();

            void compress(std::vector<int>);
    };

}