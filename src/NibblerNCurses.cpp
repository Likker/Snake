#include <curses.h>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <unistd.h>
#include "CommunLib.hpp"
#include "Fruit.hpp"
#include "Exeption.hpp"
#include "NibblerNCurses.hpp"

extern "C"
{
  INibbler	*extern_constructor()
    {
      return (new Nibbler());
    }
}

/********************************************************************/
/*                             PUBLIC                               */
/********************************************************************/

/*
**
** Nibbler : constructor of the Nibbler class
**
*/

Nibbler::Nibbler() : _initialized(false)
{
}

/*
**
** ~Nibbler : destructor of the Nibbler class
**
*/

Nibbler::~Nibbler()
{
}

/*
**
** init : init the class Graph for the graphical choice
**
** -> width + height
** -> bool : loop + initalized
** -> all functions needed for ncurses
**
*/

bool 		Nibbler::init(const int width, const int height)
{
  try
    {
      if (this->_initialized)
	throw ErrorNib(std::string("already init"));
      this->_initialized = true;
      this->_map.width = width;
      this->_map.height = height;
      if ((initscr()) == NULL)
	throw ErrorNib(std::string("timeout failed"));
      if ((start_color()) == ERR)
	throw ErrorNib(std::string("start_color failed"));
      if ((keypad(stdscr, TRUE)) ==  ERR)
	throw ErrorNib(std::string("keypad failed"));
      if ((init_pair(1, COLOR_BLACK, COLOR_BLACK)) == ERR)
	throw ErrorNib(std::string("init_pair failed"));
      if ((init_pair(2, COLOR_WHITE, COLOR_WHITE)) == ERR)
	throw ErrorNib(std::string("init_pair failed"));
      if ((init_pair(3, COLOR_RED, COLOR_RED)) == ERR)
	throw ErrorNib(std::string("init_pair failed"));
      if ((init_pair(4, COLOR_GREEN, COLOR_GREEN)) == ERR)
	throw ErrorNib(std::string("init_pair failed"));
      if ((init_pair(5, COLOR_CYAN, COLOR_CYAN)) == ERR)
	throw ErrorNib(std::string("init_pair failed"));
      if ((init_pair(6, COLOR_BLUE, COLOR_BLUE)) == ERR)
	throw ErrorNib(std::string("init_pair failed"));
      if ((init_pair(7, COLOR_WHITE, COLOR_BLACK)) == ERR)
	throw ErrorNib(std::string("init_pair failed"));
      if ((this->_screen = newwin(0, 0, 0, 0)) == NULL)
	throw ErrorNib(std::string("newwin failed"));
      timeout(0);
      if ((curs_set(0)) == ERR)
	throw ErrorNib(std::string("curs_set failed"));
      if ((wclear(this->_screen)) == ERR)
	throw ErrorNib(std::string("wclear failed"));
      if ((wrefresh(this->_screen)) == ERR)
	throw ErrorNib(std::string("wrefresh failed"));
      initMap();
    }
  catch (const ErrorNib &e)
    {
      std::cout << "Initialization failed" << std::endl;
      std::cerr << "  Error: " << e.what() << std::endl;
      return (false);
    }
  this->displayIntro();
  return (true);
}

/*
**
** getKey : read the event about keys and call the adequat functions
**
*/

int		Nibbler::getKey()
{
  int		vkey;
  int		ret;

  ret = 0;
  vkey = getch();
  if (vkey == KEY_LEFT)
    ret = NIB_KEY_LEFT;
  else if (vkey == KEY_RIGHT)
    ret = NIB_KEY_RIGHT;
  else if (vkey == 27)
    ret = NIB_KEY_ESC;
  return (ret);
}

/*
**
** timer : handle time of the programm
**
*/

void		Nibbler::timer(const int time)
{
  if ((usleep(time)) == -1)
    throw ErrorNib(std::string("usleep failed"));
}

/*
**
** display :  display the snake
**
*/

