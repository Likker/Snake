#include "Exeption.hpp"

ErrorNib::ErrorNib(std::string Msg)
{
  this->_msg = Msg;
}

ErrorNib::~ErrorNib() throw()
{
}

const char	*ErrorNib::what() const throw()
{
  return (this->_msg.c_str());
}
