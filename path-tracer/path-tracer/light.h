#ifndef light_h
#define light_h

#include <cmath>

#include "vec3.h"
#include "triangle.h"
#include "util.h"

class area_light
{
public:
	area_light(triangle t1, triangle t2, vec3 emission, float power) : t1(t1), t2(t2), emission(emission), power(power) 
	{
		u = t1.v2 - t1.v1;
		v = t1.v0 - t1.v1;
		m = t1.normal;
		extent.x = length(u);
		extent.y = length(v);

		centre = t1.v1 + (u / 2) + (v / 2);
	}

	// Check intersection by seeing if it hits any of the triangles that make up this light
	float intersects(const ray& iray)
	{
		float xsects = t1.intersects(iray);

		if (xsects != INFINITY)
			return xsects;
		else
			return t2.intersects(iray);
	}

	// Find a random point on the area light
	vec3 get_random_point(void) const
	{
		return centre + u * (get_random_range(-.5f, .5f) * extent.x) + v * (get_random_range(-.5f, .5f) * extent.y);
	}

	float get_power(void) const
	{
		return power;
	}

	vec3 get_emission(void) const
	{
		return emission;
	}

	vec3 get_normal(void) const
	{
		return m;
	}

private:
	triangle t1, t2;
	float power;
	vec3 emission;

	vec3 u, v, m, extent, centre;
};

#endif