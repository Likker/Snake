#ifndef NIBBLEROPENGL_HPP_
# define NIBBLEROPENGL_HPP_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include "INibbler.hpp"
#include "Exeption.hpp"

class		Nibbler: public INibbler
{
private:
  enum		SnakePart
    {
      HEAD,
      BODY,
      FRUIT,
      BONUS_FRUIT,
      MALUS_FRUIT
    };

private:
  struct	t_map
  {
    size_t	width;
    size_t	height;
    GLuint	ground;
  };

  struct	t_camPos
  {
    int		x;
    int		y;
    int		z;
    int		l;
    int		m;
    int		n;
    bool	xAxe;
    bool	yAxe;
    bool	zAxe;
  };

private:

  t_map		_map;

  SDL_Event	_event;
  SDL_Rect	_realSize;
  SDL_Surface	*_screen;
  TTF_Font	*_font;

private:
  bool		_initialized;

  t_camPos	_default;
  int		_fpsRot;
  bool		_firstView;

public:
  Nibbler();
  ~Nibbler();

  virtual bool	init(const int width, const int height);
  virtual int	getKey();
  virtual void	display(const int x, const int y, const bool first);
  virtual void	displayFruit(const int x, const int y, const int effect);
  virtual void	displayIntro();
  virtual void	timer(const int time);
  virtual void	refreshScreen();
  virtual void	end(const int score);

private:
  bool		loadImage(GLuint &res, const std::string &image) const;
  void		drawWall(const int x, const int y, const int z) const;
  void		drawPart(const double x, const double y, const SnakePart part) const;
  void		printSdl(const std::string str, const int wait);

};

# endif /* !NIBBLEROPENGL_HPP_ */
