#include <cmath>
#include <assert.h>

#include "Box.h"
#include <iostream>

bool Box::intersectLocal(const ray& r, isect& i) const
{
	// YOUR CODE HERE:
	// Add box intersection code here.
	// it currently ignores all boxes and just returns false.


	
	double tmax = std::numeric_limits<double>::max();
	double tmin = -std::numeric_limits<double>::max(); 
	vec3f ux = vec3f(1, 0, 0);
	vec3f uy = vec3f(0, 1, 0);
	vec3f uz = vec3f(0, 0, 1);
	vec3f lbound = vec3f(-0.5, -0.5, -0.5);
	vec3f ubound = vec3f(0.5, 0.5, 0.5);
	vec3f dir = r.getDirection();
	vec3f pos = r.getPosition();
	int ax= 0;
	i.obj = this;


	if (dir.cross(ux).length() == 0 || dir.cross(uy).length() == 0 || dir.cross(uz).length() == 0) {
		
		for (int j = 0; j < 3; j++) {
		
			if (dir[j]<lbound[j] || dir[j]>ubound[j]) { return false; }
		
		}
	
	}

	
	for (int k = 0; k < 3; k++) {

		double t0 = ((lbound[k] - pos[k]) / dir[k]);
		double t1 = ((ubound[k] - pos[k]) / dir[k]);
		if (t0 > t1) swap(t0, t1);
		if (t0 > tmin) { tmin = t0; ax = k; }
		if (t1 < tmax) tmax = t1;
		if (tmin > tmax) return false;
		if (tmax < 0) return false;
		

	}

	i.t = tmin;
	if (ax == 0) {
		if (dir[0] < 0) {
			i.N = vec3f(1, 0, 0);
		}
		else { i.N = vec3f(-1, 0, 0); }
	}

	if (ax == 1) {
		if (dir[1] < 0) {
			i.N = vec3f(0, 1, 0);
		}
		else { i.N = vec3f(0, -1, 0); }
	}

	if (ax == 2) {
		if (dir[2] < 0) {
			i.N = vec3f(0, 0, 1);
		}
		else { i.N = vec3f(0, 0, -1); }
	}
	
	
	



    return true;
	

	

}
