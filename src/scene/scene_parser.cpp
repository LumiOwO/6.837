#include <stdio.h>
#include <string.h>

#include "scene_parser.h"

#include "matrix.h"

#include "camera.h"
#include "orthographiccamera.h"
#include "perspectivecamera.h"

#include "light.h"
#include "material.h"
#include "phongmaterial.h"
#include "checkerboard.h"
#include "noise.h"
#include "marble.h"
#include "wood.h"

#include "object3d.h"
#include "group.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "transform.h"

#define DegreesToRadians(x) (((float)M_PI * x) / 180.0f)

// ====================================================================
// ====================================================================
// CONSTRUCTOR & DESTRUCTOR

SceneParser::SceneParser(const char* filename) {

  // initialize some reasonable default values
  group = NULL;
  camera = NULL;
  background_color = Vec3f(0.5,0.5,0.5);
  ambient_light = Vec3f(0,0,0);
  num_lights = 0;
  lights = NULL;
  num_materials = 0;
  materials = NULL;
  current_material = NULL;

  // parse the file
  assert(filename != NULL);
  const char *ext = &filename[strlen(filename)-4];
  assert(!strcmp(ext,".txt"));
  file = fopen(filename,"r");
  assert (file != NULL);
  parseFile();
  fclose(file);
  file = NULL;

  // if no lights are specified, set ambient light to white
  // (do solid color ray casting)
  if (num_lights == 0) {
	printf ("WARNING:  No lights specified\n");
	ambient_light = Vec3f(1,1,1);
  }
}

SceneParser::~SceneParser() {
  if (group != NULL)
	delete group;
  if (camera != NULL)
	delete camera;
  int i;
  for (i = 0; i < num_materials; i++) {
	delete materials[i]; }
  delete [] materials;
  for (i = 0; i < num_lights; i++) {
	delete lights[i]; }
  delete [] lights;
}

// ====================================================================
// ====================================================================

void SceneParser::parseFile() {
  //
  // at the top level, the scene can have a camera,
  // background color and a group of objects
  // (we add lights and other things in future assignments)
  //
  char token[MAX_PARSER_TOKEN_LENGTH];
  while (getToken(token)) {
	if (!strcmp(token, "OrthographicCamera")) {
	  parseOrthographicCamera();
	} else if (!strcmp(token, "PerspectiveCamera")) {
	  parsePerspectiveCamera();
	} else if (!strcmp(token, "Background")) {
	  parseBackground();
	} else if (!strcmp(token, "Lights")) {
	  parseLights();
	} else if (!strcmp(token, "Materials")) {
	  parseMaterials();
	} else if (!strcmp(token, "Group")) {
	  group = parseGroup();
	} else {
	  printf ("Unknown token in parseFile: '%s'\n", token);
	  exit(0);
	}
  }
}

// ====================================================================
// ====================================================================

void SceneParser::parseOrthographicCamera() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  // read in the camera parameters
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "center"));
  Vec3f center = readVec3f();
  getToken(token); assert (!strcmp(token, "direction"));
  Vec3f direction = readVec3f();
  getToken(token); assert (!strcmp(token, "up"));
  Vec3f up = readVec3f();
  getToken(token); assert (!strcmp(token, "size"));
  float size = readFloat();
  getToken(token); assert (!strcmp(token, "}"));
  camera = new OrthographicCamera(center,direction,up,size);
}


void SceneParser::parsePerspectiveCamera() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  // read in the camera parameters
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "center"));
  Vec3f center = readVec3f();
  getToken(token); assert (!strcmp(token, "direction"));
  Vec3f direction = readVec3f();
  getToken(token); assert (!strcmp(token, "up"));
  Vec3f up = readVec3f();
  getToken(token); assert (!strcmp(token, "angle"));
  float angle_degrees = readFloat();
  float angle_radians = DegreesToRadians(angle_degrees);
  getToken(token); assert (!strcmp(token, "}"));
  camera = new PerspectiveCamera(center,direction,up,angle_radians);
}

void SceneParser::parseBackground() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  // read in the background color
  getToken(token); assert (!strcmp(token, "{"));
  while (1) {
	getToken(token);
	if (!strcmp(token, "}")) {
	  break;
	} else if (!strcmp(token, "color")) {
	  background_color = readVec3f();
	} else if (!strcmp(token, "ambientLight")) {
	  ambient_light = readVec3f();
	} else {
	  printf ("Unknown token in parseBackground: '%s'\n", token);
	  assert(0);
	}
  }
}

