#define _USE_MATH_DEFINES // Enable M_PI
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>

#include "vec3.h"
#include "ray.h"
#include "triangle.h"
#include "scene_object.h"
#include "rectangle.h"
#include "light.h"

#define BIAS 0.00001f

const vec3 background_col(0.0f, 0.0f, 0.0f);

vec3 alv0(-.5f,  2.f, -5.5f); // Top left
vec3 alv1(-.5f,  2.f, -6.5f); // Bottom left
vec3 alv2( .5f,  2.f, -6.5f); // Bottom right
vec3 alv3( .5f,  2.f, -5.5f); // Top right

area_light light(
	triangle(alv0, alv1, alv2),
	triangle(alv0, alv2, alv3),
	vec3(1.0f, 1.0f, 1.0f), 80.f
);

std::vector<scene_object*> scene_objects;

bool has_los(vec3, vec3);
vec3 estimate_direct_light(const vec3&, const vec3&, const ray&, const scene_object&);
vec3 estimate_indirect_light(const vec3&, const vec3&, const ray&, const scene_object&, bool);
vec3 trace(const ray&, bool);
void init_scene(void);

// Check LoS between point 1 and point 2
bool has_los(vec3 p1, vec3 p2)
{
	float dist = length(p2 - p1);
	ray r(p1, normalise(p2 - p1));

	for (auto so : scene_objects)
	{
		float check_dist;
		if ((check_dist = so->intersects(r)) && check_dist < dist)
			return false;
	}

	return true;
}

vec3 estimate_direct_light(const vec3& xsect, const vec3& n, const ray& eye, const scene_object& so)
{
	vec3 L_o(0.f, 0.f, 0.f);

	vec3 light_point = light.get_random_point();
	vec3 light_normal = light.get_normal();

	if (has_los(xsect + (n * BIAS), light_point + (light_normal * BIAS)))
	{
		vec3 sray_dir = light_point - xsect;
		float dist = length(sray_dir);
		vec3 sray_normalised = normalise(sray_dir);

		// Get the intensity of the light at the intersection point
		vec3 rad = (light.get_emission() * light.get_power()) / (M_PI * dist * dist);
	

		vec3 colour = so.evaluate_bsdf(sray_normalised, -eye.direction, n);

		// Multiply by the cos angle between the surface normal and the shadow ray, aswell as the light normal and the shadow ray
		// This darkens the top of the room near the ceiling, as well as the bottom.
		float d = std::max(0.f, dot(sray_normalised, n)) * std::max(0.f, dot(-sray_normalised, light_normal));

		L_o = colour * rad * d;
	}

	return L_o;
}

vec3 estimate_indirect_light(const vec3& xsect, const vec3& n, const ray& iray, const scene_object& so, bool is_eye_ray)
{
	vec3 L_o = vec3(0.f, 0.f, 0.f);

	float r = get_random_range(0.f, 1.0f);

	// Arbitrary extinction chance of 30%
	if (r < 0.7f)
	{
		vec3 t, b;

		// Create a local coordinate frame around the surface, get a random vector in a hemisphere
		// multiply the random vector by the local coordinate frame to get the global vector
		create_coordinate_frame(n, t, b);
		vec3 sample = sample_hemisphere();

		vec3 random_vec(
			sample.x * b.x + sample.y * n.x + sample.z * t.x,
			sample.x * b.y + sample.y * n.y + sample.z * t.y,
			sample.x * b.z + sample.y * n.z + sample.z * t.z
		);

		ray ra(xsect, normalise(random_vec));

		return trace(ra, false);
	}

	return L_o;
}

vec3 trace(const ray& eye, bool is_eye_ray)
{
	vec3 L_o(0.f, 0.f, 0.f);

	// If an eye ray hits the light source, we can just return the light colour
	if (is_eye_ray)
	{
		if (light.intersects(eye) != INFINITY)
		{
			L_o += light.get_emission();
			return L_o;
		}
	}

	// Find the scene object that we hit
	for (auto so : scene_objects)
	{
		float dist = so->intersects(eye);
		if (dist != INFINITY)
		{
			vec3 xsect = eye.origin + (eye.direction * dist);
			vec3 normal = so->get_normal();

			L_o += estimate_direct_light(xsect, normal, eye, *so);

			L_o += estimate_indirect_light(xsect, normal, eye, *so, is_eye_ray);

			break; // There're no things that block anything in this scene, so the first interesection is going to be the closest
		}
	}

	return L_o;
}

