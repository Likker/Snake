//
// Snake.cpp for src in /home/gass_a/rendu/cpp_nibbler/src
//
// Made by Alexandre GASS
// Login  gass_a
//
// Started on  Thu Apr 03 15:58:05 2014 gass_a
// Last update Sun Apr  6 20:10:38 2014 Mathias Toussaint
//

#include "Snake.hpp"

std::list<Snake::t_node>	&Snake::getSnake()
{
  return (this->_snake);
}

const std::list<Snake::t_node>	&Snake::getSnake() const
{
  return (this->_snake);
}
