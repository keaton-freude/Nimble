#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <sstream>

using std::ostream;
using std::streambuf;
using std::unique_ptr;
using std::string;
using std::cout;
using std::endl;
using std::stringstream;

enum SeverityType
{
	DEBUG = 1,
	ERR,
	WARNING,
	INFO
};

class Logger
{
public:
	static Logger& GetInstance();

	template<SeverityType severity, typename...Args>
	inline void Print(Args...args)
	{
		//mutex lock needed?
		switch (severity)
		{
		case SeverityType::DEBUG:
			_log_stream << "[DEBUG] - ";
			break;
		case SeverityType::ERR:
			_log_stream << "[ERROR] - ";
			break;
		case SeverityType::WARNING:
			_log_stream << "[WARNING] - ";
			break;
		case SeverityType::INFO:
			_log_stream << "[INFO] - ";
			break;
		default:
			break;
		}
		Print_Impl(args...);
	}

	inline ~Logger() { _out_stream.release(); }
private:
	static unique_ptr<Logger> _instance;
	unique_ptr<ostream> _out_stream;
	stringstream _log_stream;
		
	Logger();
	void Write(const string& msg);
	void Print_Impl();

	template<typename First, typename...Rest>
	inline void Print_Impl(First param1, Rest...params)
	{
		_log_stream << param1;
		Print_Impl(params...);
	}
};

// Start with assumption Logging is always enabled (opt-out)
#ifndef LOGGING_DISABLED
	#define LOG_DEBUG Logger::GetInstance().Print<SeverityType::DEBUG>
	#define LOG_ERROR Logger::GetInstance().Print<SeverityType::ERR>
	#define LOG_WARNING Logger::GetInstance().Print<SeverityType::WARNING>
	#define LOG_INFO Logger::GetInstance().Print<SeverityType::INFO>
#else
	#define LOG_DEBUG(...)
	#define LOG_ERROR(...)
	#define LOG_WARNING(...)
	#define LOG_INFO(...)
#endif