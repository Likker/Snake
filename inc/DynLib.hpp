#ifndef DYNLIB_HPP_
# define DYNLIB_HPP_

#include <dlfcn.h>
#include <string>

class			DynLib
{
  private:
    void		*_dlhandle;

  public:
    DynLib();
    ~DynLib();

  public:
    bool		openLib(const std::string &path, const int flag);
    void		*symLib(const std::string &name_function);
    bool		closeLib();
};

#endif /* !DYNLIB_HPP_ */
