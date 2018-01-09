#ifndef rectangle_h
#define rectangle_h

#include "triangle.h"

class rectangle
{
public:
	rectangle(triangle t1, triangle t2) : t1(t1), t2(t2) {}

	float intersects(const ray& iray)
	{
		float xsects = t1.intersects(iray);

		if (xsects != INFINITY)
			return xsects;
		else
			return t2.intersects(iray);
	}

	vec3 get_normal(void) const
	{
		return t1.normal;
	}

private:
	triangle t1, t2;
};

#endif