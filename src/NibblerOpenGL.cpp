#include <iostream>
#include "CommunLib.hpp"
#include "Fruit.hpp"
#include "NibblerOpenGL.hpp"

extern "C"
{
  INibbler	*extern_constructor()
    {
      return (new Nibbler());
    }
}


Nibbler::Nibbler(): _screen(NULL),
		    _initialized(false),
		    _firstView(false)
{
  _default.z = 25;
  _default.l = 0;
  _default.m = 0;
  _default.n = 0;
  _default.xAxe = 0;
  _default.yAxe = 0;
  _default.zAxe = 1;
  this->_fpsRot = 180;
  std::cout << "Nibbler OpenGL" << std::endl;
}

Nibbler::~Nibbler()
{
  if (this->_initialized)
    {
      SDL_FreeSurface(this->_screen);
      SDL_Quit();
    }
}

bool 		Nibbler::init(const int width, const int height)
{
  std::string	path_texture("img/");
  std::string	grass_texture;

  grass_texture = path_texture + "opengl_texture_grass.png";
  try
    {
      if (this->_initialized)
	throw ErrorNib("Nibbler is already initialized");
      this->_initialized = true;
      this->_map.width = width;
      this->_map.height = height;
      this->_realSize.x = 800;
      this->_realSize.y = 600;
      SDL_WM_SetCaption("OpenGl Nibbler", NULL);
      if(SDL_Init(SDL_INIT_VIDEO) == -1)
	throw ErrorNib(std::string("SDL_Init"));
      if  (!(this->_screen = SDL_SetVideoMode(this->_realSize.x,
					      this->_realSize.y,
					      32, SDL_OPENGL | SDL_HWSURFACE)))
	throw ErrorNib(std::string("Video Init"));
      if (!(this->loadImage(this->_map.ground, grass_texture)))
	throw ErrorNib("Cannot load texture : " + grass_texture);

      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
      gluPerspective(70, (double)this->_realSize.x / this->_realSize.y, 1, 1000);
      _default.x = this->_map.width * 1.3;
      _default.y = this->_map.height * 1.3;
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_TEXTURE_2D);
    }
  catch (ErrorNib &e)
    {
      std::cout << "Initialization failed" << std::endl;
      std::cerr << "  Error: " << e.what() << std::endl;
      return (false);
    }
  display(this->_map.width / 2, this->_map.height, true);
  refreshScreen();
  std::cout << "Initialization success" << std::endl;
  return (true);
}

int		Nibbler::getKey(void)
{
  if (!SDL_PollEvent(&this->_event))
    return (NIB_KEY_NONE);
  switch(this->_event.type)
    {
    case SDL_QUIT:
      return (NIB_KEY_ESC);
      break;
    case SDL_KEYDOWN:
      switch(this->_event.key.keysym.sym)
	{
	case SDLK_ESCAPE:
	  return (NIB_KEY_ESC);
	  break;
	case SDLK_LEFT:
	  this->_fpsRot = (this->_fpsRot + 270) % 360;
	  return (NIB_KEY_RIGHT);
	  break;
	case SDLK_RIGHT:
	  this->_fpsRot = (this->_fpsRot + 90) % 360;
	  return (NIB_KEY_LEFT);
	  break;
	case SDLK_p:
	  this->_firstView = !this->_firstView;
	default:
	  break;
	}
      break;
    }
  return (NIB_KEY_NONE);
}

void		Nibbler::display(const int x, const int y, const bool first)
{
  if (first)
    {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glMatrixMode( GL_MODELVIEW );
      glLoadIdentity( );
      if (!this->_firstView)
	gluLookAt(this->_default.x, this->_default.y, this->_default.z,
		  this->_default.l, this->_default.m, this->_default.n,
		  this->_default.xAxe, this->_default.yAxe, this->_default.zAxe);
      else
	{
	  glRotatef(90, 0, 1, 0);
	  glRotatef(-90, 1, 0, 0);
	  glRotatef(this->_fpsRot, 0, 0, 1);
	  switch (this->_fpsRot)
	    {
	    case 0:
	      glTranslatef(-(x - 3), -(y), -2);
	      break;
	    case 180:
	      glTranslatef(-(x + 3), -(y), -2);
	      break;
	    case 270:
	      glTranslatef(-(x), -(y - 3), -2);
	      break;
	    case 90:
	      glTranslatef(-(x), -(y + 3), -2);
	      break;
	    }
	}
      glBegin(GL_QUADS);
      for (size_t i = 0; i < this->_map.width; i++)
	{
	  this->drawWall(i, 0, 0);
	  this->drawWall(i, this->_map.height, 0);
	}
      for (size_t i = 0; i <= this->_map.height; i++)
	{
	  this->drawWall(this->_map.width, i, 0);
	  this->drawWall(0, i, 0);
	}

      glColor3ub(255/2,255/2,255/2);
      glBindTexture(GL_TEXTURE_2D, this->_map.ground);

      glTexCoord2i(0, 0);
      glVertex3i(-1, -1, 0);
      glTexCoord2i(this->_map.width, 0);
      glVertex3i(this->_map.width, -1, 0);
      glTexCoord2i(this->_map.width, this->_map.height);
      glVertex3i(this->_map.width, this->_map.height, 0);
      glTexCoord2i(0, this->_map.height);
      glVertex3i(-1, this->_map.height, 0);

      this->drawPart((double)x, (double)y, Nibbler::HEAD);
    }
  else
    this->drawPart((double)x, (double)y, Nibbler::BODY);
}

