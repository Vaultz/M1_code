/*!\file window.c
 *
 * \brief Utilisation de GL4Dummies et d'OpenGL 3+
 *
 * \author Far�s BELHADJ, amsi@ai.univ-paris8.fr
 * \date October 27 2016
 */
#include <GL4D/gl4du.h>
#include <GL4D/gl4dg.h>
#include <GL4D/gl4duw_SDL2.h>
#include <GL4D/gl4fd.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/videoio/videoio_c.h>
/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void resize(int w, int h);
static void draw(void);
static void quit(void);
/*!\brief dimensions de la fen�tre */
static int _w = 800, _h = 600;
/*!\brief identifiant du quadrilat�re */
static GLuint _quad = 0;
/*!\brief identifiants des (futurs) GLSL programs */
static GLuint _pId = 0;
/*!\brief identifiant de la texture charg�e */
static GLuint _tId = 0;
/*!\brief device de capture OpenCV */
CvCapture * _capture = NULL;

/*!\brief Cr�ation de la fen�tre et param�trage des fonctions callback.*/
int main(int argc, char ** argv) {
  if(!(_capture = cvCaptureFromCAM(1)))
    _capture = cvCaptureFromCAM(CV_CAP_ANY);
  _w = (int)cvGetCaptureProperty(_capture, CV_CAP_PROP_FRAME_WIDTH);
  _h = (int)cvGetCaptureProperty(_capture, CV_CAP_PROP_FRAME_HEIGHT);
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			 _w, _h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN))
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
  _pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/basic.fs", NULL);
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
  resize(_w, _h);
  _quad = gl4dgGenQuadf();
  glGenTextures(1, &_tId);
  glBindTexture(GL_TEXTURE_2D, _tId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
}
/*!\brief Param�trage de la vue (viewPort) OpenGL en fonction
 * des dimensions de la fen�tre.
 * \param w largeur de la fen�tre transmise par GL4Dummies.
 * \param h hauteur de la fen�tre transmise par GL4Dummies.
 */
static void resize(int w, int h) {
  _w  = w; _h = h;
  glViewport(0, 0, _w, _h);
}
/*!\brief Dessin de la g�om�trie textur�e. */
static void draw(void) {
  IplImage* img = cvQueryFrame(_capture);
  glBindTexture(GL_TEXTURE_2D, _tId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_BGR, GL_UNSIGNED_BYTE, img->imageData);
  glUseProgram(_pId);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, _tId);
  glUniform1i(glGetUniformLocation(_pId, "inv"), 1);
  glUniform1i(glGetUniformLocation(_pId, "myTexture"), 0);
  gl4dgDraw(_quad);

  // FONCTIONS TP
  // dispersion
  gl4dfScattering(0, 0, 20, 0, 0, GL_FALSE);
  // filtre médian
  gl4dfMedian(0,0, 1, GL_FALSE);

  // prepare sobel
  gl4dfSobelSetMixMode(GL4DF_SOBEL_MIX_MULT);
  // sobel : ne conserve que les traits
  gl4dfSobel(0, 0, GL_FALSE);

}
/*!\brief Appel�e au moment de sortir du programme (atexit). Elle
 *  lib�re les donn�es utilis�es par OpenGL et GL4Dummies.*/
static void quit(void) {
  if(_capture)
    cvReleaseCapture(&_capture);
  if(_tId)
    glDeleteTextures(1, &_tId);
  gl4duClean(GL4DU_ALL);
}
