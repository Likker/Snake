//
// DynLib.cpp for cpp_nibbler in /home/gass_a/rendu/cpp_nibbler
//
// Made by Alexandre GASS
// Login  gass_a
//
// Started on  Sun Apr 06 15:06:22 2014 gass_a
// Last update Sun Apr  6 20:11:50 2014 Mathias Toussaint
//

#include "DynLib.hpp"
#include "Exeption.hpp"

DynLib::DynLib()
{
  this->_dlhandle = NULL;
}

DynLib::~DynLib()
{

}

bool		DynLib::openLib(const std::string &path, const int flag)
{
  if ((this->_dlhandle = dlopen(path.c_str(), flag)) == 0)
    {
      throw ErrorNib(std::string("Could not open file: ") + std::string(dlerror()));
      return(false);
    }
  return (true);
}

void		*DynLib::symLib(const std::string &name_function)
{
  void		*ptr = NULL;
  if ((ptr = dlsym(this->_dlhandle, name_function.c_str())) == 0)
    {
      throw ErrorNib(dlerror());
      ptr = NULL;
    }
  return (ptr);
}

bool		DynLib::closeLib()
{
  if (dlclose(this->_dlhandle) != 0)
    {
      throw ErrorNib(std::string(dlerror()));
      return (false);
    }
  return (true);
}
