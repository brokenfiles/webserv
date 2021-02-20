#ifndef UTILS_HPP
# define UTILS_HPP

namespace Utils {
	std::vector<std::string> explode(const std::string& s, const std::string& charset);
	std::string removeWhitespaces(const std::string &string);
	std::string decodeBase64(const std::string &token);
}
#endif