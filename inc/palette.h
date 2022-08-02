#pragma once

#include "definitions.h"

CLASSES_START

class palette
{
public:
	palette() = default;
	palette(const std::string& filename);
	~palette() = default;

	bool load(const std::string& filename);
	void clear();
	bool is_loaded();
	std::vector<byte> convert_color(palette& target);
	color& operator[](const size_t index);

private:
	std::vector<color> _entries;
};

CLASSES_END