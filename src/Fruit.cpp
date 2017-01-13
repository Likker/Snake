#include "Fruit.hpp"

Fruit::Fruit(int x, int y, int effect, int lenght)
{
  this->_x = x;
  this->_y = y;
  this->_effect = effect;
  this->_length = lenght;
}

Fruit::~Fruit()
{

}