// Set up Cornell box
void init_scene(void)
{
	vec3 box_top_left_front(  -2.f, 2.f, -4.f);
	vec3 box_top_left_back(   -2.f, 2.f, -8.f);
	vec3 box_top_right_front(  2.f, 2.f, -4.f);
	vec3 box_top_right_back(   2.f, 2.f, -8.f);

	vec3 box_bot_left_front( -2.f, -2.f, -4.f);
	vec3 box_bot_left_back(  -2.f, -2.f, -8.f);
	vec3 box_bot_right_front( 2.f, -2.f, -4.f);
	vec3 box_bot_right_back(  2.f, -2.f, -8.f);

	scene_objects.push_back(
		new scene_object(
			"back_wall",
			rectangle(triangle(box_top_left_back, box_bot_left_back, box_bot_right_back), triangle(box_top_left_back, box_bot_right_back, box_top_right_back)),
			bsdf(vec3(1.f, 1.f, 1.f), vec3(0.f, 0.f, 0.f), 0.f)
		)
	);

	scene_objects.push_back(
		new scene_object(
			"left_wall",
			rectangle(triangle(box_top_left_front, box_bot_left_front, box_bot_left_back), triangle(box_top_left_front, box_bot_left_back, box_top_left_back)),
			bsdf(vec3(1.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), 0.f)
		)
	);

	scene_objects.push_back(
		new scene_object (
			"right_wall",
			rectangle(triangle(box_top_right_back, box_bot_right_back, box_bot_right_front), triangle(box_top_right_back, box_bot_right_front, box_top_right_front)),
			bsdf(vec3(.25f, 1.f, 1.f), vec3(0.f, 0.f, 0.f), 0.f)
		)
	);

	scene_objects.push_back(
		new scene_object(
			"ceiling",
			rectangle(triangle(box_top_left_front, box_top_left_back, box_top_right_back), triangle(box_top_left_front, box_top_right_back, box_top_right_front)),
			bsdf(vec3(1.f, 1.f, 1.f), vec3(0.f, 0.f, 0.f), 0.f)
		)
	);

	scene_objects.push_back(
		new scene_object(
			"ground",
			rectangle(triangle(box_bot_left_back, box_bot_left_front, box_bot_right_front), triangle(box_bot_left_back, box_bot_right_front, box_bot_right_back)),
			bsdf(vec3(1.f, 1.f, 1.f), vec3(0.f, 0.f, 0.f), 0.f)
		)
	);
}

int main(int argc, char** argv)
{
	srand(time(0));

	// Setup
	int width = 1920;
	int height = 1080;
	float inverse_width = 1.f / float(width);
	float inverse_height = 1.f / float(height);
	float fov = 50.f; // Angle in degrees
	float aspect_ratio = (float)width / (float)height;
	float opposite = tanf((fov * 0.5f) * (M_PI / 180.f)); // Half length of plane opposite the frustum origin
	int samples = 1;

	std::string filename = "scene.ppm";
	std::ofstream ofs(filename, std::ios::out);
	ofs << "P3" << std::endl << width << " " << height << std::endl << "255" << std::endl;

	init_scene();

	// Main loop
	for (int y = 0; y < height; y++)
	{
		std::cout << y << std::endl;

		for (int x = 0; x < width; x++)
		{
			vec3 output;
			for (int i = 0; i < samples; i++)
			{
				float l = 0.f;
				float x_off = 0.f;
				float y_off = 0.f;

				// Force a point to be in the circle bounded by the pixel
				do
				{
					x_off = get_random_range(0.f, 1.f);
					y_off = get_random_range(0.f, 1.f);
					vec3 centre(0.5f, 0.5f, 0.f);

					vec3 off(x_off, y_off, 0.f);

					l = length(centre - off);
				} 
				while (l > 0.5f);		

				// Map x,y to range -tan(fov) to tan(fov)
				float xx = (2.f * (x + x_off) * inverse_width - 1.f) * aspect_ratio * opposite;
				float yy = (1.f - 2.f * (y + y_off) * inverse_height) * opposite;

				// Create a eye ray starting at the view position (0.f, 0.f, 0.f), directed at the position of the pixel
				ray r(vec3(0.f, 0.f, 0.f), normalise(vec3(xx, yy, -1.f)));

				output += trace(r, true);	
			}

			output /= (float)samples;

			// Clamp output
			if (output.x > 1.f) output.x = 1.f;
			if (output.y > 1.f) output.y = 1.f;
			if (output.z > 1.f) output.z = 1.f;

			ofs << output.x * 255.f << " " << output.y * 255.f << " " << output.z * 255.f << " ";
		}
		
		ofs << std::endl;
	}

	for (auto so : scene_objects)
		delete so;

	return 0;
}