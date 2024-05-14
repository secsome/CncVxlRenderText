#pragma once

#include "definitions.h"

CLASSES_START

enum ramp :char
{
	Plane,
	NW, NE, SE, SW,
	N, E, S, W,
	NH, EH, SH, WH,
	DmN, DmE, DmS, DmW,
	DnWE, UpWE, DnNS, UpNS
};

struct tmp_file_header
{
	uint32_t xblocks;
	uint32_t yblocks;
	uint32_t block_width;
	uint32_t block_height;
};

struct tmp_image_header
{
	int32_t x;
	int32_t y;
	uint32_t _reserved_1[3];
	int32_t x_extra;
	int32_t y_extra;
	uint32_t ex_width;
	uint32_t ex_height;
	uint32_t ex_flags;
	byte height;
	byte type;
	ramp ramp_type;
	byte _reserved_2[9];
	std::vector<byte> pixels;
};

class tmpfile
{
public:
	tmpfile() = default;
	tmpfile(const std::string& filename);
	~tmpfile() = default;

	//load and clear
	void clear();
	bool is_loaded();
	bool load(const std::string& filename);

	//data accessing
	size_t block_count();
	size_t valid_block_count();
	size_t tile_size();
	byte* color_data(size_t index);
	byte* zbuffer_data(size_t index);
	bool has_extra(size_t index);
	byte* extra_data(size_t index);
	byte* extra_zbuffer(size_t index);
	size_t extra_size(size_t index);

	//data modifier
	bool color_replace(const std::vector<byte>& replace_scheme);

	//save 
	size_t calculate_file_size();
	bool save(const std::string& filename);

private:
	tmp_file_header _fileheader{ 0 };
	std::vector<tmp_image_header> _imageheaders;
	std::vector<uint32_t> _original_offsets;
};

CLASSES_END;