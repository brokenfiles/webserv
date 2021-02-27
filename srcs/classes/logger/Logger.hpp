
#ifndef WEBSERV_LOGGER_HPP
#define WEBSERV_LOGGER_HPP

#include <iostream>
#include <ctime>
#include <vector>
#include <sstream>
#include "time.h"

#define COLOR_RESET  "\033[0m"
#define RED_TEXT     "\033[0;31m"
#define GREY_TEXT     "\033[1;30m"

enum LogType
{
	INFO,
	WARNING,
	NOTICE,
	ERROR,
	SUCCESS,
	CONNECT
};

#define DEFAULT_RETURN 0

class Logger
{

private:
	bool silent;
	bool strongSilent;

public:

	Logger (bool silentx = false, bool strongSilentx = false)
	{
		this->silent = silentx;
		this->strongSilent = strongSilentx;
	}

	~Logger ()
	{}

	Logger &operator= (const Logger &logger)
	{
		this->silent = logger.silent;
		return (*this);
	}

	Logger (const Logger &logger)
	{
		this->silent = logger.silent;
		this->strongSilent = logger.strongSilent;
	}

	void requestSilentMode ()
	{
		if (!this->isStrongSilent()) {
			std::string input;
			this->warning("Run Webserv in silent mode? : [y\\n]");
			std::getline(std::cin, input);
			bool state = !(input == "n" || input == "N");
			this->notice("SILENT MODE: " + Logger::to_string(state) );
			this->silence_mode(state);
		}
	}

	void silence_mode (bool x)
	{
		silent = x;
	}

	void strong_silence_mode (bool x)
	{
		strongSilent = x;
	}

	bool isSilent ()
	{
		return (silent);
	}

	bool isStrongSilent ()
	{
		return (strongSilent);
	}

	int info (const std::string &message, int return_value = DEFAULT_RETURN)
	{
		return this->message(INFO, message, return_value);
	}

	int warning (const std::string &message, int return_value = DEFAULT_RETURN)
	{
		return this->message(WARNING, message, return_value);
	}

	int connect (const std::string &message, int return_value = DEFAULT_RETURN)
	{
		return this->message(CONNECT, message, return_value);
	}

	int error (const std::string &message, int return_value = DEFAULT_RETURN)
	{
		return this->message(ERROR, message, return_value);
	}

	int success (const std::string &message, int return_value = DEFAULT_RETURN)
	{
		return this->message(SUCCESS, message, return_value);
	}

	int notice (const std::string &message, int return_value = DEFAULT_RETURN)
	{
		return this->message(NOTICE, message, return_value);
	}

	int message (LogType type, const std::string &message, int return_value = DEFAULT_RETURN)
	{
		if (this->strongSilent && type != SUCCESS && type != ERROR)
			return return_value;

		if (this->silent && type == NOTICE)
			return return_value;

		std::string prefix    = this->get_prefix(type);
		std::string timestamp = this->get_current_timestamp();
		std::cout << "\033[37m" << timestamp + " " + prefix;
		std::cout << " ";
		std::cout << message << "\033[0m" << std::endl;
		return (return_value);
	}

	template<class T>
	static std::string to_string (T n)
	{
		std::ostringstream convert;
		convert << n;
		return (convert.str());
	}

private:

	/**
	 * Get the current timestamp
	 * @return
	 */
	std::string get_current_timestamp ()
	{
		std::string timestamp_str;
		time_t      time1 = 0;
		char        buffer[256];
		// on déclare la référence de temps (time 0)
		time1 = time(0);
		if (time1)
		{
			// on récupère dans la structure tm le timestamp actuel
			tm *local_time = localtime(&time1);
			// on met dans buffer la date (sous le format d/m/y T)
			strftime(buffer, 256, "%d/%m/%y %T", local_time);
			// on case la char * en std::string pour pouvoir y ajouter des crochets
			timestamp_str  = buffer;
			timestamp_str.insert(timestamp_str.begin(), '[');
			timestamp_str.insert(timestamp_str.end(), ']');
		}
		return (timestamp_str);
	}

	std::string get_prefix (LogType type)
	{
		if (type == INFO)
			return "\033[33m[INFO]";
		else if (type == WARNING)
			return "\033[1m\033[33m[WARNING]";
		else if (type == SUCCESS)
			return "\033[32m[SUCCESS]";
		else if (type == NOTICE)
			return "\033[0;36m[NOTICE]";
		else if (type == ERROR)
			return "\033[31m[ERROR]";
		else if (type == CONNECT)
			return "\033[1;35m[CONNECT]";
		else
			return "";
	}

};
extern Logger logger;

#endif