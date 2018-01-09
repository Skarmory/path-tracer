#ifndef bsdf_h
#define bsdf_h

#include <cmath>
#include <algorithm>
#include "vec3.h"

struct bsdf
{
	vec3 kL; // Lambertian
	vec3 kG; // Glossy
	float s; // Shininess

	bsdf(void) {}
	bsdf(vec3 kL, vec3 kG, float s) : kL(kL), kG(kG), s(s) {}

	// Evaluate Blinn-Phong
	vec3 evaluate_finite(const vec3& w_i, const vec3& w_o, const vec3& n) const
	{
		vec3 halfway = normalise(w_i + w_o);
		const float cos_h = std::max(0.f, std::min(1.f, dot(halfway, n)));
		float spec = powf(cos_h, s);

		return (kL + kG * ((s + 8.f) * spec / 8.f)) / M_PI;
	}
};

#endif
