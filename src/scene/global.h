#ifndef GLOBAL_H
#define GLOBAL_H

class SceneParser;
class RayTracer;
class Grid;
class Bound;

struct Global
{
	char *input_file;
	int width;
	int height;
	char *output_file;
	bool gui;
	int theta_steps;
	int phi_steps;
	bool gouraud;
	bool shadows;
	bool shade_back;
	float weight;
	int bounces;
	int grid_nx;
	int grid_ny;
	int grid_nz;
	bool visualize_grid;
	bool stats;

	SceneParser* sceneParser;
	RayTracer* rayTracer;
	Grid* grid;
	Bound* bound;

	Global();
	~Global();

	void parse(int argc, char* argv[]);
};

#endif // MAINPARSER_H