// ====================================================================
// ====================================================================

void SceneParser::parseLights() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  // read in the number of objects
  getToken(token); assert (!strcmp(token, "numLights"));
  num_lights = readInt();
  lights = new Light*[num_lights];
  // read in the objects
  int count = 0;
  while (num_lights > count) {
	getToken(token);
	if (!strcmp(token, "DirectionalLight")) {
	  lights[count] = parseDirectionalLight();
	} else if (!strcmp(token, "PointLight")) {
	  lights[count] = parsePointLight();
	} else {
	  printf ("Unknown token in parseLight: '%s'\n", token);
	  exit(0);
	}
	count++;
  }
  getToken(token); assert (!strcmp(token, "}"));
}


Light* SceneParser::parseDirectionalLight() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "direction"));
  Vec3f direction = readVec3f();
  getToken(token); assert (!strcmp(token, "color"));
  Vec3f color = readVec3f();
  getToken(token); assert (!strcmp(token, "}"));
  return new DirectionalLight(direction,color);
}


Light* SceneParser::parsePointLight() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "position"));
  Vec3f position = readVec3f();
  getToken(token); assert (!strcmp(token, "color"));
  Vec3f color = readVec3f();
  float att[3] = { 1, 0, 0 };
  getToken(token);
  if (!strcmp(token,"attenuation")) {
	att[0] = readFloat();
	att[1] = readFloat();
	att[2] = readFloat();
	getToken(token);
  }
  assert (!strcmp(token, "}"));
  return new PointLight(position,color,att[0], att[1], att[2]);
}

// ====================================================================
// ====================================================================

void SceneParser::parseMaterials() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  // read in the number of objects
  getToken(token); assert (!strcmp(token, "numMaterials"));
  num_materials = readInt();
  materials = new Material*[num_materials];
  // read in the objects
  int count = 0;
  while (num_materials > count) {
	getToken(token);
	if (!strcmp(token, "Material") ||
		!strcmp(token, "PhongMaterial")) {
	  materials[count] = parsePhongMaterial();
	} else if (!strcmp(token, "Checkerboard")) {
	  materials[count] = parseCheckerboard(count);
	} else if (!strcmp(token, "Noise")) {
	  materials[count] = parseNoise(count);
	} else if (!strcmp(token, "Marble")) {
	  materials[count] = parseMarble(count);
	} else if (!strcmp(token, "Wood")) {
	  materials[count] = parseWood(count);
	} else {
	  printf ("Unknown token in parseMaterial: '%s'\n", token);
	  exit(0);
	}
	count++;
  }
  getToken(token); assert (!strcmp(token, "}"));
}


Material* SceneParser::parsePhongMaterial() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  Vec3f diffuseColor(1,1,1);
  Vec3f specularColor(0,0,0);
  float exponent = 1;
  Vec3f reflectiveColor(0,0,0);
  Vec3f transparentColor(0,0,0);
  float indexOfRefraction = 1;
  getToken(token); assert (!strcmp(token, "{"));
  while (1) {
	getToken(token);
	if (!strcmp(token, "diffuseColor")) {
	  diffuseColor = readVec3f();
	} else if (!strcmp(token, "specularColor")) {
	  specularColor = readVec3f();
	} else if  (!strcmp(token, "exponent")) {
	  exponent = readFloat();
	} else if (!strcmp(token, "reflectiveColor")) {
	  reflectiveColor = readVec3f();
	} else if (!strcmp(token, "transparentColor")) {
	  transparentColor = readVec3f();
	} else if (!strcmp(token, "indexOfRefraction")) {
	  indexOfRefraction = readFloat();
	} else {
	  assert (!strcmp(token, "}"));
	  break;
	}
  }
  Material *answer = new PhongMaterial(diffuseColor,specularColor,exponent,
					   reflectiveColor,transparentColor,
					   indexOfRefraction);
  return answer;
}

Material *SceneParser::parseCheckerboard(int count) {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  Matrix *matrix = NULL;
  getToken(token);
  if (!strcmp(token, "Transform")) {
	matrix = new Matrix();
	Matrix inv;
	getToken(token); assert (!strcmp(token, "{"));
	parseMatrixHelper(token, *matrix, inv);
	assert (!strcmp(token, "}"));
	getToken(token);
  }
  assert (!strcmp(token, "materialIndex"));
  int m1 = readInt();
  assert (m1 >= 0 && m1 < count);
  getToken(token); assert (!strcmp(token, "materialIndex"));
  int m2 = readInt();
  assert (m2 >= 0 && m2 < count);
  getToken(token); assert (!strcmp(token, "}"));
  return new CheckerBoard(matrix,materials[m1],materials[m2]);
}


