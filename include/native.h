#pragma once

#include <string>

namespace Native
{

unsigned char *MapWholeFile(std::string filename, size_t *len);
void UnmapWholeFile(unsigned char *bytecode, size_t len);

};
