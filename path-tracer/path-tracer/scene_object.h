#ifndef scene_object_h
#define scene_object_h

#include <string>

#include "ray.h"
#include "bsdf.h"
#include "rectangle.h"
#include "util.h"

class scene_object
{
public:
	scene_object(const std::string& name, rectangle rect, bsdf b) : name(name), rect(rect), mbsdf(b) {}

	float intersects(const ray& iray)
	{
		return rect.intersects(iray);
	}

	vec3 evaluate_bsdf(const vec3& w_i, const vec3& w_o, const vec3& n) const
	{
		return mbsdf.evaluate_finite(w_i, w_o, n);
	}

	vec3 get_normal(void) const
	{
		return rect.get_normal();
	}

	// To help with debugging
	std::string get_name(void) const
	{
		return name;
	}

private:
	std::string name;
	bsdf mbsdf;
	rectangle rect;
};

#endif