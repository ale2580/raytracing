#include <cmath>

#include "light.h"
#include "../ui/TraceUI.h"
#include <iostream>
using namespace std;
double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	bool intersection;
	vec3f& L = getDirection(P);
	vec3f atten(0, 0, 0);
	vec3f p2 = P + L * 0.00001;
	ray r(p2, L);
	isect i;
	intersection = (scene->intersect(r, i));
	if (!intersection) {
		atten = vec3f(1, 1, 1);
		return atten;
	}

	return atten;

}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation(const vec3f& P) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	
	double c1 = traceUI->getAttConst();;
	double c2 = traceUI->getAttLinear();
	double c3 = traceUI->getAttQuadratic();

	

	double dist = sqrt((position[0] * P[0]) * (position[0] * P[0]) + (position[1] * P[1]) * (position[1] * P[1]) + (position[2] * P[2]) * (position[2] * P[2]));
	double distAtt = 1 / (c1 + c2 * dist + c3 * dist * dist);
	cout << distAtt<< endl;
	if (distAtt < 1) { return distAtt; }
	else { return 1; }
	
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.

	
	
	bool intersection;
	vec3f& L = getDirection(P);
	vec3f atten(0, 0, 0);
	vec3f p2 = P + L * 0.00001;
	ray r(p2, L);
	isect i;
	intersection = (scene->intersect(r, i));
	if (!intersection) {
		atten = vec3f(1, 1, 1);
		return atten;
	}

	return atten;
	
}

vec3f AmbientLight::getAmbCol() {
	
	//return color;
	return color;
}