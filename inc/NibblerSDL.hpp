#ifndef NIBBLERSDL_HPP_
# define NIBBLERSDL_HPP_

#include <string>
#include <SDL/SDL.h>
#include "INibbler.hpp"

# define NIB_PATH_IMG	"img/"
# define NIB_FONT_IMG	"font/"

# define IMG_HEAD	"../img/head.bmp"
# define IMG_BODY	"../img/body.bmp"
# define IMG_TAIL	"../img/tail.bmp"
# define IMG_WALL	"../img/wall.bmp"
# define IMG_FLOOR	"../img/floor.bmp"

class			Nibbler: public INibbler
{
  private:
    struct		s_texture
    {
      SDL_Surface	*wall;
      SDL_Surface	*head;
      SDL_Surface	*body;
      SDL_Surface	*bonus;
      SDL_Surface	*malus;
      SDL_Surface	*empty;
      SDL_Surface	*ins;
    };
    struct s_texture	_tex;
    TTF_Font		*_font;
    SDL_Surface		*_screen;
    SDL_Event		_event;
    SDL_Rect		_size;
    bool		_loop;
    bool		_initialized;

  public:
    Nibbler();
    ~Nibbler();

  private:
    void		printSdl(const std::string &str, const int wait);
    SDL_Surface		*loadTexture(const std::string &file) const;
    void		initMap();

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

# endif /* !NIBBLERSDL_HPP_ */
