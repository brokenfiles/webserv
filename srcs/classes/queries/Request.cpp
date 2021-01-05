//
// Created by Timothe Lecouvreur on 18/12/2020.
//

#include "../../../includes/includes.h"
#include "Request.hpp"

std::ostream&	operator<<(std::ostream &o, const Request &q) {

	//std::map<std::string, std::string>::const_iterator	ite;

	o << q.getMethod() << " [" << q.getPath() << "]";
//	for(ite = q.getHeaders().begin(); ite != q.getHeaders().end(); ite++)
//	{
//			o << ite->first << ": ";
//			o << ite->second << std::endl;
//	}
//	o << q.getBody();
	return (o);
}