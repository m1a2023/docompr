#pragma once
#include <cstdio>

namespace IO {
    class IWrite {
        public: virtual void write(const char* data, size_t size) = 0;
    };

    class IRead {
        public: virtual const char* read(size_t size) = 0;
    };

    class IOpen {
        public: virtual void open() = 0;
    };

    class IClose {
        public: virtual void close() = 0; 
    };

    class IReadWrite: IRead, IWrite { }; 
    class IOpenClose: IOpen, IClose { };
}