#include "compressor.hpp"

compressor::B2ZlibCompressor::B2ZlibCompressor() 
{ 
    using namespace boost::iostreams;

    params.level        = zlib::best_speed;
    params.mem_level    = 8;
    params.method       = zlib::deflated;
    params.window_bits  = 15;
    params.strategy     = zlib::deflated;
    params.noheader     = false;
}

compressor::B2ZlibCompressor::B2ZlibCompressor(boost::iostreams::zlib_params params)
    : params(std::move(params)) { }

compressor::B2ZlibCompressor::~B2ZlibCompressor() { }

std::vector<char> 
compressor::B2ZlibCompressor::compress(const std::vector<char> &data) const
{
    namespace bio = boost::iostreams;

    bio::filtering_streambuf<bio::input> out;
    out.push(bio::zlib_compressor(params));
    out.push(bio::array_source(data.data(), data.size()));

    std::stringstream compressed;
    bio::copy(out, compressed);
    
    const auto& str = compressed.str();
    return std::vector<char>(str.begin(), str.end());
}

std::string
compressor::B2ZlibCompressor::compress(const std::string& data) const 
{
    std::vector<char> input {data.begin(), data.end()};
    std::vector<char> out = compress(input);
    return std::string {out.begin(), out.end()};
}