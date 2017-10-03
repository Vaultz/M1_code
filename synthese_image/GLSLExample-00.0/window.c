/*!\file window.c
 *
 * \brief Utilisation de GL4Dummies et d'OpenGL 3+
 *
 * \author Far�s BELHADJ, amsi@ai.univ-paris8.fr
 * \date October 27 2016
 */
#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void resize(int w, int h);
static void draw(void);
static void quit(void);
/*!\brief dimensions de la fen�tre */
static int _windowWidth = 800, _windowHeight = 600;
/*!\brief identifiant du (futur) vertex array object */
static GLuint _vao = 0;
/*!\brief identifiant du (futur) buffer de data */
static GLuint _buffer = 0;
/*!\brief identifiants des (futurs) GLSL programs */
static GLuint _pId = 0;
/*!\brief identifiant de la texture charg�e */
static GLuint _tId = 0;
/*!\brief Cr�ation de la fen�tre et param�trage des fonctions callback.*/
int main(int argc, char ** argv) {
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
/*!\brief Initialise les param�tres OpenGL.*/
static void init(void) {
  GLfloat data[] = {
    /* 3 coordonn�es de sommets */
    -1.f, -1.f, 1.f, -1.f, 0.f,  1.f,
    /* 2 coordonn�es de texture par sommet */
    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    /* A FAIRE EN COURS :
       AJOUTER TROIS COULEURS
       TRANSFORMER LE TRIANGLE EN QUAD
    */
    // on rajoute des triplets de données pour les couleurs (format binaire)
    1,0,0, 0,1,0, 0,0,1
  };
  _pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/basic.fs", NULL);
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
  resize(_windowWidth, _windowHeight);

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glGenBuffers(1, &_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, _buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // on indique une 3e donnée dans data
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const void *)((3 * 2) * sizeof *data));

  // on décrit le format de données de la nouvelle data
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const void *)((6 * 2) * sizeof *data));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glGenTextures(1, &_tId);
  glBindTexture(GL_TEXTURE_2D, _tId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  GLuint t[] = {
     0,  0, -1,  0,  0,
     0, -1,  0, -1,  0,
     0, -1,  0, -1,  0,
     0, -1,  0, -1,  0,
    -1,  0, -1,  0, -1
  };
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 5, 5, 0, GL_RGBA, GL_UNSIGNED_BYTE, t);
}
/*!\brief Param�trage de la vue (viewPort) OpenGL en fonction
 * des dimensions de la fen�tre.
 * \param w largeur de la fen�tre transmise par GL4Dummies.
 * \param h hauteur de la fen�tre transmise par GL4Dummies.
 */
static void resize(int w, int h) {
  _windowWidth  = w; _windowHeight = h;
  glViewport(0, 0, _windowWidth, _windowHeight);
}
/*!\brief Dessin de la g�om�trie textur�e. */
static void draw(void) {
  glUseProgram(_pId);
  glDisable(GL_DEPTH_TEST);
  glBindVertexArray(_vao);
  glBindTexture(GL_TEXTURE_2D, _tId);
  glUniform1i(glGetUniformLocation(_pId, "myTexture"), 0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
/*!\brief Appel�e au moment de sortir du programme (atexit). Elle
 *  lib�re les donn�es utilis�es par OpenGL et GL4Dummies.*/
static void quit(void) {
  if(_vao)
    glDeleteVertexArrays(1, &_vao);
  if(_buffer)
    glDeleteBuffers(1, &_buffer);
  if(_tId)
    glDeleteTextures(1, &_tId);
  gl4duClean(GL4DU_ALL);
}
