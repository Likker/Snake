#ifndef EXEPTIONSDL_HPP_
# define EXEPTIONSDL_HPP_

#include <iostream>
#include <sstream>
#include <exception>

class			ErrorNib: public std::exception
{
  private:
    std::string		_msg;

  public:
    ErrorNib(std::string Mag);
    virtual ~ErrorNib() throw();

  virtual const char	*what() const throw();
};

#endif /* !EXEPTIONSDL_HPP_ */
