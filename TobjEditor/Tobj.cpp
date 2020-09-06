#include "tobj.h"
#include <string>

bool Tobj::Load(const std::string& tobjPath) {
    std::ifstream input(tobjPath, std::ios::binary);
    //We will not attempt to read from a file if any of the stream's errorflags is set 
    if (!input.good())
        return false;

    input.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.PathLen > 0) {
        const uint32_t pos = offsetof(TobjHeader, Unknown_DW6) + sizeof(uint32_t);
        input.seekg(pos);
        std::getline(input, texturePath);
    }
    return !texturePath.empty();
}

bool Tobj::Save(const std::string& tobjPath, const std::string& texturePath) {
    fullPath = tobjPath;
    //Set the PathLen struct member to the length of the texturepath
    header.PathLen = (texturePath.length());

    std::ofstream output(tobjPath, std::ios::binary);

    //We will not attempt to write to a file if any of the stream's errorflags is set 
    if (output.good()) {
        const uint32_t length = sizeof(TobjHeader);
        uint8_t buffer[length];
        memcpy(buffer, &header, length); //fill the buffer

        output.write(reinterpret_cast<const char*>(buffer), length);
        output << texturePath;
        return true;
    }
    return false;
}


