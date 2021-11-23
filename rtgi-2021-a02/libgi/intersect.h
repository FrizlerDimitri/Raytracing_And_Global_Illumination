#pragma once

#include "rt.h"
#include <iostream>

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

	std::cout<<"Ray Triangle Intersection"<<std::endl;
	std::cout<<"M = "<<M<<std::endl;
	std::cout<<"beta = "<<beta<<std::endl;
	std::cout<<"gamma = "<<gamma<<std::endl;
	std::cout<<"tt = "<<tt<<std::endl;

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



//--------------------------------------------------------------------------------------------------


inline float det3x3(float m [3][3]){
	//rule of sarrus
	float ret=0;
	//for (int i=0 ; i<3;i++)
	//{
	//	for(int j=0; j<3;j++)
	//		{
	//		std::cout<<" " << m[i][j]<<"";
	//	}
	//	std::cout<<"\n";
	//}

	ret =	
		m[0][0]*m[1][1]*m[2][2] 
	+
		m[0][1]*m[1][2]*m[2][0] 

	+	m[0][2]*m[1][0]*m[2][1]

	-   m[2][0]*m[1][1]*m[0][2] 

	-   m[2][1]*m[1][2]*m[0][0] 

	-   m[2][2]*m[1][0]*m[0][1] ;

	//std::cout<<"det : "<<ret<<"\n";

	return ret;
}


inline bool intersectDimitri(const triangle &t, const vertex *vertices, const ray &ray, triangle_intersection &info) {
	// todo
	

	vec3 a=vertices[t.a].pos;
	vec3 b=vertices[t.b].pos;
	vec3 c=vertices[t.c].pos;

	float a_x=a.x;
	float a_y=a.y;
	float a_z=a.z;
	std::cout<<"A { " << a_x<<" , "<< a_y <<" , " << a_z <<"}\n";
	 
	float b_x=b.x;
	float b_y=b.y;
	float b_z=b.z;
	std::cout<<"B { " << b_x<<" , "<< b_y <<" , " << b_z <<"}\n";

	float c_x=c.x;
	float c_y=c.y;
	float c_z=c.z;
	std::cout<<"C { " << c_x<<" , "<< c_y <<" , " << c_z <<"}\n";

	vec3 o=ray.o;
	float o_x=o.x;
	float o_y=o.y;
	float o_z=o.z;
	std::cout<<"o { " << o_x<<" , "<< o_y <<" , " << o_z <<"}\n";

	vec3 d=ray.d;
	float d_x=d.x;
	float d_y=d.y;
	float d_z=d.z;
	std::cout<<"d { " << d_x<<" , "<< d_y <<" , " << d_z <<"}\n";

	//solution of x
	float beta=info.beta;
	float gamma=info.gamma;
	float t1=info.t;

	float matrixA[3][3]={
		{a_x-b_x   , a_x-c_x   ,   d_x},
		{a_y-b_y   , a_y-c_y   ,   d_y},
		{a_z-b_z   , a_z-c_z   ,   d_z}

	}; 

	float Matrix_b[3]={
		{ a_x-o_x },
		{ a_y-o_y },
		{ a_z-o_z }
	};

	float detA=det3x3(matrixA);
	std::cout<<"detA : "<<detA<<std::endl;


	//rule of cramer
	float matrixBeta [3][3]= {
		{Matrix_b[0] , a_x-c_x   ,d_x },
		{Matrix_b[1] , a_y-c_y   ,d_y },
		{Matrix_b[2] , a_z-c_z   ,d_z }
	}; 


	float matrixGamma[3][3]={
		{a_x-b_x   , Matrix_b[0]  ,    d_x},
		{a_y-b_y   , Matrix_b[1]   ,   d_y},
		{a_z-b_z   , Matrix_b[2]   ,   d_z}

	}; 

	float matrixT[3][3]={
		{a_x-b_x   , a_x-c_x   ,   Matrix_b[0]},
		{a_y-b_y   , a_y-c_y   ,   Matrix_b[1]},
		{a_z-b_z   , a_z-c_z   ,   Matrix_b[2]}
	}; 
	
	beta = det3x3(matrixBeta)/detA;
	std::cout<<"beta : "<<beta<<"\n";

	gamma=det3x3(matrixGamma)/detA;
	std::cout<<"gamma : "<<gamma<<"\n";

	t1=det3x3(matrixT)/detA;
	std::cout<<"t: "<<t1<<"\n";

	

	//if(t1 > ray.t_min && t1 < ray.t_max)
	//{
	//	if(beta > 0 && gamma > 0 && beta+gamma <=1)
	//	{
	//		info.t=t1;
	//		info.beta=beta;
	//		info.gamma=gamma;
	//		return true;
	//	}
	//}
	//std::cout<<"Matrix should compute here \n";


	//std::cout<< "tmin " <<ray.t_min<<"\n";
	//std::cout<< "tmax " <<ray.t_max<<"\n";
	if(t1<ray.t_min || t1>ray.t_max)
	{
		//std::cout<<"Here 1 "<< t1 <<"\n";
		return false;
	}

	if(gamma<0 || gamma>1)
	{
		//std::cout<<"Here 2 "<< t1 <<"\n";
		return false;
	}

	if(beta<0 || beta > 1-gamma)
	{	
		//std::cout<<"Here 3 "<< beta <<"\n";
		return false;
	}


	std::cout << "t : " << t1<<std::endl;
	std::cout << "beta: " << beta<<std::endl;
	std::cout << "gamma : " << gamma<<std::endl;

	info.t=t1;
	info.beta=beta;
	info.gamma=gamma;
	return true;
}	