#include "libgi/rt.h"
#include "libgi/camera.h"
#include "libgi/scene.h"
#include "libgi/intersect.h"
#include "libgi/framebuffer.h"
#include "libgi/context.h"
#include "libgi/timer.h"

#include "libgi/global-context.h"

#include "rt/bbvh-base/bvh.h"

#include "interaction.h"

#include "cmdline.h"

#include <png++/png.hpp>
#include <iostream>
#include <chrono>
#include <cstdio>
#include <omp.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>


using namespace std;
using namespace glm;
using namespace png;

rgb_pixel to_png(vec3 col01) {
	col01 = clamp(col01, vec3(0), vec3(1));
	col01 = pow(col01, vec3(1.0f/2.2f));
	return rgb_pixel(col01.x*255, col01.y*255, col01.z*255);
}

/*! \brief This is called from the \ref repl to compute a single image
 *  
 */
void run(gi_algorithm *algo) {
	using namespace std::chrono;
	algo->prepare_frame();
	test_camrays(rc->scene.camera);
	rc->framebuffer.clear();

	algo->compute_samples();
	algo->finalize_frame();
	
	rc->framebuffer.png().write(cmdline.outfile);
}

void rt_bench() {
	//create Buffer for rays and intersections with the size of the camera resolution
	buffer<triangle_intersection> triangle_intersections(rc->scene.camera.w, rc->scene.camera.h);
	buffer<ray> rays(rc->scene.camera.w, rc->scene.camera.h);
	
	//init Buffer with Camera rays
	rays.for_each([&](unsigned x, unsigned y) {
		rays(x, y) = cam_ray(rc->scene.camera, x, y);
	});
	
	//calculate closest triangle intersection for each ray
	raii_timer bench_timer("rt_bench");
	rays.for_each([&](unsigned x, unsigned y) {
		triangle_intersections(x, y) = rc->scene.single_rt->closest_hit(rays(x, y));
	});
}

int main(int argc, char **argv)
{

	//const aabb &box, const ray &ray, float &is


	//aabb box;
	//box.min=vec3{2,0,0};
	//box.max=vec3{6,4,0};

	//vec3 o{0,0,0};
	//vec3 d{2,5,0};
	//ray ray{o,d};

	//float is=0;

	//bool b=intersect(box,ray,is);

	//if(b)
	//{
	//	cout<<"in box \n";
	//}else{
	//	cout<<"not in box \n";
	//}



	aabb scence_bounds;
	scence_bounds.min=vec3(0,0,0);
	scence_bounds.max=vec3(6,4,0);
	
	triangle t1;
	t1.a=0;
	t1.b=1;
	t1.c=2;

	triangle t2;
	t2.a=3;
	t2.b=4;
	t2.c=5;

	vertex v0;
	v0.pos=vec3(1,1,0);

	vertex v1;
	v1.pos=vec3(1,2,0);

	vertex v2;
	v2.pos=vec3(2,1,0);

	vertex v3;
	v3.pos=vec3(1,1,1);

	vertex v4;
	v4.pos=vec3(1,2,1);


	vertex v5;
	v5.pos=vec3(2,1,1);


	vector<triangle> triangels={t1,t2};
	vector<vertex> vertexs={v0,v1,v2,v3,v4,v5};


	scene scene;
	scene.scene_bounds=scence_bounds;

	scene.triangles=triangels;
	scene.vertices=vertexs;
	
	

	naive_bvh bhv;
	bhv.build(&scene);


	// //for(vertex v : vertexs)
	// //{
	// //	std::cout << v.pos << std::endl;
	// //}


	// naive_bvh::node root = bhv.nodes[0];

	// naive_bvh::node left = bhv.nodes[root.left];
	// vec3 vec=left.box.min;
	//std::cout<< "left_min : " << vec << std::endl;

	//for(int i=0; i<bhv.nodes.size();i++)
	//{/
	//	//std::cout<<"node "<<i<<" box_min = " << bhv.nodes[i].box.min << " box_max = " <<bhv.nodes[i].box.max<<std::endl;
	///}



	
	// parse_cmdline(argc, argv);

	// repl_update_checks uc;
	// if (cmdline.script != "") {
	// 	ifstream script(cmdline.script);
	// 	repl(script, uc);
	// }
	// if (cmdline.interact)
	// 	repl(cin, uc);

	// stats_timer.print();

	// delete rc->algo;
	return 0;
}
