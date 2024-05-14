#include "tmpfile.h"

CLASSES_START

tmpfile::tmpfile(const std::string& filename) :tmpfile()
{
    load(filename);
}

void tmpfile::clear()
{
    _imageheaders.clear();
    _original_offsets.clear();
}

bool tmpfile::is_loaded()
{
    return !_imageheaders.empty() && !_original_offsets.empty();
}

bool tmpfile::load(const std::string& filename)
{
    clear();

    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file)
        return false;

    std::vector<byte> buffer;

    file.seekg(0, std::ios::end);
    size_t filesize = static_cast<size_t>(file.tellg());

    buffer.resize(filesize);
    file.seekg(0, std::ios::beg);

    file.read(reinterpret_cast<char*>(buffer.data()), filesize);

    tmp_image_header temp;

    memcpy(&_fileheader, buffer.data(), sizeof(_fileheader));
    _original_offsets.resize(block_count());

    size_t current_valid_index = 0;
    memcpy(_original_offsets.data(), &buffer[sizeof(_fileheader)], block_count() * sizeof(uint32_t));
    for (size_t offset : _original_offsets)
    {
        const size_t header_size = sizeof(tmp_image_header) - sizeof(std::vector<byte>);
        if (offset)
        {
            //offset += reinterpret_cast<uint32_t>(buffer.data());
            memcpy(&temp, &buffer[offset], header_size);

            temp.pixels.resize(tile_size() * 2);
            memcpy(temp.pixels.data(), &buffer[offset + header_size], tile_size() * 2);

            _imageheaders.push_back(temp);
            if (has_extra(current_valid_index))
            {
                auto& back = _imageheaders.back();
                back.pixels.resize(tile_size() * 2 + extra_size(current_valid_index) * 2);
                memcpy(&back.pixels[tile_size() * 2], &buffer[offset + header_size + tile_size() * 2], extra_size(current_valid_index) * 2);
            }

            ++current_valid_index;
        }
    }

    file.close();
    return true;
}

size_t tmpfile::block_count()
{
    return _fileheader.xblocks * _fileheader.yblocks;
}

size_t tmpfile::valid_block_count()
{
    return _imageheaders.size();
}

size_t tmpfile::tile_size()
{
    return _fileheader.block_height * _fileheader.block_width / 2;
}

byte* tmpfile::color_data(size_t index)
{
    if (index >= _imageheaders.size())
        return nullptr;
    return _imageheaders[index].pixels.data();
}

byte* tmpfile::zbuffer_data(size_t index)
{
    if (index >= _imageheaders.size())
        return nullptr;
    return &_imageheaders[index].pixels[tile_size()];
}

bool tmpfile::has_extra(size_t index)
{
    if (index >= _imageheaders.size())
        return false;
    return _imageheaders[index].ex_flags & 1u;
}

byte* tmpfile::extra_data(size_t index)
{
    if (!has_extra(index))
        return nullptr;
    return &_imageheaders[index].pixels[tile_size() * 2];
}

byte* tmpfile::extra_zbuffer(size_t index)
{
    if (!has_extra(index))
        return nullptr;
    return &_imageheaders[index].pixels[tile_size() * 2 + extra_size(index)];
}

size_t tmpfile::extra_size(size_t index)
{
    if (!has_extra(index))
        return 0;
    return _imageheaders[index].ex_width * _imageheaders[index].ex_height;
}

bool tmpfile::color_replace(const std::vector<byte>& replace_scheme)
{
    const size_t valid_color_count = 256;
    if (!is_loaded() || replace_scheme.size() != valid_color_count)
        return false;

    for (size_t i = 0; i < valid_block_count(); i++)
    {
        byte* colors = color_data(i);
        for (size_t x = 0; x < tile_size(); x++)
            colors[x] = replace_scheme[colors[x]];

        if (byte* extras = extra_data(i))
        {
            for (size_t x = 0; x < extra_size(i); x++)
                extras[x] = replace_scheme[extras[x]];
        }
    }

    return true;
}

size_t tmpfile::calculate_file_size()
{
    if (!is_loaded())
        return 0;

    const size_t header_size = sizeof(tmp_image_header) - sizeof(std::vector<byte>);
    size_t total_size = sizeof(_fileheader) + block_count() * sizeof(uint32_t);
    for (auto& block_data : _imageheaders)
        total_size += header_size + block_data.pixels.size();

    return total_size;
}

bool tmpfile::save(const std::string& filename)
{
    if (!is_loaded())
        return false;

    std::vector<byte> filebuffer;
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file)
        return false;

    filebuffer.resize(calculate_file_size());
    const size_t image_header_size = sizeof(tmp_image_header) - sizeof(std::vector<byte>);

    memcpy(filebuffer.data(), &_fileheader, sizeof(_fileheader));
    memset(&filebuffer[sizeof(_fileheader)], 0, block_count() * sizeof(uint32_t));

    uint32_t* offsets = reinterpret_cast<uint32_t*>(&filebuffer[sizeof(_fileheader)]);
    size_t current_offset = sizeof(_fileheader) + block_count() * sizeof(uint32_t);
    size_t block_data_size = image_header_size + tile_size() * 2;

    //memset(offsets, 0, block_count() * sizeof(uint32_t));
    memcpy(offsets, _original_offsets.data(), block_count() * sizeof(uint32_t));
    for (size_t i = 0; i < valid_block_count(); i++)
    {
        auto& block_data = _imageheaders[i];
        memcpy(&filebuffer[current_offset], &block_data, image_header_size);
        memcpy(&filebuffer[current_offset + image_header_size], block_data.pixels.data(), block_data.pixels.size());

        //offsets[i] = current_offset;
        current_offset += image_header_size + block_data.pixels.size();
    }

    file.write(reinterpret_cast<char*>(filebuffer.data()), filebuffer.size());
    file.close();
    return true;
}

CLASSES_END