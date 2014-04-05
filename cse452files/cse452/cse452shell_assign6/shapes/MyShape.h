#ifndef _MY_SHAPE_H_
#define _MY_SHAPE_H_

#include "../cse452.h"
#include "vecmath/Vector3.h"
#include "vecmath/Point3.h"
#include <vector>

//### for the Intersection Lab
#include "../intersection/HitRecord.h"
//###

class Triangle{//define the triangle
	public:
		Triangle(Point3 p1_, Point3 p2_, Point3 p3_, Vector3 n1_, Vector3 n2_, Vector3 n3_);//constructor
		void drawTriangle();//draw the current triangle
	//private:
		Point3 p1, p2, p3; //the tree points that define the triangle, in counterclockwise order
		Vector3 n1, n2, n3; //the normals for each vertex; the normals are the same if we only have one normal per triangle
		
		//### for the Intersection Lab
		//HitRecord intersect(const Point3& p, Vector3& d);
		//###
};

class MyShape{
 public:
	 MyShape();
	 MyShape(int t1_, int t2_);
	 MyShape(int t1_);
	 ~MyShape();
	 
	void drawShape();//draw the current shape
	
	//### for the Intersection Lab
	virtual HitRecord intersect(const Point3& p, Vector3& d) ;
	//###

protected: //protected variables are seen in the children of the current class, while private are not
	int t1, t2;//tessellation parameters
	std::vector<Triangle> triangles;//a vector of triangles for a shape
	
	//for the lists
	GLuint displayList;
	void makeList();
};
 
class Cube : public MyShape {
public:
	Cube(int t1_);
	
	//### for the Intersection Lab
	HitRecord intersect(const Point3& p, Vector3& d);
	//###
};

class Sphere : public MyShape {
public:
	Sphere(int t1_);
	
	//### for the Intersection Lab
	HitRecord intersect(const Point3& p, Vector3& d);
	//###
private:
	void makeIcosahedron();
	void splitTriangles();//in 2
	void normalizeVertices();
};

class Cylinder : public MyShape {
public:
	Cylinder(int t1_, int t2_);
	
	//### for the Intersection Lab
	HitRecord intersect(const Point3& p, Vector3& d);
	//###
};

class Cone : public MyShape {
public:
	Cone(int t1_, int t2_);
	
	//### for the Intersection Lab
	HitRecord intersect(const Point3& p, Vector3& d);
	//###
};

class Torus: public MyShape {
public:
	Torus(int t1_, int t2_);
	
	//### for the Intersection Lab
	HitRecord intersect(const Point3& p, Vector3& d);
	//###
};

#endif /* _MY_SHAPE_H_ */
