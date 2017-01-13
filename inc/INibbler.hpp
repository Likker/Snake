#ifndef INIBBLER_HPP_
# define INIBBLER_HPP_

#include	<iostream>

# define NIB_SIZE_IMG		20
# define NIB_MAP_MIN_SIZE	10

class			INibbler
{
  public:
    virtual bool	init(const int width, const int height) = 0;
    virtual int		getKey() = 0;
    virtual void	display(const int x, const int y, const bool first) = 0;
    virtual void	displayFruit(const int x, const int y, const int effect) = 0;
    virtual void	displayIntro() = 0;
    virtual void	timer(const int time) = 0;
    virtual void	refreshScreen() = 0;
    virtual void	end(const int score) = 0;
};

#endif /* !INIBBLER_HPP_ */