Material *SceneParser::parseNoise(int count) {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  Matrix *matrix = NULL;
  getToken(token);
  if (!strcmp(token, "Transform")) {
	matrix = new Matrix();
	Matrix inv;
	getToken(token); assert (!strcmp(token, "{"));
	parseMatrixHelper(token, *matrix, inv);
	assert (!strcmp(token, "}"));
	getToken(token);
  }
  assert (!strcmp(token, "materialIndex"));
  int m1 = readInt();
  assert (m1 >= 0 && m1 < count);
  getToken(token); assert (!strcmp(token, "materialIndex"));
  int m2 = readInt();
  assert (m2 >= 0 && m2 < count);
  getToken(token); assert (!strcmp(token, "octaves"));
  int octaves = readInt();
  getToken(token); assert (!strcmp(token, "}"));
  return new Noise(matrix,materials[m1],materials[m2],octaves);
}

Material *SceneParser::parseMarble(int count) {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  Matrix *matrix = NULL;
  getToken(token);
  if (!strcmp(token, "Transform")) {
	matrix = new Matrix();
	Matrix inv;
	getToken(token); assert (!strcmp(token, "{"));
	parseMatrixHelper(token, *matrix, inv);
	assert (!strcmp(token, "}"));
	getToken(token);
  }
  assert (!strcmp(token, "materialIndex"));
  int m1 = readInt();
  assert (m1 >= 0 && m1 < count);
  getToken(token); assert (!strcmp(token, "materialIndex"));
  int m2 = readInt();
  assert (m2 >= 0 && m2 < count);
  getToken(token); assert (!strcmp(token, "octaves"));
  int octaves = readInt();
  getToken(token); assert (!strcmp(token, "frequency"));
  float frequency = readFloat();
  getToken(token); assert (!strcmp(token, "amplitude"));
  float amplitude = readFloat();
  getToken(token); assert (!strcmp(token, "}"));
  return new Marble(matrix,materials[m1],materials[m2],octaves,frequency,amplitude);
}


Material *SceneParser::parseWood(int count) {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  Matrix *matrix = NULL;
  getToken(token);
  if (!strcmp(token, "Transform")) {
	matrix = new Matrix();
	Matrix inv;
	getToken(token); assert (!strcmp(token, "{"));
	parseMatrixHelper(token, *matrix, inv);
	assert (!strcmp(token, "}"));
	getToken(token);
  }
  assert (!strcmp(token, "materialIndex"));
  int m1 = readInt();
  assert (m1 >= 0 && m1 < count);
  getToken(token); assert (!strcmp(token, "materialIndex"));
  int m2 = readInt();
  assert (m2 >= 0 && m2 < count);
  getToken(token); assert (!strcmp(token, "octaves"));
  int octaves = readInt();
  getToken(token); assert (!strcmp(token, "frequency"));
  float frequency = readFloat();
  getToken(token); assert (!strcmp(token, "amplitude"));
  float amplitude = readFloat();
  getToken(token); assert (!strcmp(token, "}"));
  return new Wood(matrix,materials[m1],materials[m2],octaves,frequency,amplitude);
}


// ====================================================================
// ====================================================================

Object3D* SceneParser::parseObject(char token[MAX_PARSER_TOKEN_LENGTH]) {
  Object3D *answer = NULL;
  if (!strcmp(token, "Group")) {
	answer = (Object3D*)parseGroup();
  } else if (!strcmp(token, "Sphere")) {
	answer = (Object3D*)parseSphere();
  } else if (!strcmp(token, "Plane")) {
	answer = (Object3D*)parsePlane();
  } else if (!strcmp(token, "Triangle")) {
	answer = (Object3D*)parseTriangle();
  } else if (!strcmp(token, "TriangleMesh")) {
	answer = (Object3D*)parseTriangleMesh();
  } else if (!strcmp(token, "Transform")) {
	answer = (Object3D*)parseTransform();
  } else {
	printf ("Unknown token in parseObject: '%s'\n", token);
	exit(0);
  }
  return answer;
}

// ====================================================================
// ====================================================================

