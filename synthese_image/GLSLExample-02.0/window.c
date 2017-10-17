/*!\file window.c
 *
 * \brief Utilisation de GL4Dummies et OpenGL 3+ pour :
 * flou, sobel ou itérations de Flous en GPU
 *
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date October 27 2016
 */
#include <GL4D/gl4du.h>
#include <GL4D/gl4dg.h>
#include <GL4D/gl4duw_SDL2.h>
/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void resize(int w, int h);
static void ioDraw(GLuint fbo, GLuint inTex, GLuint outTex);
static void draw(void);
static void myTex(GLuint * id);
static void quit(void);

/*!\brief dimensions de la fenêtre */
static int _windowWidth = 800, _windowHeight = 600;
/*!\brief identifiant de la géométrie de type Quad */
static GLuint _quad = 0;
/*!\brief identifiants des (futurs) GLSL programs */
static GLuint _pId = 0;
/*!\brief identifiant de la texture chargée */
static GLuint _tId = 0;
/*!\brief identifiant du FBO et des textures E/S */
static GLuint _fbo, _fboTex[2];
/*!\brief mode du programme : 1 flou, 2 sobel, 3 itérations de flou */
static int _mode = 1;
/*!\brief Création de la fenêtre et paramétrage des fonctions callback.*/
int main(int argc, char ** argv) {
  if(argc != 2 || (_mode = atoi(argv[1])) < 1 || _mode > 3) {
    fprintf(stderr, "usage: %s <1-3>\n1 flou, 2 sobel, 3 iterations de flou\n", argv[0]);
    return 2;
  }
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			 _windowWidth, _windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN))
    return 1;
  init();
  atexit(quit);
  gl4duwResizeFunc(resize);
  gl4duwDisplayFunc(draw);
  gl4duwMainLoop();
  return 0;
}
/*!\brief Initialise les paramètres OpenGL.*/
static void init(void) {
  SDL_Surface * t;
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
  _pId = _mode == 2 ? gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/sobel.fs", NULL) : gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/blur.fs", NULL);
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  resize(_windowWidth, _windowHeight);
  _quad = gl4dgGenQuadf();
  myTex(&_tId);
  if( (t = SDL_LoadBMP("ciel.bmp")) != NULL ) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "Erreur lors du chargement de la texture\n");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
  }
  
  myTex(&_fboTex[0]); glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _windowWidth, _windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  myTex(&_fboTex[1]); glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _windowWidth, _windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glGenFramebuffers(1, &_fbo);

  glUseProgram(_pId);
  glUniform1i(glGetUniformLocation(_pId, "inv"), 1);
  ioDraw(_fbo, _tId, _fboTex[0]);
}
/*!\brief Paramétrage de la vue (viewPort) OpenGL en fonction
 * des dimensions de la fenêtre.
 * \param w largeur de la fenêtre transmise par GL4Dummies.
 * \param h hauteur de la fenêtre transmise par GL4Dummies.
 */
static void resize(int w, int h) {
  _windowWidth  = w; _windowHeight = h;
  glViewport(0, 0, _windowWidth, _windowHeight);
  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.5, 0.5, -0.5 * _windowHeight / _windowWidth, 0.5 * _windowHeight / _windowWidth, 1.0, 1000.0);
  gl4duBindMatrix("modelViewMatrix");
}

static void ioDraw(GLuint fbo, GLuint inTex, GLuint outTex) {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outTex,  0);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, inTex);
  glUniform1i(glGetUniformLocation(_pId, "myTexture"), 0);
  glUniform1f(glGetUniformLocation(_pId, "width"), (GLfloat)_windowWidth);
  glUniform1f(glGetUniformLocation(_pId, "height"), (GLfloat)_windowHeight);
  gl4dgDraw(_quad);
}
/*!\brief Dessin de la géométrie texturée et effet. */
static void draw(void) {
  static int nbIt4Effect = 0;

  glUseProgram(_pId);
  glUniform1i(glGetUniformLocation(_pId, "inv"), 0);
  ioDraw(_fbo, _fboTex[nbIt4Effect % 2], _fboTex[(nbIt4Effect + 1) % 2]);

  glUseProgram(0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, _windowWidth, _windowHeight, 0, 0, _windowWidth, _windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  if(_mode == 3)
    nbIt4Effect++;
}
/*!\brief Génération et paramétrage d'une texture OpenGL.*/
static void myTex(GLuint * id) {
  glGenTextures(1, id);
  glBindTexture(GL_TEXTURE_2D, *id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
/*!\brief Appelée au moment de sortir du programme (atexit). Elle
 *  libère les données utilisées par OpenGL et GL4Dummies.*/
static void quit(void) {
  if(_tId)
    glDeleteTextures(1, &_tId);
  if(_fbo)
    glDeleteFramebuffers(1, &_fbo);
  if(_fboTex[0])
    glDeleteTextures(2, _fboTex);
  gl4duClean(GL4DU_ALL);
}
