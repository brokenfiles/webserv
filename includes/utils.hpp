#ifndef UTILS_HPP
# define UTILS_HPP

namespace Utils {
	std::vector<std::string> explode(const std::string& s, const std::string& charset);
	std::string removeWhitespaces(const std::string &string);
	std::string decodeBase64(const std::string &token);
	std::string generateToken(unsigned int len);
	std::string toUppercase(std::string string);
	std::string replaceAll(std::basic_string<char> string, char haystack, char needle);
	void replaceString (std::string &fileContent, const std::string &replace, const std::string &newString);
	int ft_strlen(const char *str);
	char *ft_strdup(const char *str);
}
#endif