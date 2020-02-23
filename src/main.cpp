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

using namespace std;

static Global global;

void render();
void traceRayAt(float x, float y);

int main(int argc, char* argv[])
{
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
	return 0;
}

void render()
{
	Camera *camera = global.sceneParser->getCamera();
	RayTracer *rayTracer = global.rayTracer;

	// draw images
	cout << "drawing image: " << global.output_file << "... ";
	Image output(global.width, global.height);

	for(int i=0; i<global.width; i++)
		for(int j=0; j<global.height; j++)
		{
//			cout << i << ", " << j << endl;
			Vec2f origin(1.0f*i/global.width, 1.0f*j/global.height);
			Ray ray = camera->generateRay(origin);

			Hit hit;
			Color buffer = rayTracer->traceRay(
				ray, camera->getTMin(), 0, 1,
				RayTracer::indexOfVacuum, hit
			);

			output.SetPixel(i, j, buffer);
		}

	// save tga image
	output.SaveTGA(global.output_file);
	cout << endl;
}

void traceRayAt(float x, float y)
{
	Camera *camera = global.sceneParser->getCamera();
	Ray ray = camera->generateRay(Vec2f(x, y));

	Hit hit;
	global.rayTracer->traceRay(
		ray, camera->getTMin(), 0, 1,
		RayTracer::indexOfVacuum, hit
	);
}

