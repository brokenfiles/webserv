//
// Created by Timothe Lecouvreur on 18/12/2020.
//

#include "../../../includes/includes.h"
#include "Query.hpp"

std::ostream&	operator<<(std::ostream &o, const Query &q) {

	std::map<std::string, std::string>::const_iterator	ite;

	o << "----------{Contenue de la requete}------------" << std::endl;
	o << q.getMethod() << " [" << q.getPath() << "]" << std::endl;
	for(ite = q.getHeaders().begin(); ite != q.getHeaders().end(); ite++)
	{
			o << ite->first << ": ";
			o << ite->second << std::endl;
	}
	o << "----------------------------------------------" << std::endl;
	return (o);
}