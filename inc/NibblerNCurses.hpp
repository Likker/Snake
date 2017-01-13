#ifndef NIBBLERNCURSES_HPP_
# define NIBBLERNCURSES_HPP_

#include <string>
#include "INibbler.hpp"

class			Nibbler: public INibbler
{
  private:
    typedef struct	s_map
    {
      size_t		width;
      size_t		height;
    }			t_map;

  private:
    bool		_initialized;
    t_map		_map;
    WINDOW		*_screen;

  public:
    Nibbler();
    ~Nibbler();

  private:
    void		initMap() const;
    void		printChar(const std::string msg, const int x, const int y, const int color) const;

  public:
    virtual bool	init(const int width, const int height);
    virtual int		getKey();
    virtual void	display(const int x, const int y, const bool first);
    virtual void	displayFruit(const int x, const int y, const int effect);
    virtual void	displayIntro();
    virtual void	timer(const int time);
    virtual void	refreshScreen();
    virtual void	end(const int score);
};

# endif /* !NIBBLERNCURSES_HPP_ */
