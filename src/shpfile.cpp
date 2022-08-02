#include "shpfile.h"

CLASSES_START

shpfile::shpfile(const std::string& filename) :shpfile()
{
    load(filename);
}

void shpfile::clear()
{
    _frameheaders.clear();
    _pixels.clear();
}

bool shpfile::is_loaded()
{
    return !_frameheaders.empty() && !_pixels.empty();
}

bool shpfile::load(const std::string& filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file)
        return false;

    clear();

    std::vector<byte> buffer;

    size_t filesize = static_cast<size_t>(file.seekg(0, std::ios::end).tellg());
    file.seekg(0, std::ios::beg);
    buffer.resize(filesize);

    file.read(reinterpret_cast<char*>(buffer.data()), filesize);
    memcpy_s(&_fileheader, sizeof _fileheader, buffer.data(), sizeof _fileheader);

    _frameheaders.resize(frame_count());
    memcpy_s(_frameheaders.data(), frame_count() * sizeof shp_frame_header, &buffer[sizeof _fileheader], frame_count() * sizeof shp_frame_header);

    uint32_t pixels_offset = static_cast<uint32_t>(sizeof _fileheader + frame_count() * sizeof shp_frame_header);
    size_t pixels_size = buffer.size() - pixels_offset;

    _pixels.resize(pixels_size);
    memcpy_s(_pixels.data(), pixels_size, &buffer[pixels_offset], pixels_size);

    file.close();
    return true;
}

size_t shpfile::frame_count()
{
    return _fileheader.frames;
}

byte* shpfile::pixel_data(size_t index)
{
    if (index >= _frameheaders.size())
        return nullptr;

    shp_frame_header& header = _frameheaders[index];
    uint32_t offset = static_cast<uint32_t>(header.data_offset - sizeof _fileheader - _frameheaders.size() * sizeof shp_frame_header);

    return &_pixels[offset];
}

rectangle shpfile::frame_bound(size_t index)
{
    rectangle temp{ 0,0,0,0 };

    if (index >= _frameheaders.size())
        return temp;

    shp_frame_header& frame_data = _frameheaders[index];
    temp = { frame_data.x,frame_data.y,frame_data.width,frame_data.height };

    return temp;
}

rectangle shpfile::file_bound()
{
    return rectangle{ 0,0,_fileheader.width,_fileheader.height };
}

bool shpfile::color_replace(const std::vector<byte>& replace_scheme)
{
    const size_t valid_replace_count = 256;

    if (!is_loaded() || replace_scheme.size() != valid_replace_count)
        return false;

    for (size_t i = 0; i < frame_count(); i++)
    {
        byte* colors = pixel_data(i);
        shp_frame_header& header = _frameheaders[i];
        if (!colors)
            continue;

        size_t width = header.width;
        size_t height = header.height;
        bool has_compression = header.flags & 2u;

        if (has_compression)
        {
            for (size_t l = 0; l < height; l++)
            {
                byte* current = colors + sizeof uint16_t;
                uint16_t pitch = *reinterpret_cast<uint16_t*>(colors);

                while (current != colors + pitch)
                {
                    if (*current)
                    {
                        *current = replace_scheme[*current];
                        current++;
                    }
                    else
                    {
                        current += 2;
                    }
                }
                colors += pitch;
            }
        }
        else
        {
            for (size_t l = 0; l < height; l++)
            {
                for (size_t x = 0; x < width; x++)
                    colors[l * width + x] = replace_scheme[colors[l * width + x]];
            }
        }
    }
    return true;
}

size_t shpfile::calculate_file_size()
{
    if (!is_loaded())
        return 0;
    return sizeof _fileheader + frame_count() * sizeof shp_frame_header + _pixels.size();
}

bool shpfile::save(const std::string& filename)
{
    if (!is_loaded())
        return false;

    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file)
        return false;

    file.write(reinterpret_cast<char*>(&_fileheader), sizeof _fileheader);
    file.write(reinterpret_cast<char*>(_frameheaders.data()), _frameheaders.size() * sizeof shp_frame_header);
    file.write(reinterpret_cast<char*>(_pixels.data()), _pixels.size());

    file.close();
    return true;
}

CLASSES_END