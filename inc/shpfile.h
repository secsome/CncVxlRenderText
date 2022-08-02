#pragma once

#include "definitions.h"

CLASSES_START

struct shp_file_header
{
	uint16_t type;
	uint16_t width;
	uint16_t height;
	uint16_t frames;
};

struct shp_frame_header
{
	int16_t x;
	int16_t y;
	uint16_t width;
	uint16_t height;
	uint32_t flags;
	uint32_t color;
	uint32_t _reserved;
	uint32_t data_offset;
};

class shpfile
{
public:
	shpfile() = default;
	shpfile(const std::string& filename);
	~shpfile() = default;

	//load and clear
	void clear();
	bool is_loaded();
	bool load(const std::string& filename);

	//data accessing
	size_t frame_count();
	byte* pixel_data(size_t index);
	rectangle frame_bound(size_t index);
	rectangle file_bound();

	//data modifier
	bool color_replace(const std::vector<byte>& replace_scheme);

	//save
	size_t calculate_file_size();
	bool save(const std::string& filename);

private:
	shp_file_header _fileheader{ 0 };
	std::vector<shp_frame_header> _frameheaders;
	std::vector<byte> _pixels;
};
CLASSES_END