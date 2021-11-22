#include "bvh.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <stack> 

using namespace glm;

// 
//    naive_bvh
//


void naive_bvh::build(::scene *scene) {
	this->scene = scene;
	std::cout << "Building BVH..." << std::endl; 
	auto t1 = std::chrono::high_resolution_clock::now();


	root = subdivide(scene->triangles, scene->vertices, 0, scene->triangles.size());
	std::cout<<"end"<<std::endl;

	
	auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "Done after " << duration << "ms" << std::endl;




	//test test test remove after test 

	ray ray;
	ray.o=vec3(0);
	ray.d=vec3(5,3,0);

	std::cout << "closest hit : "<< closest_hit(ray).t << std::endl;
}



vec3 triangle_middle(vec3 a , vec3 b , vec3 c )
{
	return (a +b +c)*vec3(1.0f/3.0f, 1.0f/3.0f, 1.0f/3.0f);
}


uint32_t naive_bvh::subdivide(std::vector<triangle> &triangles, std::vector<vertex> &vertices, uint32_t start, uint32_t end) {
	// todo
	std::cout<<"subdivide"<<std::endl;
	// recursiv end : diffrenece between triangel is 1 
	aabb box;
	//box.max=vec3(-FLT_MAX, -FLT_MAX,-FLT_MAX);

	for(int i=start;i<end; i++)
	{

		float x1=vertices[triangles[i].a].pos.x;
		float x2=vertices[triangles[i].b].pos.x;
		float x3=vertices[triangles[i].c].pos.x;

		float y1=vertices[triangles[i].a].pos.y;
		float y2=vertices[triangles[i].b].pos.y;
		float y3=vertices[triangles[i].c].pos.y;

		float z1=vertices[triangles[i].a].pos.z;
		float z2=vertices[triangles[i].b].pos.z;
		float z3=vertices[triangles[i].c].pos.z;

		float x_min=min(x1,min(x2,x3));
		float x_max=max(x1,max(x2,x3));

		float y_min=min(y1,min(y2,y3));
		float y_max=max(y1,max(y2,y3));

		float z_min=min(z1,min(z2,z3));
		float z_max=max(z1,max(z2,z3));

		vec3 min={x_min,y_min,z_min};
		vec3 max= {x_max,y_max,z_max};

		if (box.max.x<max.x)
			box.max.x=max.x;
		if (box.max.y<max.y)
			box.max.y=max.y;
		if (box.max.z<max.z)
			box.max.z=max.z;

		if (box.min.x>min.x)
			box.min.x=min.x;
		if (box.min.y>min.y)
			box.min.y=min.y;
		if (box.min.z>min.z)
			box.min.z=min.z;	
	}

		if(end-start ==1)
	{
		int index=nodes.size();
		nodes.emplace_back();

		nodes[index].box=box;
		return index;
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
	

	for (int i =0; i<triangles.size(); i++)
	{
		std::cout<< "triangle a  : " << triangles[i].a << std::endl;
		std::cout<< "triangle c  : " << triangles[i].b << std::endl;
		std::cout<< "triangle c  : " << triangles[i].c << std::endl;
	}


	int mid=start+(end-start)/2;
	
	int index= nodes.size();

	std::cout<<"index: "<<index << ", mid "<<mid<<std::endl;

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

	//todo
	triangle_intersection closest;
	triangle_intersection intersection;

	std::stack <int> stack;

	std::vector<triangle> triangles= scene->triangles;
	std::vector<vertex> vertecies=scene->vertices;

	stack.push(root);

		while (!stack.empty())
		{
			int i = stack.top();
			stack.pop();

			naive_bvh::node node = nodes[i];

			if(node.inner())
			{

				//has Ray hit left Node ? Yes Pust other Node to stack, if not check right Node 
				float dist = FLT_MAX;

				if(intersect(node.box, ray, dist))
				{
					// if dist <closesthit.t ? 
					// why in soulution left first than right ? 
					if(dist < closest.t)
					{
						stack.push(node.right);
						stack.push(node.left);
					}
				} 

			}else{

				int t_index=node.triangle;
				triangle triangle = triangles[t_index];

				//intersect(const triangle &t, const vertex *vertices, const ray &ray, triangle_intersection &info) 
				// does the ray hit the triangle ? if hit than update t_min 
				
				bool hasRayHitTriangle=intersect(triangle, &vertecies[0], ray, intersection);
				if(hasRayHitTriangle)
				{
					if(intersection.t < closest.t)
					{
						closest = intersection;
						closest.ref=node.triangle;
						//what is t in triangle_intersection ?
					}
				}
				
			}
		}
	
	int i=0;
	//throw std::logic_error("Not implemented, yet");
	return closest;
}


bool naive_bvh::any_hit(const ray &ray) {
	auto is = closest_hit(ray);
	if (is.valid())
		return true;
	return false;
}

