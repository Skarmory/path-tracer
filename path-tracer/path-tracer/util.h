#ifndef util_h
#define util_h

#include <random>
#include "vec3.h"

// Generate a random number in the range
float get_random_range(float lo, float hi)
{
	return lo + (rand() / (RAND_MAX / (hi - lo)));
}

// Create a coordinate frame about normal n
void create_coordinate_frame(const vec3& n, vec3& t, vec3& b)
{
	if (std::fabs(n.x) > std::fabs(n.y))
		t = vec3(n.z, 0.f, -n.x) / sqrtf(n.x * n.x + n.z * n.z);
	else
		t = vec3(0.f, -n.z, n.y) / sqrtf(n.y * n.y + n.z * n.z);

	b = cross(n, t);
}

// Get a random vector in a hemisphere
vec3 sample_hemisphere(void)
{
	float r1 = get_random_range(0.f, 1.f);
	float r2 = get_random_range(0.f, 1.f);

	float sintheta = sqrt(1.f - r1 * r1);
	float phi = 2 * M_PI * r2;
	float x = sintheta * cosf(phi);
	float z = sintheta * sinf(phi);

	return normalise(vec3(x, r1, z));
}

#endif