#pragma once
#include <fstream>
#include <cstdint>

struct TobjHeader {
	uint32_t    Version{ 0x70B10A01 }; //Same magic number on all valid tobj files
	uint32_t	unknown_DW1;
	uint32_t	unknown_DW2;
	uint32_t	unknown_DW3;
	uint32_t	unknown_DW4;
	uint16_t	uknown_short1;
	uint8_t		Bias;
	uint8_t		unknown_byte1;
	uint8_t		Mapping;
	uint8_t		unknown_byte2;
	uint8_t		MagFilter;
	uint8_t		MinFilter;
	uint8_t		MipFilter;
	uint8_t		unknown_byte3;
	uint8_t		WrapModeU;
	uint8_t		WrapModeV;
	uint8_t		WrapModeW;
	uint8_t		Compress;
	uint8_t		unknown_byte4;
	uint8_t		Anisotropic;
	uint8_t		unknown_byte5;
	uint8_t		unknown_byte6;
	uint8_t		ColorSpace;
	uint8_t		unknown_byte7;
	uint32_t	PathLen{ 0 };
	uint32_t	Unknown_DW6;
};

class Tobj {
public:
	bool Load(const std::string& tobjPath);
	bool Save(const std::string& tobjPath, const std::string& texturePath);

	//Getters
	std::string GetTexturePath() const { return texturePath; }
	std::string GetFullPath() const {return fullPath;}

private:
	std::string fullPath;
	std::string texturePath;
	struct TobjHeader header;
};