void		Nibbler::display(const int x, const int y, const bool first)
{
  if (first == true)
    {
      initMap();
      printChar(" ", x * 2, y, 5);
      printChar(" ", x * 2 + 1, y, 5);
    }
  else
    {
      printChar(" ", x * 2, y, 6);
      printChar(" ", x * 2 + 1, y, 6);
    }
}

/*
**
** displayFruit : display the fruit at map[y][x] in different color
** look at the effect
**
*/

void		Nibbler::displayFruit(const int x, const int y, const int effect)
{
  if (effect == NO_EFFECT)
    {
      printChar(" ", x * 2, y, 4);
      printChar(" ", x * 2 + 1, y, 4);
    }
  else
    {
      printChar(" ", x * 2, y, 3);
      printChar(" ", x * 2 + 1, y, 3);
    }
}

/*
**
** displayIntro : it display a count 3 to 0
**
*/

void			Nibbler::displayIntro()
{
  int			i = 3;
  std::string		tmp;
  std::stringstream	ss;

  while (i)
    {
      ss << i;
      tmp = ss.str();
      initMap();
      printChar(tmp.c_str(),
		(this->_map.height - (tmp.size() / 2)),
		(this->_map.width / 2),
		7);
      if ((wrefresh(this->_screen)) == ERR)
	throw ErrorNib(std::string("wrefresh failed"));
      if ((usleep(500000)) == -1)
	throw ErrorNib(std::string("usleep failed"));
      ss.str("");
      ss.clear();
      --i;
    }
  initMap();
  printChar("GO !!",
	    (this->_map.height - 3),
	    (this->_map.width / 2),
	    7);
  if ((wrefresh(this->_screen)) == ERR)
    throw ErrorNib(std::string("wrefresh failed"));
  if ((usleep(500000)) == -1)
    throw ErrorNib(std::string("usleep failed"));
}

/*
**
** refreshScreen : refresh the screen and print the new picture
**
*/

void		Nibbler::refreshScreen()
{
  if ((wrefresh(this->_screen)) == ERR)
    throw ErrorNib(std::string("wrefresh failed"));
}

/*
**
** end : it print the score and destroy / free what we need to
**
*/

void			Nibbler::end(const int score)
{
  std::string		tmp("Score = ");
  std::stringstream	ss;

  if (this->_initialized)
    {
      this->_initialized = false;
      ss << score;
      tmp += ss.str();
      initMap();
      printChar(tmp.c_str(),
		(this->_map.height - (tmp.size() / 2)),
		(this->_map.width / 2),
		7);
      if ((wrefresh(this->_screen)) == ERR)
	throw ErrorNib(std::string("wrefresh failed"));
      if ((usleep(500000)) == -1)
	throw ErrorNib(std::string("usleep failed"));
      if ((endwin()) == ERR)
	throw ErrorNib(std::string("endwin failed"));
    }
}


/********************************************************************/
/*                             PRIVATE                              */
/********************************************************************/

/*
**
** initMap : init the map and print it
**
*/

void		Nibbler::initMap() const
{
  int		pos_x = this->_map.width * 2;
  int		pos_y = this->_map.height;
  int		x;

  for (int y = 0; y < pos_y; y++)
    {
      for (x = 0; x < pos_x; x++)
	{
	  if (x > 1 && x < (pos_x - 2) &&
	      y > 0 && y < (pos_y - 1))
	    printChar(" ", x, y, 1);
	  else
	    printChar(" ", x, y, 2);
	}
    }
}

/*
**
** printChar : print the char * at map[y][x] with the color received
**
*/

void		Nibbler::printChar(const std::string msg, const int x, const int y, const int color) const
{
  if ((wattron(this->_screen, COLOR_PAIR(color))) == ERR)
    throw ErrorNib(std::string("wattron failed"));
  if ((mvwprintw(this->_screen, y, x , msg.c_str())) == ERR)
    throw ErrorNib(std::string("mvwprintw failed"));
  if ((wattroff(this->_screen, COLOR_PAIR(color))) == ERR)
    throw ErrorNib(std::string("wattroff failed"));
}
