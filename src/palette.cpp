#include "palette.h"

CLASSES_START

palette::palette(const std::string& filename)
{
    load(filename);
}

bool palette::load(const std::string& filename)
{
    const size_t valid_color_count = 256;
    constexpr size_t valid_palsize = valid_color_count * sizeof color;

    clear();

    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file)
        return false;

    size_t filesize = static_cast<size_t>(file.seekg(0, std::ios::end).tellg());
    file.seekg(0, std::ios::beg);
    if (filesize != valid_palsize)
        return false;

    _entries.resize(valid_color_count);
    file.read(reinterpret_cast<char*>(_entries.data()), valid_palsize);
    for (color& color : _entries)
    {
        color.r <<= 2;
        color.g <<= 2;
        color.b <<= 2;
    }

    file.close();
    return true;
}

void palette::clear()
{
    _entries.clear();
}

bool palette::is_loaded()
{
    return !_entries.empty();
}

std::vector<byte> palette::convert_color(palette& target)
{
    const size_t valid_color_count = 256;
    std::vector<byte> convert_table;

    if (!is_loaded() || !target.is_loaded())
        return convert_table;

    convert_table.resize(valid_color_count);
    auto distance_squared = [](color& left, color& right)->size_t
    {
        size_t dis_r = right.r - left.r;
        size_t dis_g = right.g - left.g;
        size_t dis_b = right.b - left.b;
        return dis_r * dis_r + dis_g * dis_g + dis_b * dis_b;
    };

    auto find_nearest_color = [valid_color_count, &target, &distance_squared](color& color)->size_t
    {
        size_t nearest_index = 0;
        size_t nearest_distance = distance_squared(color, target[0]);
        for (size_t i = 1; i < valid_color_count; i++)
        {
            size_t distance = distance_squared(color, target[i]);
            if (distance < nearest_distance)
            {
                nearest_distance = distance;
                nearest_index = i;
            }
        }

        return nearest_index;
    };

    for (size_t i = 1; i < valid_color_count; i++)
        convert_table[i] = static_cast<byte>(find_nearest_color(_entries[i]));

    convert_table[0] = 0;
    return convert_table;
}

color& palette::operator[](const size_t index)
{
    assert(index < _entries.size());
    return _entries[index];
}

CLASSES_END