#include <iostream>
#include <cstring>
#include <cassert>
#include <GL/glut.h>

#include "grid.h"

#include "ray.h"
#include "hit.h"
#include "light.h"
#include "raytracer.h"
#include "raytracing_stats.h"

#include "camera.h"

#include "scene_parser.h"
#include "global.h"

#include "image.h"
#include "glCanvas.h"

#include "film.h"
#include "sampler.h"
#include "filter.h"

using namespace std;

Global global;

void render();
void traceRayAt(float x, float y);

int main(int argc, char* argv[])
{
	// init
	srand(2020);
	glutInit(&argc, argv);
	global.parse(argc, argv);

	if (global.gui) {
		GLCanvas().initialize(global.sceneParser, render, 
			traceRayAt, global.grid, global.visualize_grid);
	} else {
		render();
	}

	if(global.stats)
		RayTracingStats::PrintStatistics();
	cout << endl;
	return 0;
}

void render()
{
	if (!global.sample_file && !global.filter_file && !global.output_file) {
		cout << "no output filename is given! "<< endl;
		return;
	}

	cout << "rendering scene: " << global.input_file << "..." << endl;
	Camera *camera = global.sceneParser->getCamera();
	RayTracer *rayTracer = global.rayTracer;

	// sampling
	Sampler* sampler = global.sampler;
	Filter* filter = global.filter;
	Film film(global.width, global.height, sampler->n);
	
	Vec2f delta(1.0f / global.width, 1.0f / global.height);
	for(int i = 0; i < global.width; i++)
	for(int j = 0; j < global.height; j++) {
		//cout << i << ", " << j << endl;
		Vec2f origin = Vec2f(i * delta.x(), j * delta.y());
		for (int k = 0; k < sampler->n; k++) {
			Vec2f offset = sampler->getSamplePosition(k);
			Ray ray = camera->generateRay(origin + offset * delta);

			Hit hit;
			Vec3f color = rayTracer->traceRay(
				ray, camera->getTMin(), 0, 1,
				Material::indexOfVacuum, hit
			);
			film.setSample(i, j, k, offset, color);
		}
	}
	// draw pixel
	Image output(global.width, global.height);
	for (int i = 0; i < global.width; i++)
	for (int j = 0; j < global.height; j++) {
		output.SetPixel(i, j, filter->getColor(i, j, &film));
	}
	// save image
	if (global.sample_file) {
		cout << "output sample image: " << global.sample_file << endl;
		film.renderSamples(global.sample_file, global.sample_zoom);
	}
	if (global.filter_file) {
		cout << "output filter image: " << global.filter_file << endl;
		film.renderFilter(global.filter_file, global.filter_zoom, filter);
	}
	if (!global.output_file) {
		cout << "output filename of the scene is not given." << endl;
	} else {
		cout << "output image: " << global.output_file << endl;
		output.SaveTGA(global.output_file);
	}
}

void traceRayAt(float x, float y)
{
	Camera *camera = global.sceneParser->getCamera();
	Ray ray = camera->generateRay(Vec2f(x, y));

	Hit hit;
	global.rayTracer->traceRay(
		ray, camera->getTMin(), 0, 1,
		Material::indexOfVacuum, hit
	);
}

