#include "vplfile.h"

CLASSES_START

vplfile vplfile::global;

bool vplfile::load_global(const std::string& filename)
{
	return global.load(filename);
}

bool vplfile::load_global(const byte* buffer)
{
	return global.load(buffer);
}

vplfile::vplfile(const std::string& filename)
{
	load(filename);
}

vplfile::vplfile(const byte* buffer)
{
	load(buffer);
}

void vplfile::clear()
{
	_sections.reset();
	memset(&_internal_palette, 0, sizeof(_internal_palette));
}

bool vplfile::load(const std::string& filename)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file)
		return false;

	size_t filesize = static_cast<size_t>(file.seekg(0, std::ios::end).tellg());
	file.seekg(0, std::ios::beg);

	std::vector<byte> buffer(filesize);
	file.read(reinterpret_cast<char*>(buffer.data()), filesize);

	return load(buffer.data());
}

bool vplfile::load(const byte* buffer)
{
	if (!buffer)
		return false;

	clear();

	memcpy(&_fileheader, buffer, sizeof(_fileheader));
	memcpy(&_internal_palette, buffer + sizeof(_fileheader), sizeof(_internal_palette));

	for (color& color : _internal_palette)
	{
		color.r <<= 2;
		color.g <<= 2;
		color.b <<= 2;
	}

	_sections.reset(new byte[_fileheader.section_count][256]);
	memcpy(_sections.get(), buffer + sizeof(_fileheader) + sizeof(_internal_palette), _fileheader.section_count * sizeof(vpl_section));
	
	return true;
}

bool vplfile::is_loaded()
{
	return _sections.operator bool();
}

vplfile::vpl_section& vplfile::operator[](size_t index)
{
	// assert(index < _fileheader.section_count);
	return _sections[index];
}

CLASSES_END