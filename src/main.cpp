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

#include <thread>

using namespace std;

Global global;

void render();
void traceRayAt(float x, float y);

int divide(int bound[], int x0, int y0, int x1, int y1);
void sample_parellel(Film *film, int x0, int y0, int x1, int y1);
void filter_parellel(Image *image, Film *film, const Filter* filter,
					 int x0, int y0, int x1, int y1);

int main(int argc, char* argv[])
{
	// init
	srand(2020);
	global.parse(argc, argv);

	if (global.gui) {
		glutInit(&argc, argv);
		GLCanvas().initialize(global.sceneParser, render, 
			traceRayAt, global.grid, global.visualize_grid);
	} else {
		render();
	}

	if (global.stats) {
		RayTracingStats::PrintStatistics();
	}
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

	// sampling with 4 threads
	Film film(global.width, global.height, global.sampler->n);
	sample_parellel(&film, 0, 0, global.width, global.height);
	// draw pixel with 4 threads
	Image output(global.width, global.height);
	filter_parellel(&output, &film, global.filter, 0, 0, global.width, global.height);

	// save image
	if (global.sample_file) {
		cout << "output sample image: " << global.sample_file << endl;
		film.renderSamples(global.sample_file, global.sample_zoom);
	}
	if (global.filter_file) {
		cout << "output filter image: " << global.filter_file << endl;
		film.renderFilter(global.filter_file, global.filter_zoom, global.filter);
	}
	if (global.output_file) {
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

int divide(int bound[], int x0, int y0, int x1, int y1)
{
	int width_half = (x1 - x0) >> 1;
	int height_half = (y1 - y0) >> 1;
	if (width_half <= 0 && height_half <= 0) {
		bound[0] = x0;
		bound[1] = y0;
		bound[2] = x1;
		bound[3] = y1;
		return 1;
	}

	int b[] = {
		x0, y0, x0 + width_half, y0 + height_half,
		x0, y0 + height_half, x0 + width_half, y1,
		x0 + width_half, y0, x1, y0 + height_half,
		x0 + width_half, y0 + height_half, x1, y1,
	};
	for (int i = 0; i < 16; i++) {
		bound[i] = b[i];
	}
	return 4;
}

void sample_parellel(Film *film, int x0, int y0, int x1, int y1)
{
	int bound[16];
	int num = divide(bound, x0, y0, x1, y1);
	// sampling with 4 threads
	vector<thread> threads;
	for (int id = 0; id < num; id++) {
		threads.push_back(thread([id, bound, film]() mutable {
			const int *v = &bound[id << 2];
			Vec2f delta(1.0f / global.width, 1.0f / global.height);
			Camera *camera = global.sceneParser->getCamera();
			RayTracer *rayTracer = global.rayTracer;
			Sampler* sampler = global.sampler;

			for (int i = v[0]; i < v[2]; i++)
			for (int j = v[1]; j < v[3]; j++) {
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
					film->setSample(i, j, k, offset, color);
				}
			}
		}));
	}
	for (thread& t : threads) {
		t.join();
	}
}

void filter_parellel(Image *image, Film *film, const Filter* filter, 
					 int x0, int y0, int x1, int y1)
{
	int bound[16];
	int num = divide(bound, x0, y0, x1, y1);
	// filter with 4 threads
	vector<thread> threads;
	for (int id = 0; id < num; id++) {
		threads.push_back(thread([id, bound, image, film, filter]() {
			const int *v = &bound[id << 2];
			Vec2f delta(1.0f / global.width, 1.0f / global.height);
			for (int i = v[0]; i < v[2]; i++)
				for (int j = v[1]; j < v[3]; j++) {
					image->SetPixel(i, j, global.filter->getColor(i, j, film));
				}
		}));
	}
	for (thread& t : threads) {
		t.join();
	}
}
