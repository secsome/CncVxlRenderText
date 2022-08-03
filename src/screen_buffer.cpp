#include "screen_buffer.h"

CLASSES_START

screen_buffer screen_buffer::zbuffer;
screen_buffer screen_buffer::abuffer;

screen_buffer::screen_buffer(const int32_t x, const int32_t y, const size_t width, const size_t height) :
    screen_buffer()
{
    allocate(x, y, width, height);
}

bool screen_buffer::allocated()
{
    return buffer_size() && _start;
}

bool screen_buffer::allocate(const int32_t x, const int32_t y, const size_t width, const size_t height)
{
    free();

    if (width == 0 || height == 0)
        return false;

    _start = buffer_pointer(new element_type[width * height]);
    if (_start)
    {
        _buffer_size = width * height * sizeof(element_type);
        _bound = { x,y,width,height };

        return true;
    }

    return false;
}

void screen_buffer::free()
{
    if (allocated())
    {
        _start.reset();
        _bound = rectangle();
        _buffer_size = 0;
    }
}

bool screen_buffer::beyond(const int32_t x, const int32_t y)
{
    return 
        x < bound().x || 
        x >= bound().x + static_cast<int32_t>(width()) || 
        y < bound().y || 
        y >= bound().y + static_cast<int32_t>(height());
}

bool screen_buffer::fill(const element_type& value)
{
    if (!allocated())
        return false;

    for (int32_t x = 0; x < static_cast<int32_t>(width()); x++)
    {
        for (int32_t y = 0; y < static_cast<int32_t>(height()); y++)
        {
            *at(bound().x + x, bound().y + y) = value;
        }
    }
    return true;
}

rectangle screen_buffer::bound()
{
    return _bound;
}

size_t screen_buffer::width()
{
    return _bound.width;
}

size_t screen_buffer::height()
{
    return _bound.height;
}

size_t screen_buffer::buffer_size()
{
    return _buffer_size;
}

screen_buffer::element_type* screen_buffer::at(const int32_t x, const int32_t y)
{
    if (!allocated())
        return nullptr;

    assert(!beyond(x, y));

    return &_start[(y - bound().y) * width() + (x - bound().x)];
}

screen_buffer::element_type* screen_buffer::operator[](const int32_t y)
{
    return at(bound().x, y);
}

CLASSES_END