Group* SceneParser::parseGroup() {
  //
  // each group starts with an integer that specifies
  // the number of objects in the group
  //
  // the material index sets the material of all objects which follow,
  // until the next material index (scoping for the materials is very
  // simple, and essentially ignores any tree hierarchy)
  //
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));

  // read in the number of objects
  getToken(token); assert (!strcmp(token, "numObjects"));
  int num_objects = readInt();

  Group *answer = new Group(num_objects);

  // read in the objects
  int count = 0;
  while (num_objects > count) {
	getToken(token);
	if (!strcmp(token, "MaterialIndex")) {
	  // change the current material
	  int index = readInt();
	  assert (index >= 0 && index <= getNumMaterials());
	  current_material = getMaterial(index);
	} else {
	  Object3D *object = parseObject(token);
	  assert (object != NULL);
	  answer->addObject(count,object);
	  count++;
	}
  }
  getToken(token); assert (!strcmp(token, "}"));

  // return the group
  return answer;
}

// ====================================================================
// ====================================================================

Sphere* SceneParser::parseSphere() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "center"));
  Vec3f center = readVec3f();
  getToken(token); assert (!strcmp(token, "radius"));
  float radius = readFloat();
  getToken(token); assert (!strcmp(token, "}"));
  assert (current_material != NULL);
  return new Sphere(center,radius,current_material);
}


Plane* SceneParser::parsePlane() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "normal"));
  Vec3f normal = readVec3f();
  getToken(token); assert (!strcmp(token, "offset"));
  float offset = readFloat();
  getToken(token); assert (!strcmp(token, "}"));
  assert (current_material != NULL);
  return new Plane(normal,offset,current_material);
}


Triangle* SceneParser::parseTriangle() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token);
  assert (!strcmp(token, "vertex0"));
  Vec3f v0 = readVec3f();
  getToken(token);
  assert (!strcmp(token, "vertex1"));
  Vec3f v1 = readVec3f();
  getToken(token);
  assert (!strcmp(token, "vertex2"));
  Vec3f v2 = readVec3f();
  getToken(token); assert (!strcmp(token, "}"));
  assert (current_material != NULL);
  return new Triangle(v0,v1,v2,current_material);
}

Group* SceneParser::parseTriangleMesh() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  char filename[MAX_PARSER_TOKEN_LENGTH];
  // get the filename
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "obj_file"));
  getToken(filename);
  getToken(token); assert (!strcmp(token, "}"));
  const char *ext = &filename[strlen(filename)-4];
  assert(!strcmp(ext,".obj"));
  // read it once, get counts
  FILE *file = fopen(filename,"r");
  assert (file != NULL);
  int vcount = 0; int fcount = 0;
  while (1) {
	int c = fgetc(file);
	if (c == EOF) { break;
	} else if (c == 'v') {
	  assert(fcount == 0); float v0,v1,v2;
	  fscanf (file,"%f %f %f",&v0,&v1,&v2);
	  vcount++;
	} else if (c == 'f') {
	  int f0,f1,f2;
	  fscanf (file,"%d %d %d",&f0,&f1,&f2);
	  fcount++;
	} // otherwise, must be whitespace
  }
  fclose(file);
  // make arrays
  Vec3f *verts = new Vec3f[vcount];
  Group *answer = new Group(fcount);
  // read it again, save it
  file = fopen(filename,"r");
  assert (file != NULL);
  int new_vcount = 0; int new_fcount = 0;
  while (1) {
	int c = fgetc(file);
	if (c == EOF) { break;
	} else if (c == 'v') {
	  assert(new_fcount == 0); float v0,v1,v2;
	  fscanf (file,"%f %f %f",&v0,&v1,&v2);
	  verts[new_vcount] = Vec3f(v0,v1,v2);
	  new_vcount++;
	} else if (c == 'f') {
	  assert (vcount == new_vcount);
	  int f0,f1,f2;
	  fscanf (file,"%d %d %d",&f0,&f1,&f2);
	  // indexed starting at 1...
	  assert (f0 > 0 && f0 <= vcount);
	  assert (f1 > 0 && f1 <= vcount);
	  assert (f2 > 0 && f2 <= vcount);
	  assert (current_material != NULL);
	  Triangle *t = new Triangle(verts[f0-1],verts[f1-1],verts[f2-1],current_material);
	  answer->addObject(new_fcount,t);
	  new_fcount++;
	} // otherwise, must be whitespace
  }
  delete [] verts;
  assert (fcount == new_fcount);
  assert (vcount == new_vcount);
  fclose(file);
  return answer;
}


