#pragma once

#include "definitions.h"

CLASSES_START

class screen_buffer
{
public:
	using element_type = uint16_t;
	using buffer_pointer = std::shared_ptr<element_type[]>;

	static screen_buffer zbuffer;
	static screen_buffer abuffer;

	screen_buffer() = default;
	~screen_buffer() = default;
	screen_buffer(const int32_t x, const int32_t y, const size_t width, const size_t height);

	bool allocated();
	bool allocate(const int32_t x, const int32_t y, const size_t width, const size_t height);
	void free();
	bool beyond(const int32_t x, const int32_t y);
	bool fill(const element_type& value = (element_type)0);
	rectangle bound();
	size_t width();
	size_t height();
	size_t buffer_size();
	element_type* at(const int32_t x, const int32_t y);
	element_type* operator[](const int32_t y);
	
private:
	rectangle _bound;
	buffer_pointer _start;
	size_t _buffer_size = 0;
};

CLASSES_END