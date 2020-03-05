#ifndef GLOBAL_H
#define GLOBAL_H

class SceneParser;
class RayTracer;
class Grid;
class Bound;
class Sampler;
class Filter;

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

	char *sample_file;
	int sample_zoom;
	char *filter_file;
	int filter_zoom;

	SceneParser* sceneParser;
	RayTracer* rayTracer;
	Grid* grid;
	Bound* bound;
	Sampler *sampler;
	Filter *filter;

	Global();
	~Global();

	void parse(int argc, char* argv[]);
};

extern Global global;

#endif // MAINPARSER_H
