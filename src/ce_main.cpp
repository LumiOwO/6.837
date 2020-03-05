#include <stdio.h>
#include <assert.h>
using namespace std;

#include "ce_arg_parser.h"
#include "ce_glCanvas.h"
#include "ce_spline_parser.h"
#include <GL/glut.h>

// ====================================================================
// ====================================================================

int main(int argc, char *argv[]) {

  // parse the command line arguments & input file
  ArgParser *args = new ArgParser(argc,argv);
  SplineParser* splines = new SplineParser(args->input_file);

  // launch curve editor!
  if (args->gui) {
	glutInit(&argc, argv);
    GLCanvas glcanvas;
    glcanvas.initialize(args,splines);
    // this never returns...
  }

  // output as required
  splines->SaveBezier(args);
  splines->SaveBSpline(args);
  splines->SaveTriangles(args);

  // cleanup
  delete args;
  delete splines;
  return 0;
}

// ====================================================================
// ====================================================================



