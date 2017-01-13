#ifndef FRUIT_HPP_
# define FRUIT_HPP_

class Snake;

#include "Snake.hpp"

enum		effect
  {
    NO_EFFECT = 1,
    POISON = 2,
    GROW_UP = 4
  };

class			Fruit
{

private:

  int			_x;
  int			_y;
  int			_effect;
  int			_length;

public:

  Fruit(int x = 0, int y = 0, int effect = NO_EFFECT, int lenght = 1);
  ~Fruit();

  int			getX() const { return (this->_x); };
  int			getY() const { return (this->_y); };
  int			getEffect() const { return (this->_effect); };
  int			getLength() const { return (this->_length); };

};

#endif /* !FRUIT_HPP_ */