void		Nibbler::refreshScreen(void)
{
  glEnd();
  glFlush();
  SDL_GL_SwapBuffers();
}

void		Nibbler::displayFruit(const int x, const int y, const int effect)
{
  switch (effect)
    {
    case NO_EFFECT:
      this->drawPart((double)x, (double)y, Nibbler::FRUIT);
      break;
    default:
      this->drawPart((double)x, (double)y, Nibbler::MALUS_FRUIT);
      break;
    }
}

void			Nibbler::displayIntro()
{
  int			i = 3;
  std::stringstream	ss;

  while (i)
    {
      ss << i;
      printSdl(ss.str(), 1000000);
      ss.str("");
      ss.clear();
      --i;
    }
  printSdl("Go !!", 1000000);
  return ;
}

void		Nibbler::timer(const int time)
{
  SDL_Delay((time / 1000));
}

/* PRIVATE STUFF */

void		Nibbler::drawPart(const double x, const double y, const SnakePart part) const
{
  double	r;
  double	g;
  double	b;
  double	z;
  double	size;

  z = 0.0;
  size = 1.0;
  switch (part)
    {
    case  Nibbler::HEAD:
      r = 1.0;
      g = 0.0;
      b = 1.0;
      break;
    case Nibbler::BODY:
      r = 0.0;
      g = 1.0;
      b = 0.0;
      break;
    case Nibbler::FRUIT:
      r = 0.0;
      g = 1.0;
      b = 1.0;
      break;
    case Nibbler::MALUS_FRUIT:
      r = .78;
      g = .10;
      b = 0.0;
      break;
    case Nibbler::BONUS_FRUIT:
      r = .0;
      g = 0.42;
      b = 1.0;
      break;
    default: return ;
    }
  glBegin(GL_POLYGON);
  glColor3f(r, g, b);
  glVertex3f(x + size, y + 0,    z + 0);
  glVertex3f(x + size, y + size, z + 0);
  glVertex3f(x + 0,    y + size, z + 0);
  glVertex3f(x + 0,    y + 0,    z + 0);

  // White side - BACK
  glBegin(GL_POLYGON);
  glColor3f(r, g, b);
  glVertex3f(x + size, y + 0,    z + size);
  glVertex3f(x + size, y + size, z + size);
  glVertex3f(x + 0,    y + size, z + size);
  glVertex3f(x + 0,    y + 0,    z + size);

  // Purple side - RIGHT
  glBegin(GL_POLYGON);
  glColor3f(r, g, b);
  glVertex3f(x + size, y + 0,    z + 0);
  glVertex3f(x + size, y + size, z + 0);
  glVertex3f(x + size, y + size, z + size);
  glVertex3f(x + size, y + 0,    z + size);

  // Green side - LEFT
  glBegin(GL_POLYGON);
  glColor3f(r, g, b);
  glVertex3f(x + 0,    y + 0,    z + size);
  glVertex3f(x + 0,    y + size, z + size);
  glVertex3f(x + 0,    y + size, z + 0);
  glVertex3f(x + 0,    y + 0,    z + 0);

  // Blue side - TOP
  glBegin(GL_POLYGON);
  glColor3f(r, g, b);
  glVertex3f(x + size, y + size, z + size);
  glVertex3f(x + size, y + size, z + 0);
  glVertex3f(x + 0,    y + size, z + 0);
  glVertex3f(x + 0,    y + size, z + size);

  // Red side - BOTTOM
  glBegin(GL_POLYGON);
  glColor3f(r, g, b);
  glVertex3f(x + size, y + 0, z + 0);
  glVertex3f(x + size, y + 0, z + size);
  glVertex3f(x + 0,    y + 0, z + size);
  glVertex3f(x + 0,    y + 0, z + 0);
}

