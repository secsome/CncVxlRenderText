#pragma once
#include "definitions.h"

CLASSES_START

#pragma pack(push, 4)
struct vpl_header
{
	uint32_t remap_start;
	uint32_t remap_end;
	uint32_t section_count;
	uint32_t _reserved;
};
#pragma pack(pop)

class vplfile
{
public:
	using vpl_section = byte[256];
	using vpl_pointer = byte[][256];
	using vpl_section_table = std::shared_ptr<vpl_pointer>;

	static vplfile global;
	static bool load_global(const std::string& filename);
	static bool load_global(const byte* buffer);

	vplfile() = default;
	~vplfile() = default;
	vplfile(const std::string& filename);
	vplfile(const byte* buffer);

	void clear();
	bool load(const std::string& filename);
	bool load(const byte* buffer);
	bool is_loaded();
	vpl_section& operator[](size_t index);
	

private:
	vpl_header _fileheader;
	color _internal_palette[256];
	vpl_section_table _sections;
};

CLASSES_END