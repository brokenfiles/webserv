
#ifndef WEBSERV_LOGGER_HPP
#define WEBSERV_LOGGER_HPP

#include <iostream>
#include <ctime>
#include <vector>
#include <sstream>
#include "time.h"

enum LogType
{
	INFO,
	WARNING,
	NOTICE,
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
#define DEFAULT_RETURN 0

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
	int info(const std::string &message, Cls print_class, int return_value = DEFAULT_RETURN)
	{
		return this->message(INFO, message, print_class, return_value);
	}

	template<class Cls>
	int warning(const std::string &message, Cls print_class, int return_value = DEFAULT_RETURN)
	{
		return this->message(WARNING, message, print_class, return_value);
	}

	template<class Cls>
	int error(const std::string &message, Cls print_class, int return_value = DEFAULT_RETURN)
	{
		return this->message(ERROR, message, print_class, return_value);
	}

	template<class Cls>
	int success(const std::string &message, Cls print_class, int return_value = DEFAULT_RETURN)
	{
		return this->message(SUCCESS, message, print_class, return_value);
	}

	template<class Cls>
	int notice(const std::string &message, Cls print_class, int return_value = DEFAULT_RETURN)
	{
		return this->message(NOTICE, message, print_class, return_value);
	}

	template<class Cls>
	int message(LogType type, const std::string &message, Cls print_class, int return_value = DEFAULT_RETURN)
	{
		if (this->silent)
		{
			return return_value;
		}

		std::string prefix    = this->get_prefix(type);
		std::string timestamp = this->get_current_timestamp();
		std::cout << "\033[37m" << timestamp + " " + prefix;
		Cls *pointer = &print_class;
		if (dynamic_cast<NoPrintClass *>(pointer)) {
			std::cout << " ";
		} else {
			std::cout << " " << print_class;
		}
		std::cout << message << "\033[0m" << std::endl;
		return (return_value);
	}

	template <class T>
	static std::string to_string(T n)
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
			return "\033[33m[INFO]";
		}
		else if (type == WARNING)
		{
			return "\033[1m\033[33m[WARNING]";
		}
		else if (type == SUCCESS)
		{
			return "\033[32m[SUCCESS]";
		}
		else if (type == NOTICE)
		{
			return "\033[0;36m[NOTICE]";
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
