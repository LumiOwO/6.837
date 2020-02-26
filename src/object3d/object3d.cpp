#include "object3d.h"
#include "grid.h"

void Object3D::insertIntoGrid(Grid *grid) const
{
	int minVoxel[3];
	int maxVoxel[3];
	int n[3] = { grid->nx, grid->ny, grid->nz };
	for (int i = 0; i < 3; i++) {
		minVoxel[i] = grid->toIndex(worldBound().getMin(), i);
		maxVoxel[i] = grid->toIndex(worldBound().getMax(), i);
		if (minVoxel[i] < 0) minVoxel[i] = 0;
		if (minVoxel[i] >= n[i]) minVoxel[i] = n[i] - 1;
		if (maxVoxel[i] < 0) maxVoxel[i] = 0;
		if (maxVoxel[i] >= n[i]) maxVoxel[i] = n[i] - 1;
	}
	for (int i = minVoxel[0]; i <= maxVoxel[0]; i++)
	for (int j = minVoxel[1]; j <= maxVoxel[1]; j++)
	for (int k = minVoxel[2]; k <= maxVoxel[2]; k++) {
		grid->addObjectTo(this, i, j, k);
	}
}