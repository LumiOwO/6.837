#include "global.h"
#include <iostream>
#include <cstring>
#include <cassert>

#include "scene_parser.h"

#include "raytracer.h"
#include "raytracing_stats.h"

#include "glCanvas.h"

#include "grid.h"
#include "group.h"

#include "RandomSampler.h"
#include "UniformSampler.h"
#include "JitteredSampler.h"

#include "BoxFilter.h"
#include "TentFilter.h"
#include "GaussianFilter.h"

Global::Global()
{
	input_file = NULL;
	width = 100;
	height = 100;
	output_file = NULL;
	gui = false;
	theta_steps = 20;
	phi_steps = 10;
	gouraud = false;
	shadows = false;
	shade_back = false;
	weight = 0.01f;
	bounces = 0;
	grid_nx = 0;
	grid_ny = 0;
	grid_nz = 0;
	visualize_grid = false;
	stats = false;

	sample_file = NULL;
	sample_zoom = 0;
	filter_file = NULL;
	filter_zoom = 0;

	sceneParser = NULL;
	rayTracer = NULL;
	grid = NULL;
	bound = NULL;
	sampler = NULL;
	filter = NULL;
}

Global::~Global()
{
	if (sceneParser) delete sceneParser;
	if (rayTracer) delete rayTracer;
	if (grid) delete grid;
	if (bound) delete bound;
	if (sampler) delete sampler;
	if (filter) delete filter;
}

void Global::parse(int argc, char* argv[])
{
	// sample command line:
	// raytracer -input scene5_01_sphere.txt -size 200 200
	// -output output5_01c.tga -gui -grid 15 15 15 -visualize_grid

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i],"-input")) {
			i++; assert (i < argc);
			input_file = argv[i];
		} else if (!strcmp(argv[i],"-size")) {
			i++; assert (i < argc);
			width = atoi(argv[i]);
			i++; assert (i < argc);
			height = atoi(argv[i]);
		} else if (!strcmp(argv[i],"-output")) {
			i++; assert (i < argc);
			output_file = argv[i];
		} else if (!strcmp(argv[i],"-gui")) {
			gui = true;
		} else if (!strcmp(argv[i],"-tessellation")) {
			i++; assert (i < argc);
			theta_steps = atoi(argv[i]);
			i++; assert (i < argc);
			phi_steps = atoi(argv[i]);
		} else if (!strcmp(argv[i],"-gouraud")) {
			gouraud = true;
		} else if (!strcmp(argv[i],"-shadows")) {
			shadows = true;
		} else if (!strcmp(argv[i],"-shade_back")) {
			shade_back = true;
		} else if (!strcmp(argv[i],"-weight")) {
			i++; assert (i < argc);
			weight = (float)atof(argv[i]);
		} else if (!strcmp(argv[i],"-bounces")) {
			i++; assert (i < argc);
			bounces = atoi(argv[i]);
		} else if (!strcmp(argv[i],"-grid")) {
			i++; assert (i < argc);
			grid_nx = atoi(argv[i]);
			i++; assert (i < argc);
			grid_ny = atoi(argv[i]);
			i++; assert (i < argc);
			grid_nz = atoi(argv[i]);
		} else if (!strcmp(argv[i],"-visualize_grid")) {
			visualize_grid = true;
		} else if (!strcmp(argv[i], "-stats")) {
			stats = true;
		} else if (!strcmp(argv[i], "-random_samples")) {
			i++; assert(i < argc);
			sampler = new RandomSampler(atoi(argv[i]));
		} else if (!strcmp(argv[i], "-uniform_samples")) {
			i++; assert(i < argc);
			sampler = new UniformSampler(atoi(argv[i]));
		} else if (!strcmp(argv[i], "-jittered_samples")) {
			i++; assert(i < argc);
			sampler = new JitteredSampler(atoi(argv[i]));
		} else if (!strcmp(argv[i], "-box_filter")) {
			i++; assert(i < argc);
			filter = new BoxFilter((float)atof(argv[i]));
		} else if (!strcmp(argv[i], "-tent_filter")) {
			i++; assert(i < argc);
			filter = new TentFilter((float)atof(argv[i]));
		} else if (!strcmp(argv[i], "-gaussian_filter")) {
			i++; assert(i < argc);
			filter = new GaussianFilter((float)atof(argv[i]));
		} else if (!strcmp(argv[i], "-render_samples")) {
			i++; assert(i < argc);
			sample_file = argv[i];
			i++; assert(i < argc);
			sample_zoom = atoi(argv[i]);
		} else if (!strcmp(argv[i], "-render_filter")) {
			i++; assert(i < argc);
			filter_file = argv[i];
			i++; assert(i < argc);
			filter_zoom = atoi(argv[i]);
		} else {
			printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
			assert(0);
		}
	}
	if (!input_file) {
		cout << "no output filename is given! " << endl;
		exit(1);
	}

	if (!sampler) {
		sampler = new UniformSampler(1);
	}
	if (!filter) {
		filter = new BoxFilter(0.5);
	}
	GLCanvas::theta_steps = theta_steps;
	GLCanvas::phi_steps = phi_steps;
	GLCanvas::gouraud = gouraud;

	sceneParser = new SceneParser(input_file);
	bound = new Bound(sceneParser->getGroup()->worldBound());
	RayTracingStats::Initialize(width, height, bound, grid_nx, grid_ny, grid_nz);

	if(grid_nx > 0)
		grid = new Grid(bound, grid_nx, grid_ny, grid_nz);
	rayTracer = new RayTracer(sceneParser, bounces, weight, shadows, shade_back, grid, visualize_grid);

}
