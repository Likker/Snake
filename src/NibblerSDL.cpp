#include <iostream>
#include <sstream>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "CommunLib.hpp"
#include "Fruit.hpp"
#include "Exeption.hpp"
#include "NibblerSDL.hpp"

extern "C"
{
  INibbler	*extern_constructor()
    {
      return (new Nibbler());
    }
}

Nibbler::Nibbler()
{
  std::cout << "Nibbler SDL" << std::endl;
  this->_initialized = false;
}

Nibbler::~Nibbler()
{
}

bool 		Nibbler::init(const int width, const int height)
{

  if (this->_initialized)
    {
      std::cout << "Nibbler is already initialized" << std::endl;
      return (false);
    }
  try
    {
      this->_initialized = true;
      this->_loop = true;
      this->_size.x = width;
      this->_size.y = height;
      if(TTF_Init() == -1)
	throw ErrorNib(std::string("TTF_Init"));
      if ((this->_font = TTF_OpenFont("font/square_pixel-7.ttf", 50)) == NULL)
	throw ErrorNib(std::string("Load font"));
      if (SDL_Init(SDL_INIT_VIDEO))
	throw ErrorNib(std::string("SDL_Init"));
      if (!(this->_screen = SDL_SetVideoMode((this->_size.y * NIB_SIZE_IMG), (this->_size.x * NIB_SIZE_IMG), 32, SDL_HWSURFACE)))
	throw ErrorNib("SDL_SetVideoMode");
      this->_tex.wall = loadTexture("wall.jpg");
      this->_tex.head = loadTexture("head.png");
      this->_tex.body = loadTexture("body.png");
      this->_tex.malus = loadTexture("malus.png");
      this->_tex.bonus = loadTexture("bonus.png");
      this->_tex.empty = loadTexture("empty.png");
      this->_tex.ins = loadTexture("ins.png");
      initMap();
      if (SDL_Flip(this->_screen) == -1)
	throw ErrorNib("refresh screen");
    }
  catch (const ErrorNib &e)
    {
      std::cout << "Initialization failed =(" << std::endl;
      std::cerr << "  Error: " << e.what() << std::endl;
      return (false);
    }
  std::cout << "Initialization success =)" << std::endl;
  displayIntro();
  return (true);
}

int		Nibbler::getKey()
{
  int		key;

  key = 0;
  while (SDL_PollEvent(&(this->_event)))
    {
      if (this->_event.type == SDL_QUIT)
	key = NIB_KEY_ESC;
      if (this->_event.type == SDL_KEYDOWN)
	{
	  switch (this->_event.key.keysym.sym)
	    {
	    case (SDLK_ESCAPE):
	      key = NIB_KEY_ESC;
	    break;
	    case (SDLK_RIGHT):
	      key = NIB_KEY_RIGHT;
	    break;
	    case (SDLK_LEFT):
	      key = NIB_KEY_LEFT;
	    break;
	    default:
	    break;
	    }
	}
    }
  return (key);
}

void		Nibbler::timer(const int time)
{
  SDL_Delay((time / 1000));
}

void		Nibbler::refreshScreen()
{
  if (SDL_Flip(this->_screen) == -1)
    throw ErrorNib("Error: SDL_Flip failed");
}

void		Nibbler::end(const int score)
{
  if (this->_initialized)
    {
      std::stringstream ss;
      ss << score;
      printSdl("Score : " + ss.str(), 2000000);;
      std::cout << "Score = " << score << std::endl;
      std::cout << "End of game" << std::endl;
      this->_initialized = false;
      this->_loop = false;
      this->_size.x = 0;
      this->_size.y = 0;
      TTF_CloseFont(this->_font);
      TTF_Quit();
      SDL_FreeSurface(this->_screen);
      SDL_Quit();
    }
}

void		Nibbler::display(const int x, const int y, const bool first)
{
  SDL_Rect	pos;
  SDL_Surface	*elem;

  pos.x = x * NIB_SIZE_IMG;
  pos.y = y * NIB_SIZE_IMG;
  if (first)
    {
      initMap();
      elem = this->_tex.head;
    }
  else
    elem = this->_tex.body;
  if (SDL_BlitSurface(elem, 0, this->_screen, &pos) < 0)
    throw ErrorNib("Error: display snake");
}

void		Nibbler::displayFruit(const int x, const int y, const int effect)
{
  SDL_Rect	pos;
  SDL_Surface	*elem;

  pos.x = x * NIB_SIZE_IMG;
  pos.y = y * NIB_SIZE_IMG;
  if (effect == NO_EFFECT)
    elem = this->_tex.bonus;
  else
    elem = this->_tex.malus;
  if (SDL_BlitSurface(elem, 0, this->_screen, &pos) < 0)
    throw ErrorNib("Error: display fruit");
}

void			Nibbler::displayIntro()
{
  int			i = 3;
  std::stringstream	ss;

  while (i)
    {
      ss << i;
      printSdl(ss.str(), 1000000);
      initMap();
      ss.str("");
      ss.clear();
      --i;
    }
  printSdl("Go !!", 1000000);
}

/* PRIVATE */

void		Nibbler::printSdl(const std::string &str, const int wait)
{
  SDL_Rect	tmp;
  SDL_Color	BlackColor;
  SDL_Surface	*text = NULL;

  BlackColor.r = 0;
  BlackColor.g = 0;
  BlackColor.b = 0;
  text = TTF_RenderText_Blended(this->_font, str.c_str(), BlackColor);
  tmp.x = ((this->_size.x * NIB_SIZE_IMG) / 2) - (text->w / 2);
  tmp.y = ((this->_size.y * NIB_SIZE_IMG) / 2) - (text->h / 2);
  if (SDL_BlitSurface(text, 0, this->_screen, &tmp) < 0)
    throw ErrorNib("Error: display text: '" + str + "'");
  refreshScreen();
  SDL_Delay((wait / 1000));
}

SDL_Surface	*Nibbler::loadTexture(const std::string &file) const
{
  SDL_Surface	*elem = 0;
  std::string	path = NIB_PATH_IMG;

  path += file;
  if (!(elem = IMG_Load(path.c_str())))
    throw ErrorNib(std::string("IMG_Load " + std::string(IMG_GetError())));
  return (elem);
}

void		Nibbler::initMap()
{
  SDL_Surface	*elem;
  SDL_Rect	tmp;
  int		y;
  int		x;

  for (x = 0, tmp.x = 0; x < this->_size.x; x++)
    {
      for (y = 0, tmp.y = 0; y < this->_size.y; y++)
	{
	  if (x > 0 && x < (this->_size.x - 1) &&
	      y > 0 && y < (this->_size.y - 1))
	    elem = this->_tex.empty;
	  else
	    elem = this->_tex.wall;
	  if (SDL_BlitSurface(elem, 0, this->_screen, &tmp) < 0)
	    throw ErrorNib("Create map");
	  tmp.y += NIB_SIZE_IMG;
	}
      tmp.x += NIB_SIZE_IMG;
    }
}
