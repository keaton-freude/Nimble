#include "Logger.h"

unique_ptr<Logger> Logger::_instance(new Logger());

Logger& Logger::GetInstance()
{
	if (_instance == 0)
	{
		
		_instance.reset(new Logger());
	}
	return *_instance;
}

Logger::Logger() 
	: _out_stream()
{
	
	_out_stream.reset(&cout);
}

void Logger::Write(const string& msg)
{
	(*_out_stream) << msg << endl;
}

void Logger::Print_Impl()
{
	Write(_log_stream.str());
	_log_stream.str("");
}
