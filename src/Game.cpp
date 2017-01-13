#include <iostream>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include "Exeption.hpp"
#include "DynLib.hpp"
#include "Snake.hpp"
#include "Fruit.hpp"
#include "CommunLib.hpp"
#include "Game.hpp"
#include "INibbler.hpp"

Game::Game(const std::string height, const std::string width, int size, const int life)
{
  Snake::t_node	new_n;

  fflush(stdout);
  this->_life = life;
  this->_height = convertArg(height);
  this->_width = convertArg(width);
  this->hasEat(NIB_FOOD);
  this->_loop = true;
  new_n.pos.x = (this->_height / 2);
  new_n.pos.y = (this->_width / 2);
  new_n.dir.x = -1;
  new_n.dir.y = 0;
  this->_isMalus = false;
  this->_score = 0;
  this->_objSnake.getSnake().push_front(new_n);
  while (size > 0)
    {
      new_n.pos.x -= new_n.dir.x;
      new_n.pos.y -= new_n.dir.y;
      this->_objSnake.getSnake().push_back(new_n);
      size--;
    }
  this->_keyCodeAction[NIB_KEY_LEFT] = &Game::moveDirLeft;
  this->_keyCodeAction[NIB_KEY_RIGHT] = &Game::moveDirRight;
  this->setFps(4.0);
}

Game::~Game()
{
  delete this->_objFruit;
  if (this->_isMalus == true)
    delete this->_objMalus;
}

void		Game::setFps(const double fps)
{
  if (fps > 0 && fps <= 150)
    this->_snakeFps = fps;
}

double		Game::getFps(void) const
{
    return (this->_snakeFps);
}

int		Game::getDelay(void) const
{
  return ((int)(1000000.0 / this->_snakeFps));
}

std::string	Game::checkLib(std::string lib)
{
  if (lib.find("/") == std::string::npos)
    return (("./" + lib));
  return (lib);
}

bool		Game::execCore(char **av)
{
  DynLib	dl;
  INibbler*	(*external_creator)();
  INibbler*	nib;
  int		pos;
  int		key;

  try
    {
      dl.openLib(checkLib(av[3]), RTLD_LAZY);
      external_creator = reinterpret_cast<INibbler* (*)()>(dl.symLib("extern_constructor"));
      nib = external_creator();
      srand(time(0) * getpid() * rand());
      if (!nib->init(this->_height, this->_width))
	return (false);
      while (this->getLoop() && (this->getLife() > 0))
	{
	  key = nib->getKey();
	  if (key == NIB_KEY_ESC)
	    this->setLoop(false);
	  else
	    this->moveSnake(key);
	  pos = this->checkPos();
	  if (pos == NIB_WALL || pos ==  NIB_SNAKE)
	    this->setLoop(false);
	  else if (pos == NIB_FOOD || pos == NIB_MALUS)
	    {
	      if ((this->eat(pos)) == -1)
		this->setLoop(false);
	      this->hasEat(pos);
	    }
	  this->displaySnake(*nib);
	  nib->timer(this->getDelay());
	  nib->refreshScreen();
	}
      nib->end(this->_score);
    }
  catch (const ErrorNib &e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
      return (false);
    }
  return (true);
}

void		Game::moveDirLeft(Snake::t_node *new_n) const
{
  if (this->_objSnake.getSnake().front().dir.x == -1 &&
      this->_objSnake.getSnake().front().dir.y == 0)
    {
      new_n->dir.x = 0;
      new_n->dir.y = 1;
    }
  else if (this->_objSnake.getSnake().front().dir.x == 1 &&
	   this->_objSnake.getSnake().front().dir.y == 0)
    {
      new_n->dir.x = 0;
      new_n->dir.y = -1;
    }
  else if (this->_objSnake.getSnake().front().dir.x == 0 &&
	   this->_objSnake.getSnake().front().dir.y == -1)
    {
      new_n->dir.x = -1;
      new_n->dir.y = 0;
    }
  else if (this->_objSnake.getSnake().front().dir.x == 0 &&
	   this->_objSnake.getSnake().front().dir.y == 1)
    {
      new_n->dir.x = 1;
      new_n->dir.y = 0;
    }
  new_n->dir.z = 0;
}

void		Game::moveDirRight(Snake::t_node *new_n) const
{
  if (this->_objSnake.getSnake().front().dir.x == -1 &&
      this->_objSnake.getSnake().front().dir.y == 0)
    {
      new_n->dir.x = 0;
      new_n->dir.y = -1;
    }
  else if (this->_objSnake.getSnake().front().dir.x == 1 &&
	   this->_objSnake.getSnake().front().dir.y == 0)
    {
      new_n->dir.x = 0;
      new_n->dir.y = 1;
    }
  else if (this->_objSnake.getSnake().front().dir.x == 0 &&
	   this->_objSnake.getSnake().front().dir.y == -1)
    {
      new_n->dir.x = 1;
      new_n->dir.y = 0;
    }
  else if (this->_objSnake.getSnake().front().dir.x == 0 &&
	   this->_objSnake.getSnake().front().dir.y == 1)
    {
      new_n->dir.x = -1;
      new_n->dir.y = 0;
    }
  new_n->dir.z = 0;
}

void		Game::moveSnake(const int &key)
{
  Snake::t_node	new_n;
  void		(Game::*ptr)(Snake::t_node*) const;

  new_n.pos.x = 0;
  new_n.pos.y = 0;
  new_n.dir.x = this->_objSnake.getSnake().front().dir.x;
  new_n.dir.y = this->_objSnake.getSnake().front().dir.y;
  if (this->_keyCodeAction.find(key) != this->_keyCodeAction.end())
    {
      ptr = this->_keyCodeAction[key];
      (this->*ptr)(&new_n);
    }
  new_n.pos.x = this->_objSnake.getSnake().front().dir.x + this->_objSnake.getSnake().front().pos.x;
  new_n.pos.y = this->_objSnake.getSnake().front().dir.y + this->_objSnake.getSnake().front().pos.y;
  this->_objSnake.getSnake().push_front(new_n);
  this->_objSnake.getSnake().pop_back();
}

