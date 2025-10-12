#pragma once 
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <sstream>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

#include "io.hpp"
#include "gzstream.h"


namespace compressor {

    class ICompressor {
        public: 
            virtual ~ICompressor() {};
    };  

    class B2ZlibCompressor : public ICompressor {
        public:
            B2ZlibCompressor();
            ~B2ZlibCompressor();

            explicit B2ZlibCompressor(boost::iostreams::zlib_params params);
            
            std::vector<char> compress(const std::vector<char>& data) const;
            std::string       compress(const std::string& data)       const;

        private:
            boost::iostreams::zlib_params params;
    };
} // end compressor