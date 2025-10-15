#pragma once
#include <iostream>
#include <string>
#include <fstream>


class TempFileHandler {
public:
    TempFileHandler(const std::string &filename);
    ~TempFileHandler();

    template <typename Container>
    void write(Container container) {
        _stream.write(container.data(), container.size());
    };
   
    void write(const char* data, size_t size);

private:
    std::fstream _stream;

};