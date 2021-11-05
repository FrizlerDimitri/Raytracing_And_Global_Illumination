#pragma once

#include "rt.h"

struct aabb {
	vec3 min, max;
	aabb() : min(FLT_MAX), max(-FLT_MAX) {}
	void grow(vec3 v) {
		min = glm::min(v, min);
		max = glm::max(v, max);
	}
	void grow(const aabb &other) {
		min = glm::min(other.min, min);
		max = glm::max(other.max, max);
	}
};

// See Shirley (2nd Ed.), pp. 206. (library or excerpt online)
inline bool intersect(const triangle &t, const vertex *vertices, const ray &ray, triangle_intersection &info) {
	vec3 pos = vertices[t.a].pos;
	const float a_x = pos.x;
	const float a_y = pos.y;
	const float a_z = pos.z;

	pos = vertices[t.b].pos;
	const float &a = a_x - pos.x;
	const float &b = a_y - pos.y;
	const float &c = a_z - pos.z;
	
	pos = vertices[t.c].pos;
	const float &d = a_x - pos.x;
	const float &e = a_y - pos.y;
	const float &f = a_z - pos.z;
	
	const float &g = ray.d.x;
	const float &h = ray.d.y;
	const float &i = ray.d.z;
	
	const float &j = a_x - ray.o.x;
	const float &k = a_y - ray.o.y;
	const float &l = a_z - ray.o.z;

	float common1 = e*i - h*f;
	float common2 = g*f - d*i;
	float common3 = d*h - e*g;
	float M 	  = a * common1  +  b * common2  +  c * common3;
	float beta 	  = j * common1  +  k * common2  +  l * common3;

	common1       = a*k - j*b;
	common2       = j*c - a*l;
	common3       = b*l - k*c;
	float gamma   = i * common1  +  h * common2  +  g * common3;
	float tt    = -(f * common1  +  e * common2  +  d * common3);

	beta /= M;
	gamma /= M;
	tt /= M;

	if (tt > ray.t_min && tt < ray.t_max)
		if (beta > 0 && gamma > 0 && beta + gamma <= 1)
		{
			info.t = tt;
			info.beta = beta;
			info.gamma = gamma;
			return true;
		}

	return false;
}	


inline bool intersect(const aabb &box, const ray &ray, float &is) {
	// todo

	

	vec3 Min=box.min;
	vec3 Max=box.max;

	vec3 o=ray.o;
	vec3 d=ray.d;

	//TODO Fragen wegen der nebenbedingung 
	if(d.x==0 &&((o.x < Min.x) || o.x > Max.x ) )
	{
		return false;		
	}

	if(d.y==0 &&((o.y < Min.y) || o.y > Max.y ))
	{
		return false;
	}

	if (d.z==0 &&((o.z < Min.z) || o.z > Max.z ))
	{
		return false;
	}
	
	float tXMin=(Min.x-o.x)/d.x;
	float tXMax=(Max.x-o.x)/d.x;

	float tYMin=(Min.y-o.y)/d.y;
	float tYMax=(Max.y-o.y)/d.y;

	float tZMin=(Min.z-o.z)/d.z;
	float tZMax=(Max.z-o.z)/d.z;

	if(d.x < 0)
	{
		std::swap(tXMax,tXMin);
	}

	if(d.y < 0)
	{
		std::swap(tYMax,tYMin);
	}
	
	if(d.z < 0)
	{
		std::swap(tZMax,tZMin);
	}

	if(tXMin>tYMax || tYMin>tXMax)
	{
		return false;
	}

	if(tXMin > tZMax || tZMin > tXMax)
	{
		return false;
	}

	
	float tnear=FLT_MAX;//infinity
	float arr[]={tXMin,tYMin,tZMin};

	for(int i=0; i<3 ; i++)
	{
		if(arr[i] < tnear)
		{
			tnear=arr[i];
		}
	}

	is=tnear;


	return true;
}

inline bool intersect2(const aabb &box, const ray &ray, float &is) {
	// todo
	return false;
}


inline bool intersect3(const aabb &box, const ray &ray, float &is) {
	// todo
	return false;
}

inline bool intersect4(const aabb &box, const ray &ray, float &is) {
	// todo
	return false;
}