Transform* SceneParser::parseTransform() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  Matrix matrix;
  Matrix inv;
  // opening brace
  getToken(token); assert (!strcmp(token, "{"));
  // the matrix
  parseMatrixHelper(token, matrix, inv);
  // the Object3D
  Object3D *object = parseObject(token);
  assert(object != NULL);
  // closing brace
  getToken(token); assert (!strcmp(token, "}"));
  return new Transform(object, matrix, inv);
}

void SceneParser::parseMatrixHelper(char token[MAX_PARSER_TOKEN_LENGTH], Matrix &matrix, Matrix &inv) {
	// read in transformations:
	// apply to the LEFT side of the current matrix (so the first
	// transform in the list is the last applied to the object)
	while (1) {
		getToken(token);
		if (!strcmp(token, "Scale")) {
			Vec3f s = readVec3f();
			Vec3f s_inv = Vec3f(1.0f / s.x(), 1.0f / s.y(), 1.0f / s.z());
			matrix = matrix * Matrix::MakeScale(s);
			inv = Matrix::MakeScale(s_inv) * inv;
		} else if (!strcmp(token, "UniformScale")) {
			float s = readFloat();
			matrix = matrix * Matrix::MakeScale(s);
			inv = Matrix::MakeScale(1.0f / s) * inv;
		} else if (!strcmp(token, "Translate")) {
			Vec3f v = readVec3f();
			matrix = matrix * Matrix::MakeTranslation(v);
			inv = Matrix::MakeTranslation(-v) * inv;
		} else if (!strcmp(token, "XRotate")) {
			float rad = DegreesToRadians(readFloat());
			matrix = matrix * Matrix::MakeXRotation(rad);
			inv = Matrix::MakeXRotation(-rad) * inv;
		} else if (!strcmp(token, "YRotate")) {
			float rad = DegreesToRadians(readFloat());
			matrix = matrix * Matrix::MakeYRotation(rad);
			inv = Matrix::MakeYRotation(-rad) * inv;
		} else if (!strcmp(token, "ZRotate")) {
			float rad = DegreesToRadians(readFloat());
			matrix = matrix * Matrix::MakeZRotation(rad);
			inv = Matrix::MakeZRotation(-rad) * inv;
		} else if (!strcmp(token, "Rotate")) {
			getToken(token); assert(!strcmp(token, "{"));
			Vec3f axis = readVec3f();
			float rad = DegreesToRadians(readFloat());
			matrix = matrix * Matrix::MakeAxisRotation(axis, rad);
			inv = Matrix::MakeAxisRotation(axis, -rad) * inv;
			getToken(token); assert(!strcmp(token, "}"));
		} else if (!strcmp(token, "Matrix")) {
			Matrix matrix2; matrix2.SetToIdentity();
			getToken(token); assert(!strcmp(token, "{"));
			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 4; i++) {
					float v = readFloat();
					matrix2.Set(i, j, v);
				}
			}
			getToken(token); assert(!strcmp(token, "}"));
			matrix = matrix * matrix2;
			inv = matrix2.Inverse() * inv;
		} else {
			// otherwise this must be the thing to transform
			break;
		}
	}
}


// ====================================================================
// ====================================================================

int SceneParser::getToken(char token[MAX_PARSER_TOKEN_LENGTH]) {
  // for simplicity, tokens must be separated by whitespace
  assert (file != NULL);
  int success = fscanf(file,"%s ",token);
  if (success == EOF) {
	token[0] = '\0';
	return 0;
  }
  return 1;
}


Vec3f SceneParser::readVec3f() {
  float x,y,z;
  int count = fscanf(file,"%f %f %f",&x,&y,&z);
  if (count != 3) {
	printf ("Error trying to read 3 floats to make a Vec3f\n");
	assert (0);
  }
  return Vec3f(x,y,z);
}


Vec2f SceneParser::readVec2f() {
  float u,v;
  int count = fscanf(file,"%f %f",&u,&v);
  if (count != 2) {
	printf ("Error trying to read 2 floats to make a Vec2f\n");
	assert (0);
  }
  return Vec2f(u,v);
}


float SceneParser::readFloat() {
  float answer;
  int count = fscanf(file,"%f",&answer);
  if (count != 1) {
	printf ("Error trying to read 1 float\n");
	assert (0);
  }
  return answer;
}


int SceneParser::readInt() {
  int answer;
  int count = fscanf(file,"%d",&answer);
  if (count != 1) {
	printf ("Error trying to read 1 int\n");
	assert (0);
  }
  return answer;
}

// ====================================================================
// ====================================================================

