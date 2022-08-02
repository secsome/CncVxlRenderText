#pragma once

#include "definitions.h"

CLASSES_START

class logger
{
public:
	static logger instance;
	
	static bool open_log(const std::string& filename);
	static void write_line(const char* string);
	static bool prepare_log();
	static bool logfile_opened();
	static void close_log();
	
	template<typename... TArgs>
	static logger& log(const char* format, TArgs&&... args)
	{
		instance.log_write(format, std::forward<TArgs>(args)...);
		return instance;
	}

	logger() = default;
	~logger() = default;

	bool open(const std::string& filename);
	void close();
	bool is_opened();

	void log_write(const char* format, ...);
	logger& operator<<(const char* string);

private:
	std::fstream _logfile;
};
CLASSES_END