#include <cstdlib>
#include "Game.hpp"
#include "CommunLib.hpp"
#include "INibbler.hpp"

/*
**
** usage : print msg error
**
*/

static bool	usage(const int flag)
{
  if (flag == 0)
    std::cout << "usage: nibbler width height lib_nibbler_[sdl|opengl|qt].so"
	      << std::endl;
  else if (flag == 1)
    std::cout << "usage: nibbler need a width / height >= 20"
	      << std::endl;
  else if (flag == 2)
    std::cout << "usage: nibbler need a width / height <= 40"
	      << std::endl;
  return (false);
}

/*
**
** check_arg : check if size are good and if ac >= 3
**
*/

static bool	check_arg(const int ac, char **av)
{
  if (ac != 4)
    return (usage(0));
  else if (ac == 4 && (atoi(av[1]) < 20 || atoi(av[2]) < 20))
    return (usage(1));
  else if (ac == 4 && (atoi(av[1]) > 40 || atoi(av[2]) > 40))
    return (usage(2));
  return (true);
}

/*
**
** main : check arguments and call execCore from Game class
**
*/

int		main(const int ac, char **av)
{
  Game		*game;

  if ((check_arg(ac, av)) == false)
    return (-1);
  game = new Game(av[1], av[2], 3, 3);
  if ((game->execCore(av)) == false)
    {
      delete game;
      return (-1);
    }
  delete game;
  return (0);
}
