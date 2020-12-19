
#ifndef WEBSERV_LOGGER_HPP
#define WEBSERV_LOGGER_HPP

#include <iostream>
#include <ctime>
#include <vector>
#include "time.h"

enum LogType
{
	INFO,
	WARNING,
	ERROR,
	SUCCESS
};

class NoPrintClass {
public:
	NoPrintClass(int x) {
		(void)x;
	}

	friend std::ostream &operator<<(std::ostream &os, const NoPrintClass &aClass)
	{
		(void)aClass;
		return os;
	}
};

#define NO_PRINT_CLASS NoPrintClass(0)

class Logger
{

private:
	// si vrai on ne print rien, autrement on print les logs
	bool silent;

public:
	Logger(bool silent = false)
	{
		this->silent = silent;
	}

	~Logger()
	{}

	Logger &operator=(const Logger &logger)
	{
		this->silent = logger.silent;
		return (*this);
	}

	Logger(const Logger &logger)
	{
		this->silent = logger.silent;
	}

	template<class Cls>
	void info(const std::string &message, Cls print_class)
	{
		this->message(INFO, message, print_class);
	}

	template<class Cls>
	void warning(const std::string &message, Cls print_class)
	{
		this->message(WARNING, message, print_class);
	}

	template<class Cls>
	void error(const std::string &message, Cls print_class)
	{
		this->message(ERROR, message, print_class);
	}

	template<class Cls>
	void success(const std::string &message, Cls print_class)
	{
		this->message(SUCCESS, message, print_class);
	}

	template<class Cls>
	void message(LogType type, const std::string &message, Cls print_class)
	{
		if (this->silent)
		{
			return;
		}

		std::string prefix    = this->get_prefix(type);
		std::string timestamp = this->get_current_timestamp();
		std::cout << "\033[30m" << timestamp + " " + prefix;
		if (dynamic_cast<NoPrintClass *>(&print_class)) {
			std::cout << " ";
		} else {
			std::cout << " " << print_class;
		}
		std::cout << message << "\033[0m" << std::endl;
	}

private:

	/**
	 * Get the current timestamp
	 * @return
	 */
	std::string get_current_timestamp()
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

	std::string get_prefix(LogType type)
	{
		if (type == INFO)
		{
			return "\033[30m[INFO]";
		}
		else if (type == WARNING)
		{
			return "\033[1m\033[33m[WARNING]";
		}
		else if (type == SUCCESS)
		{
			return "\033[32m[SUCCESS]";
		}
		else if (type == ERROR)
		{
			return "\033[31m[ERROR]";
		}
		else
		{
			return "";
		}
	}

};
extern Logger logger;

#endif
