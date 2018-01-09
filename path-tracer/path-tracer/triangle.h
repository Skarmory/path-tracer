#ifndef triangle_h
#define triangle_h

#include "vec3.h"
#include "ray.h"
#include <cmath>

#define eps1 1e-7
#define eps2 1e-10

struct triangle
{
	vec3 v0, v1, v2;
	vec3 normal;

	triangle(vec3 v0, vec3 v1, vec3 v2)
		: v0(v0), v1(v1), v2(v2)
	{
		vec3 e1 = v1 - v0; 
		vec3 e2 = v2 - v0;

		normal = normalise(cross(e1, e2));
	}

	float intersects(const ray& iray) const
	{
		// Two edges of triangle connected to point alv0
		vec3 e1 = v1 - v0;
		vec3 e2 = v2 - v0;
		vec3 pvec = cross(iray.direction, e2);

		float rate_of_approach = dot(e1, pvec);

		vec3 tvec = iray.origin - v0; // Ray pointing from v0 to origin
		vec3 qvec = cross(tvec, e1);

		float beta = dot(tvec, pvec) / rate_of_approach;

		float gamma = dot(iray.direction, qvec) / rate_of_approach;

		float alpha = 1.f - gamma - beta;

		float dist = dot(e2, qvec) / rate_of_approach;

		if ((rate_of_approach <= eps1) || (alpha < -eps2) || (beta < -eps2) || (gamma < -eps2) || (dist <  0.f))
			return INFINITY;
		else
			return dist;	
	}
};

#endif
