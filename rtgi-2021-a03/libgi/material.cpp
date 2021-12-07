#include "material.h"
#include "scene.h"
#include "util.h"
#include <iostream>

using namespace glm;

vec3 layered_brdf::f(const diff_geom &geom, const vec3 &w_o, const vec3 &w_i) {
	// real world materials are neither perfectly specular or purely diffuse
	// simple adding diff + spec is problematic as normalization is not easy
	//vec3 diff = base->f(geom, w_o, w_i);
	//vec3 spec = coat->f(geom, w_o, w_i);
	//return diff+spec;
	
	
	// todo proper fresnel reflection for layered material
	vec3 diff = base->f(geom, w_o, w_i);
	vec3 spec = coat->f(geom, w_o, w_i);

	float R =  fresnel_dielectric(absdot(geom.ns, w_o), 1.0f, geom.mat->ior);

	return (1-R)*diff+R*spec;
}


// lambertian_reflection

vec3 lambertian_reflection::f(const diff_geom &geom, const vec3 &w_o, const vec3 &w_i) {
	if (!same_hemisphere(w_i, geom.ns)) return vec3(0);

	//todo
	//return vec3(0);

 	vec3 p=geom.albedo();
	return p/pi;

}


// specular_reflection

vec3 phong_specular_reflection::f(const diff_geom &geom, const vec3 &w_o, const vec3 &w_i) {
	if (!same_hemisphere(w_i, geom.ng)) return vec3(0);

	float exponent = exponent_from_roughness(geom.mat->roughness);
	vec3 r = 2.0f * geom.ns * dot(geom.ns, w_i) - w_i;
	float cos_theta = cdot(w_o, r);
	const float norm_f = (exponent + 2.0f) * one_over_2pi;
	return (coat ? vec3(1) : geom.albedo()) * powf(cos_theta, exponent) * norm_f * cdot(w_i,geom.ns); //modified phong brdf ? 


}




// In the following, make sure that corner cases are taken care of (positive
// dot products, >0 denominators, correctly aligned directions, etc)


#define sqr(x) ((x)*(x))

inline float A(float theta, float m)
{
	float A=(1/2) *(sqrt(1+pow(m,2) *pow(tan(theta), 2 ) -1));  
	return A;
}


inline float ggx_d(const float NdotH, float roughness) {

	float m = roughness;
	float cos_theta_h=NdotH;

	return ((m*m)/pi) * pow((1+(m*m-1) * cos_theta_h*cos_theta_h ),-2);
}

inline float ggx_g1(const float NdotV, float roughness) {

	float omega=NdotV;

	if(omega<0)
	{
		return 0.0f;
	}
	float m =roughness;

	return 1/(1+A(omega,m));
}


vec3 gtr2_reflection::f(const diff_geom &geom, const vec3 &w_o, const vec3 &w_i) {


	if (!same_hemisphere(geom.ng, w_i)) return vec3(0);

	vec3 N = geom.ng;
	vec3 L=w_i;
	vec3 V=w_o;
	vec3 H = normalize(V+L);

	float HdotL =cdot(H,L);


	// a dot b = |a| * |b| *  cos(phi)  ->  |a| =1 und |b|=1

	float cos_theta_o=cdot(N, V);
	float cos_theta_i=cdot(N, L);
	float cos_theta_h=cdot(N,H);

	const float roughness = geom.mat->roughness;

	const float G = ggx_g1(cos_theta_o, roughness) * ggx_g1(cos_theta_i, roughness);
	const float F = fresnel_dielectric(HdotL, 1.f, geom.mat->ior);
	const float D = ggx_d(cos_theta_h, roughness);
	const float microfacet = (F * D * G) / (4 * abs(cos_theta_o) * abs(cos_theta_i));

    return coat ? vec3(microfacet) : geom.albedo() * microfacet;

}



brdf *new_brdf(const std::string name, scene &scene) {
	if (scene.brdfs.count(name) == 0) {
		brdf *f = nullptr;
		if (name == "lambert")
			f = new lambertian_reflection;
		else if (name == "phong")
			f = new phong_specular_reflection;
		else if (name == "layered-phong") {
			brdf *base = new_brdf("lambert", scene);
			specular_brdf *coat = dynamic_cast<specular_brdf*>(new_brdf("phong", scene));
			assert(coat);
			f = new layered_brdf(coat, base);
		}
		else if (name == "gtr2")
			f = new gtr2_reflection;
		else if (name == "layered-gtr2") {

			brdf *base = new_brdf("lambert", scene);
			specular_brdf *coat = dynamic_cast<specular_brdf*>(new_brdf("gtr2", scene));
			assert(coat);
			f = new layered_brdf(coat, base);
			//throw std::runtime_error(std::string("Not implemented yet: ") + name);
		}
		else
			throw std::runtime_error(std::string("No such brdf defined: ") + name);
		return f;
	}
	return scene.brdfs[name];
}

