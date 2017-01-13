#ifndef GAME_HPP_
# define GAME_HPP_

#include <map>
#include "INibbler.hpp"
#include "Snake.hpp"
#include "Fruit.hpp"

class Snake;
class Fruit;

class			Game
{

private:
  Fruit						*_objFruit;
  Fruit						*_objMalus;
  Snake						_objSnake;
  int						_width;
  int						_height;
  int						_life;
  time_t					_timer;
  bool						_isMalus;
  bool						_isBonus;
  bool						_loop;
  double					_snakeFps;
  std::map<int, void (Game::*)(Snake::t_node*) const>	_keyCodeAction;
  int						_score;

public:
  Game(const std::string height = "20", const std::string width = "20",
       int size = 3, const int life = 3);
  ~Game();

public :
  bool			execCore(char **av);

private:
  inline bool		getLoop() const {return (this->_loop);}
  inline void		setLoop(const bool &b) {this->_loop = b;}
  int			convertArg(const std::string &s) const;
  int			getLife() const;
  void			moveSnake(const int &key);
  void			moveDirLeft(Snake::t_node *new_n) const;
  void			moveDirRight(Snake::t_node *new_n) const;
  int			checkOnSnake(const int x, const int y) const;
  int			checkPos() const;
  int			eat(const int type);
  void			hasEat(const int type);
  void			displaySnake(INibbler &nib);
  double		getFps() const;
  void			setFps(const double fps);
  int			getDelay() const;
  std::string		checkLib(std::string lib);
};

#endif /* !GAME_HPP_ */