void		Nibbler::drawWall(int x, int y, int z) const
{
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0);
  glVertex3f(x + 1, y + 0, z + 0);
  glVertex3f(x + 1, y + 1, z + 0);
  glVertex3f(x + 0, y + 1, z + 0);
  glVertex3f(x + 0, y + 0, z + 0);

  // White side - BACK
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0);
  glVertex3f(x + 1, y + 0, z + 1);
  glVertex3f(x + 1, y + 1, z + 1);
  glVertex3f(x + 0, y + 1, z + 1);
  glVertex3f(x + 0, y + 0, z + 1);

  // Purple side - RIGHT
  glBegin(GL_POLYGON);
  glColor3f(1.0, 0.0, 1.0);
  glVertex3f(x + 1, y + 0, z + 0);
  glVertex3f(x + 1, y + 1, z + 0);
  glVertex3f(x + 1, y + 1, z + 1);
  glVertex3f(x + 1, y + 0, z + 1);

  // Green side - LEFT
  glBegin(GL_POLYGON);
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(x + 0, y + 0, z + 1);
  glVertex3f(x + 0, y + 1, z + 1);
  glVertex3f(x + 0, y + 1, z + 0);
  glVertex3f(x + 0, y + 0, z + 0);

  // Blue side - TOP
  glBegin(GL_POLYGON);
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(x + 1, y + 1, z + 1);
  glVertex3f(x + 1, y + 1, z + 0);
  glVertex3f(x + 0, y + 1, z + 0);
  glVertex3f(x + 0, y + 1, z + 1);

  // Red side - BOTTOM
  glBegin(GL_POLYGON);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(x + 1, y + 0, z + 0);
  glVertex3f(x + 1, y + 0, z + 1);
  glVertex3f(x + 0, y + 0, z + 1);
  glVertex3f(x + 0, y + 0, z + 0);
}

bool			Nibbler::loadImage(GLuint &res, const std::string &image) const
{
  SDL_Surface		*tmpImage;
  SDL_Surface		*glImage;
  SDL_PixelFormat	*pixel;

  if (!(tmpImage = IMG_Load(image.c_str())))
    throw ErrorNib("Cannot open texture:" + image);
  pixel = tmpImage->format;
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
      pixel->Rmask = 0xff000000;
      pixel->Gmask = 0x00ff0000;
      pixel->Bmask = 0x0000ff00;
      pixel->Amask = 0x000000ff;
    }
  else
    {
      pixel->Rmask = 0x000000ff;
      pixel->Gmask = 0x0000ff00;
      pixel->Bmask = 0x00ff0000;
      pixel->Amask = 0xff000000;
    }
  if (!(glImage = SDL_ConvertSurface(tmpImage, pixel, SDL_SWSURFACE)))
    return (false);
  glGenTextures(1, &res);
  glBindTexture(GL_TEXTURE_2D, res);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, tmpImage->w, tmpImage->h,
		    GL_RGBA, GL_UNSIGNED_BYTE,
		    tmpImage->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  SDL_FreeSurface(tmpImage);
  SDL_FreeSurface(glImage);
  return (true);
}

void		Nibbler::end(int score)
{
  if (this->_initialized)
    {
      std::cout << "Score = " << score << std::endl;
      this->_initialized = false;
      SDL_FreeSurface(this->_screen);
      SDL_Quit();
    }
  std::cout << "End of game" << std::endl;
}

void		Nibbler::printSdl(std::string str, int wait)
{
  SDL_Rect	tmp;
  SDL_Color	BlackColor;
  SDL_Surface	*text = NULL;

  BlackColor.r = 255;
  BlackColor.g = 0;
  BlackColor.b = 0;
  text = TTF_RenderText_Blended(this->_font, str.c_str(), BlackColor);
  std::cout << str << std::endl;
  tmp.x = (this->_realSize.x / 2) - (text->w / 2);
  tmp.y = (this->_realSize.y / 2) - (text->h / 2);
  if (SDL_BlitSurface(text, 0, this->_screen, &tmp) < 0)
    std::cerr << "  Error: display text: " << str << std::endl;
  if (SDL_Flip(this->_screen) == -1)
    std::cerr << "  Error: SDL_Flip failed" << std::endl;
  SDL_Delay((wait / 1000));
}