int		Game::checkOnSnake(const int x, const int y) const
{
  std::list<Snake::t_node>::const_iterator	it = this->_objSnake.getSnake().begin();

  for (; it != this->_objSnake.getSnake().end(); ++it)
    if (x == it->pos.x && y == it->pos.y && it != this->_objSnake.getSnake().begin())
      return (NIB_SNAKE);
  return (0);
}

int			Game::checkPos() const
{
  Snake::t_node		first;

  first = this->_objSnake.getSnake().front();
  if (first.pos.x == 0 || first.pos.y == 0 ||
      first.pos.x == (this->_height - 1) ||
      first.pos.y == (this->_width - 1))
    return (NIB_WALL);
  if (first.pos.x == this->_objFruit->getX() && first.pos.y == this->_objFruit->getY())
    return (NIB_FOOD);
  if (this->_isMalus == true && first.pos.x == this->_objMalus->getX() &&
      first.pos.y == this->_objMalus->getY())
    return (NIB_MALUS);
  if ((checkOnSnake(first.pos.x, first.pos.y)) == NIB_SNAKE)
    return (NIB_SNAKE);
  return (0);
}

int					Game::eat(const int type)
{
  std::list<Snake::t_node>::iterator	it = this->_objSnake.getSnake().begin();
  Snake::t_node				new_n;
  int					length;

  if (type == NIB_MALUS && this->_objMalus->getEffect() == 2)
    this->_life -= 1;
  else if (type == NIB_FOOD || (type == NIB_MALUS && this->_objMalus->getEffect() == 4))
    {
      if (type == NIB_FOOD)
	{
	  this->_score += 1;
	  length = this->_objFruit->getLength();
	}
      else if (type == NIB_MALUS)
	length = this->_objMalus->getLength();
      while (length > 0)
	{
	  new_n = this->_objSnake.getSnake().back();
	  new_n.pos.x -= new_n.dir.x;
	  new_n.pos.y -= new_n.dir.y;
	  this->_objSnake.getSnake().push_back(new_n);
	  length--;
	}
      for (; it != this->_objSnake.getSnake().end(); ++it)
	{
	  if (it->pos.x == 0 || it->pos.y == 0 ||
	      it->pos.x == (this->_height - 1) ||
	      it->pos.y == (this->_width - 1))
	    return (-1);
	}
    }
  if (type == NIB_FOOD)
    delete this->_objFruit;
  else if (type == NIB_MALUS)
    {
      this->_isMalus = false;
      delete this->_objMalus;
    }
  this->setFps(this->getFps() + 0.5);
  return (0);
}

void		Game::hasEat(const int type)
{
  int		effect;
  int		length;
  int		nb;
  int		x;
  int		y;

  nb = 0;
  length = 1;
  if (type == NIB_MALUS)
    return ;
  while (nb == 0)
    {
      x = (rand() % (this->_height - 2)) + 1;
      y = (rand() % (this->_width - 2)) + 1;
      if (x == 0)
	x++;
      if (y == 0)
	y++;
      if ((checkOnSnake(x, y)) != NIB_SNAKE)
	nb = 1;
    }
  effect = rand() % 4;
  if (effect == 0 || effect == 3)
    effect++;
  if (effect == 4)
    length = (rand() % 4) + 1;
  if (effect != 1 && this->_isMalus == false)
    {
      nb = 0;
      this->_isMalus = true;
      time(&(this->_timer));
      this->_objMalus = new Fruit(x, y, effect, length);
      while (nb == 0)
	{
	  x = (rand() % (this->_height - 2)) + 1;
	  y = (rand() % (this->_width - 2)) + 1;
	  if (x == 0)
	    x++;
	  if (y == 0)
	    y++;
	  if ((checkOnSnake(x, y)) != NIB_SNAKE)
	    nb = 1;
	}
      this->_objFruit = new Fruit(x, y, 1, 1);
    }
  else
    this->_objFruit = new Fruit(x, y, 1, 1);
}

int		Game::getLife() const
{
  return (this->_life);
}

void						Game::displaySnake(INibbler &nib)
{
  std::list<Snake::t_node>::const_iterator	it = this->_objSnake.getSnake().begin();
  time_t					tmp;

  time(&tmp);
  for (; it != this->_objSnake.getSnake().end(); ++it)
    nib.display(it->pos.x, it->pos.y,
		((it == this->_objSnake.getSnake().begin()) ? (true) : (false)));
  nib.displayFruit(this->_objFruit->getX(), this->_objFruit->getY(),
		   this->_objFruit->getEffect());
  if (this->_isMalus == true && (difftime(tmp, this->_timer)) <= 5)
    nib.displayFruit(this->_objMalus->getX(), this->_objMalus->getY(),
		     this->_objMalus->getEffect());
  else if (difftime(tmp, this->_timer) > 5 && this->_isMalus == true)
    {
      this->_isMalus = false;
      delete this->_objMalus;
    }
}

int				Game::convertArg(const std::string &s) const
{
  std::string::const_iterator	it = s.begin();
  std::istringstream		ss(s);
  int				result = 0;

  while (it != s.end() && std::isdigit(*it))
    ++it;
  ss >> result;
  if (!s.empty() && it != s.end())
    throw ErrorNib(std::string("value if size'map is not correct"));
  if (result <= 0)
    throw ErrorNib(std::string("value if size'map need to be > 0"));
  return (result);
}
