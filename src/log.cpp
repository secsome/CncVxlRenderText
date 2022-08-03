#include "log.h"

CLASSES_START

logger logger::instance;

bool logger::open_log(const std::string& filename)
{
    instance.open(filename);

    return logfile_opened();
}

void logger::write_line(const char* string)
{
    instance.log("%s\n", string);
}

bool logger::prepare_log()
{
    char filename[0x100]{ 0 };

    if (!std::filesystem::exists(".\\dlldebug"))
    {
        if (!std::filesystem::create_directory(".\\dlldebug"))
            return false;
    }

    time_t rawtime;
    tm info;
    time(&rawtime);
    info = *localtime(&rawtime);
    snprintf(filename, sizeof(filename), ".\\dlldebug\\Debug-%04u%02u%02u-%02u%02u%02u.LOG",
        1900 + info.tm_year, 1 + info.tm_mon, info.tm_mday, info.tm_hour, info.tm_min, info.tm_sec);

    open_log(filename);

    if (logfile_opened())
    {
        instance << "*** Cnc Image Library Debug Log ***"
            << "*** Author : ThomasPusheen ***"
            << "*** Sent to gual000@163.com when issues occur ***";

        return true;
    }

    return false;
}

bool logger::logfile_opened()
{
    return instance.is_opened();
}

void logger::close_log()
{
    instance.close();
}

bool logger::open(const std::string& filename)
{
    _logfile.open(filename, std::ios::out | std::ios::trunc);

    return is_opened();
}

void logger::close()
{
    _logfile.close();
}

bool logger::is_opened()
{
    return _logfile.is_open();
}

void logger::log_write(const char* format, ...)
{
#if 0
    char buffer[0x100]{ 0 };

    va_list arguments;
    
    va_start(arguments, format);
    vsprintf(buffer, format, arguments);
    va_end(arguments);

    _logfile.write(buffer, strlen(buffer));
    _logfile.flush();
#endif
}

logger& logger::operator<<(const char* string)
{
    _logfile << string << std::endl;
    return *this;
}

CLASSES_END