#ifndef _RAYTRACING_STATS_H_
#define _RAYTRACING_STATS_H_

#include <time.h>
#include <limits.h>
#include <mutex>

#include "vectors.h"
#include "bound.h"
#include "global.h"

// ====================================================================
// ====================================================================

// This class only contains static variables and static member
// functions so there is no need to call the constructor, destructor
// etc.  It's just a wrapper for the global statistics data.

class RayTracingStats {

public:

  // To be called before computation begins
  static void Initialize(int _width, int _height, Bound *_bbox, 
			 int nx, int ny, int nz);

  // Call for each non-shadow ray
  static void IncrementNumNonShadowRays() { 
	  if (!global.stats) return;
	  mutex_nonshadow_rays.lock();
	  num_nonshadow_rays++; 
	  mutex_nonshadow_rays.unlock();
  }

  // Call for each shadow ray
  static void IncrementNumShadowRays() { 
	  if (!global.stats) return;
	  mutex_shadow_rays.lock();
	  num_shadow_rays++; 
	  mutex_shadow_rays.unlock();
  }

  // Add this to each Object3D primitive's intersect routine 
  // (but not group and transform). 
  // This is a count of the number of times ray-primitive intersection
  // is computed, not the number of times it returns true
  static void IncrementNumIntersections() {    
    // WARNING:  this might overflow
	  if (!global.stats) return;
	  mutex_intersections.lock();
	  num_intersections++;
	  mutex_intersections.unlock();
  }

  // Call each time a new cell is entered by a ray
  static void IncrementNumGridCellsTraversed() {
	  if (!global.stats) return;
	  mutex_grid_cells_traversed.lock();
	  num_grid_cells_traversed++; 
	  mutex_grid_cells_traversed.unlock();
  }

  // Call when you're all done
  static void PrintStatistics();

private:

  // TIMING VARIABLES
  // use long longs to try to avoid overflow problems, but with really
  // big, complex scenes we will still have problems.
  static int width;
  static int height;
  static Bound *bbox;
  static int num_x;
  static int num_y;
  static int num_z;
  static unsigned long long start_time;

  static unsigned long long num_nonshadow_rays;
  static unsigned long long num_shadow_rays;
  static unsigned long long num_intersections;
  static unsigned long long num_grid_cells_traversed; 
  static std::mutex mutex_nonshadow_rays;
  static std::mutex mutex_shadow_rays;
  static std::mutex mutex_intersections;
  static std::mutex mutex_grid_cells_traversed;
};

// ====================================================================
// ====================================================================

#endif
