#include "ray.h"
#include "material.h"
#include "light.h"
#include "../ui/TraceUI.h"
#include <iostream>
using namespace std;
// Apply the phong model to this point on the surface of the object, returning
// the color of that point.

extern class TraceUI* traceUI;

vec3f Material::shade(Scene* scene, const ray& r, const isect& i) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
	// You will need to call both distanceAttenuation() and shadowAttenuation()
   // somewhere in your code in order to compute shadows and light falloff.

	vec3f I; I[0] = 0; I[1] = 0; I[2] = 0; 
	vec3f intersection = r.at(i.t);
	vec3f Ia; //ambient intensity;
	Ia[0] = 0; Ia[1] = 0; Ia[2] = 0;

	list<AmbientLight*> ambLitghts = scene->GetAmbLitghts();
	vec3f trans = (vec3f(1, 1, 1) - kt);
	for (std::list<AmbientLight*>::iterator it = ambLitghts.begin(); it != ambLitghts.end(); ++it) {

		Ia = Ia + (*it)->getAmbCol();

	}
	double k2 = traceUI->getAmbLight();
	vec3f kaScale = ka*k2;
//	ka = prod(ka, kaScale);
	//cout << kaScale[0] << " " << kaScale[1] << " " << kaScale[2] << endl;
	
	I = ke + prod(prod(Ia,trans), ka);
	//testing
	vec3f temp;
	temp[0] = 0.5;
	temp[1] = 0.5;
	temp[2] = 0.5;

	for (std::list<Light*>::const_iterator it = scene->beginLights(); it != scene->endLights(); ++it) {

		int n = 128;
		vec3f V = r.getDirection();
		vec3f L = (*it)->getDirection(intersection);
		
		vec3f satten = (*it)->shadowAttenuation(intersection);
		
		vec3f atten = (*it)->distanceAttenuation(intersection) * (*it)->shadowAttenuation(intersection);

		vec3f lightColor = (*it)->getColor(intersection);

		double LdotN = i.N.dot((*it)->getDirection(intersection));
		if (LdotN < 0.0) { LdotN = 0; }
		
		vec3f diff = prod(kd* LdotN, vec3f(1,1,1));
		vec3f R = (2 * i.N * (LdotN)-L);
		R = R.normalize();
		double RdotV = R.dot(-V);
		if (RdotV < 0) { RdotV = 0; }
		
		//I = I + prod(prod(atten, lightColor), (diff + (ks * pow(RdotV, n))));
		I = I + prod(prod(atten, lightColor), (prod(diff,trans) + (ks * pow(RdotV, n))));
	}
	cout << I << endl;
	return I;
}
