// The main ray tracer.

#include <Fl/fl_ask.h>

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"
#include "ui/TraceUI.h"






//mats.push(vacuum);
// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
	
    ray r( vec3f(0,0,0), vec3f(0,0,0) );
    scene->getCamera()->rayThrough( x,y,r );
	scene->getMats() = std::stack<Material>();
	Material vacuum = Material({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 1,1,1 }, 0, 1);
	scene->getMats().push(vacuum);
	return traceRay( scene, r, vec3f(1.0,1.0,1.0), 0 ).clamp();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth )
{
	isect i;
	if (traceUI->getDepth() < depth) { return vec3f(0, 0, 0); }
	if( scene->intersect( r, i ) ) {
		// YOUR CODE HERE
		
		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.
		
		const Material& m = i.getMaterial();
		vec3f n = i.N.normalize();
		
			vec3f q = r.at(i.t);
			vec3f d = r.getDirection();
			
			double dDotn = d.dot(n);
			vec3f R = (d - 2*(dDotn)*n).normalize();
			
			vec3f I = m.shade(scene, r, i); 
			double ni = scene->getMats().top().index;
			double nt = 1;
			ray r2(q, R);
			I = I + prod(traceRay(scene, r2, thresh, depth + 1), m.kr);
			// check if transmissive and whether the ray is entering or leaving

			/*
			if (m.kt[0] > 0 || m.kt[1] > 0 || m.kt[2] > 0) {
				
				if (scene->getMats().top().index == m.index) {
					scene->getMats().pop();
					 nt = scene->getMats().top().index;
					 cout << "test" << endl;
				}
				else {
					cout << " in else" << endl;
					scene->getMats().push(m);
					nt = m.index;
					
				}
			}

			double nr = ni / nt;
			double nri = nt / ni;
			double nd = i.N.dot(d);
			vec3f T = nr * ((sqrt((nd * nd) + nri * nri - 1) - nd) * i.N + d);
			vec3f Tn = T.normalize();
			vec3f Tn2 = refraction2(d, n, ni, nt);
			ray r3(q, Tn);
			
			I = I + prod(traceRay(scene, r3, thresh, depth + 1), m.kt);
			*/
		//return m.shade(scene, r, i);
		return I;
	
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.

		return vec3f( 0.0, 0.0, 0.0 );
	}
}

RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

	m_bSceneLoaded = false;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	
	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
	}
	memset( buffer, 0, w*h*3 );
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;

	if( !scene )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	col = trace( scene,x,y );

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}

