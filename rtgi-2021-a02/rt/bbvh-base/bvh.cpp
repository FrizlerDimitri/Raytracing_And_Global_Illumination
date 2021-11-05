#include "bvh.h"

#include <algorithm>
#include <iostream>
#include <chrono>

using namespace glm;

// 
//    naive_bvh
//


void naive_bvh::build(::scene *scene) {
	this->scene = scene;
	std::cout << "Building BVH..." << std::endl;
	auto t1 = std::chrono::high_resolution_clock::now();

	root = subdivide(scene->triangles, scene->vertices, 0, scene->triangles.size());
	
	auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "Done after " << duration << "ms" << std::endl;
}



vec3 triangle_middle(vec3 a , vec3 b , vec3 c )
{
	return (a +b +c)*vec3(1.0f/3.0f, 1.0f/3.0f, 1.0f/3.0f);
}


uint32_t naive_bvh::subdivide(std::vector<triangle> &triangles, std::vector<vertex> &vertices, uint32_t start, uint32_t end) {
	// todo
	
	// recursiv end : diffrenece between triangel is 1 
	if(end-start ==1)
	{
		int index=nodes.size();
		nodes.emplace_back();
		nodes[index].triangle=start;
		return index;
	}


	aabb box;
	for(int i=start;i<end; i++)
	{
		aabb other;
		other.grow(vertices[triangles[i].a].pos);
		other.grow(vertices[triangles[i].b].pos);
		other.grow(vertices[triangles[i].b].pos);
		box.grow(other);
	}

	vec3 axis_helper=box.max-box.min;
	//find largest axis x,y or z to sort after it
	float largest=max(axis_helper.x, max(axis_helper.y,axis_helper.z));
	if(largest==axis_helper.x)
	{	//[] -> Lambda 
		//sort after x axis
		std::sort(triangles.data()+start,triangles.data()+end, [&](triangle &t1, triangle &t2) {

			return triangle_middle(vertices[t1.a].pos,vertices[t1.b].pos,vertices[t1.c].pos).x < 
			triangle_middle(vertices[t2.a].pos,vertices[t2.b].pos,vertices[t2.c].pos).x  
			;
		});
	}else if (largest==axis_helper.y)
	{	//sort after y axis	
				std::sort(triangles.data()+start,triangles.data()+end, [&](triangle &t1, triangle &t2) {

			return triangle_middle(vertices[t1.a].pos,vertices[t1.b].pos,vertices[t1.c].pos).y < 
			triangle_middle(vertices[t2.a].pos,vertices[t2.b].pos,vertices[t2.c].pos).y  
			;
		});
	}else
	{	//sort after z axis
				std::sort(triangles.data()+start,triangles.data()+end, [&](triangle &t1, triangle &t2) {

				return	triangle_middle(vertices[t1.a].pos,vertices[t1.b].pos,vertices[t1.c].pos).z < 
					triangle_middle(vertices[t2.a].pos,vertices[t2.b].pos,vertices[t2.c].pos).z  
					;
		});
	}
	
	int mid=start+(end-start)/2;
	
	int index= nodes.size();
	nodes.emplace_back();
	int l=subdivide(triangles,vertices, start, mid);
	int r=subdivide(triangles,vertices, mid, end);
	nodes[index].left=l;
	nodes[index].right=r;
	nodes[index].box=box;

	return index;
	
	//throw std::logic_error("Not implemented, yet");
	//return 0;
}

triangle_intersection naive_bvh::closest_hit(const ray &ray) {
	// todo
	throw std::logic_error("Not implemented, yet");
	return triangle_intersection();
}


bool naive_bvh::any_hit(const ray &ray) {
	auto is = closest_hit(ray);
	if (is.valid())
		return true;
	return false;
}

