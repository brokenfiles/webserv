#include "Query.hpp"


Query::Query()
{

}

Query::Query(const Query &query) {
    *this = query;
}

Query::~Query()
{

}

Query & Query::operator=(const Query &q) {
    this->_headers = q.getHeaders();
    this->_body = q.getBody();
    return (*this);
}


const std::string &Query::getBody() const
{
    return _body;
}

void Query::setBody(const std::string &body)
{
    _body = body;
}

void Query::setHeaders(const std::map<std::string, std::string> &headers)
{
    _headers = headers;
}

const std::map<std::string, std::string> &Query::getHeaders() const
{
    return _headers;
}