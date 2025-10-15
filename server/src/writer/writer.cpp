#include "writer.hpp"

TempFileHandler::TempFileHandler(const std::string &filename) {
    using namespace std;
    _stream.open(filename, ios::binary | ios::app);
}

TempFileHandler::~TempFileHandler()
{
    _stream.flush();
    _stream.close();
}

void TempFileHandler::write(const char *data, size_t size) {
    _stream.write(data, size);
}
