#ifndef point_h
#define point_h

#include <math.h>

struct vec3
{
	float x, y, z;
	vec3(void) : x(0.f), y(0.f), z(0.f) {}
	vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	vec3(const vec3& o) : x(o.x), y(o.y), z(o.z) {}

	vec3 operator-(const vec3& o) const
	{
		return vec3(x - o.x, y - o.y, z - o.z);
	}

	vec3 operator+(const vec3& o) const
	{
		return vec3(x + o.x, y + o.y, z + o.z);
	}

	vec3 operator+(float o) const
	{
		return vec3(x + o, y + o, z + o);
	}

	vec3 operator*(float f) const
	{
		return vec3(x * f, y * f, z * f);
	}

	vec3 operator*(const vec3& o) const
	{
		return vec3(x * o.x, y * o.y, z * o.z);
	}

	vec3 operator/(float f) const
	{
		return vec3(x / f, y / f, z / f);
	}

	vec3 operator-(void) const
	{
		return vec3(-x, -y, -z);
	}

	float average(void) const
	{
		return (x + y + z) / 3.f;
	}

	vec3& operator+=(const vec3& o)
	{
		x += o.x;
		y += o.y;
		z += o.z;

		return *this;
	}

	vec3& operator*=(const vec3& o)
	{
		x *= o.x;
		y *= o.y;
		z *= o.z;

		return *this;
	}

	vec3& operator/=(float f)
	{
		x /= f;
		y /= f;
		z /= f;

		return *this;
	}

	bool non_zero(void) const
	{
		if (x != 0.f && y != 0.f && z != 0.f)
			return true;
		return false;
	}
};

static float length(const vec3& a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

static vec3 normalise(const vec3& a)
{
	float len = length(a);
	if (len > 0)
		return vec3(a.x / len, a.y / len, a.z / len);
	else
		return a;
}

static float dot(const vec3& a, const vec3& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

static vec3 cross(const vec3& alv1, const vec3& alv2)
{
	return vec3(
		(alv1.y * alv2.z) - (alv1.z * alv2.y), (alv1.z * alv2.x) - (alv1.x * alv2.z), (alv1.x * alv2.y) - (alv1.y * alv2.x)
	);
}

static vec3 reflect(vec3 v, vec3 normal)
{
	return (v - normal * 2 * dot(v, normal));
}

#endif
