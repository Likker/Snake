#ifndef SNAKE_HPP_
# define SNAKE_HPP_

#include <list>

class			Snake
{

public:
  typedef struct	s_rect
  {
    int			x;
    int			y;
    int			z;
  }			t_rect;

  typedef struct	s_node
  {
    t_rect		pos;
    t_rect		dir;
  }			t_node;

private:
  std::list<t_node>	_snake;

public:
  std::list<t_node>		&getSnake();
  const std::list<t_node>	&getSnake() const;
};

#endif /* !SNAKE_HPP_ */
