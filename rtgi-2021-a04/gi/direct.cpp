#include "direct.h"

#include "libgi/rt.h"
#include "libgi/context.h"
#include "libgi/intersect.h"
#include "libgi/util.h"
#include "libgi/color.h"
#include "libgi/sampling.h"

#include "libgi/global-context.h"

using namespace glm;
using namespace std;

gi_algorithm::sample_result direct_light::sample_pixel(uint32_t x, uint32_t y, uint32_t samples) {
	sample_result result;
	for (int sample = 0; sample < samples; ++sample) {
		vec3 radiance(0,0,0);
		ray view_ray = cam_ray(rc->scene.camera, x, y, glm::vec2(rc->rng.uniform_float()-0.5f, rc->rng.uniform_float()-0.5f));
		triangle_intersection closest = rc->scene.single_rt->closest_hit(view_ray);
		if (closest.valid()) {
			diff_geom dg(closest, rc->scene);
			flip_normals_to_ray(dg, view_ray);

			// todo: compute direct lighting contribution
			if (dg.mat->emissive != vec3(0))
			{
				radiance = dg.mat->emissive;
			}
			else
			{
				brdf *brdf = dg.mat->brdf;
				// auto col = dg.mat->albedo_tex ? dg.mat->albedo_tex->sample(dg.tc) : dg.mat->albedo;
				if (sampling_mode == sample_uniform)
					radiance = sample_uniformly(dg, view_ray);
				//else if (sampling_mode == sample_light)
				//	radiance = sample_lights(dg, view_ray);
			}

		}
		result.push_back({radiance,vec2(0)});
	}
	return result;
}

vec3 direct_light::sample_uniformly(const diff_geom &hit, const ray &view_ray) {
	// set up a ray in the hemisphere that is uniformly distributed
	vec2 xi = rc->rng.uniform_float2();
	// todo: implement uniform hemisphere sampling

	float z=xi.x;  //z =cos(theta)
	float phi = 2*pi*xi.y;
	float theta = acos(z); 

	float sin_theta=sqrt(1.0f-z*z);
	float cos_phi=cos(phi);
	float sin_phi=sin(phi);

	vec3 w_i(sin_theta *cos_phi, sin_theta*sin_phi, z);
	w_i=align(w_i,hit.ng);
	ray sample_ray(hit.x, w_i);

	vec3 brightness(0);
	triangle_intersection closest = rc->scene.single_rt->closest_hit(sample_ray);
	if (closest.valid()) {
		diff_geom dg(closest, rc->scene);
		brightness = dg.mat->emissive;
	}

	float cos_theta_i=cdot(w_i, hit.ns);		
	return 2*pi*brightness * hit.mat->brdf->f(hit,-view_ray.d, sample_ray.d)*cos_theta_i;

}


vec3 direct_light::sample_lights(const diff_geom &hit, const ray &view_ray) {
	// todo: implement uniform sampling on the first few of the scene's lights' surfaces
	return vec3(0);
}


bool direct_light::interprete(const std::string &command, std::istringstream &in) {
	string value;
	if (command == "is") {
		in >> value;
		if (value == "uniform") sampling_mode = sample_uniform;
		else if (value == "cosine") sampling_mode = sample_cosine;
		else if (value == "light") sampling_mode = sample_light;
		else if (value == "brdf") sampling_mode = sample_brdf;
		else cerr << "unknown sampling mode in " << __func__ << ": " << value << endl;
		return true;
	}
	return false;
}



