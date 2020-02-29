#include "filter.h"
#include "film.h"

Vec3f Filter::getColor(int i, int j, Film *film) const
{
	Vec3f color(0, 0, 0);
	
	int r = getSupportRadius();
	float sum = 0;
	for(int dx = -r; dx <= r; dx ++)
	for(int dy = -r; dy <= r; dy ++)
	{
		int pixel[2] = { i + dx, j + dy };
		if (pixel[0] < 0 || pixel[0] >= film->getWidth()
			|| pixel[1] < 0 || pixel[1] >= film->getHeight()) {
			continue;
		}
		for (int pk = 0; pk < film->getNumSamples(); pk++) {
			Sample s = film->getSample(pixel[0], pixel[1], pk);
			Vec2f pos = Vec2f(0.5f, 0.5f) - (s.getPosition() + Vec2f((float)dx, (float)dy));
			float w = getWeight(pos.x(), pos.y());
			color += w * s.getColor();
			sum += w;
		}
	}
	Vec3f t = color / sum;
	return color / sum;
}