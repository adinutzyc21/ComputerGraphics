#include "MyShape.h"
using namespace std; //cout instead of std::cout

Triangle::Triangle(Point3 p1_, Point3 p2_, Point3 p3_, Vector3 n1_, Vector3 n2_, Vector3 n3_){//with vertex normal
	p1=p1_; p2=p2_; p3=p3_;
	n1=n1_; n2=n2_; n3=n3_;//initialize
	//make vectors unit
	n1.normalize();
	n2.normalize();
	n3.normalize();
}

void Triangle::drawTriangle(){
	glNormal3f(n1[0], n1[1], n1[2]);
	glVertex3f(p1[0], p1[1], p1[2]);
	glNormal3f(n2[0], n2[1], n2[2]);
	glVertex3f(p2[0], p2[1], p2[2]);
	glNormal3f(n3[0], n3[1], n3[2]);
	glVertex3f(p3[0], p3[1], p3[2]);
}

MyShape::MyShape(){
	displayList = 0;//initialize
}

MyShape::MyShape(int t1_){
	t1=t1_; 
	displayList = 0;//keep a list of everything I've drawn
}

MyShape::MyShape(int t1_, int t2_){
	t1=t1_;t2=t2_; //initialize
	displayList = 0;//keep a list of everything I've drawn
}

MyShape::~MyShape(){
	//delete the variables here
	if(displayList!=0)
		glDeleteLists(displayList, 1);
	displayList = 0;
}

void MyShape::drawShape(){
	if(displayList==0){//if it doesn't exist or we have the wrong normals
		makeList();//make it
	}
	glCallList(displayList);//otherwise just display it
}

void MyShape::makeList(){
	displayList=glGenLists(GLsizei(1));//make one list
	glNewList(displayList, GL_COMPILE);
		glBegin(GL_TRIANGLES);
			for(std::vector<Triangle>::iterator triangle = triangles.begin(); triangle != triangles.end(); triangle++){
				triangle->drawTriangle();
			}
		glEnd();
	glEndList();
}

Sphere::Sphere(int t1_){
	t1=t1_; 
	if(t1 >= 5)
		t1 = 5;
		
	makeIcosahedron();
	
	//split triangles
	if(t1 > 1){
	for(int i=1;i<t1;i++)
		splitTriangles();
	}
	normalizeVertices();//and add normal if faceNormal is true
}

void Sphere::splitTriangles(){//how many times do we split
	Point3 m12,m23,m31;//midpoints
	std::vector<Triangle> triangles_copy;
	Vector3 n=Vector3(0,1,0);
	for(int j=0; j<int(triangles.size()); j++){
		Triangle t = triangles[j];
		m12 = Point3((t.p1[0]+t.p2[0])/2, (t.p1[1]+t.p2[1])/2, (t.p1[2]+t.p2[2])/2);
		m23 = Point3((t.p2[0]+t.p3[0])/2, (t.p2[1]+t.p3[1])/2, (t.p2[2]+t.p3[2])/2);
		m31 = Point3((t.p3[0]+t.p1[0])/2, (t.p3[1]+t.p1[1])/2, (t.p3[2]+t.p1[2])/2);
		triangles_copy.push_back(Triangle(t.p1,m12,m31,n,n,n));
		triangles_copy.push_back(Triangle(m12,t.p2,m23,n,n,n));
		triangles_copy.push_back(Triangle(m12,m23,m31,n,n,n));
		triangles_copy.push_back(Triangle(m31,m23,t.p3,n,n,n));
	}
	triangles.swap(triangles_copy);
}

void Sphere::normalizeVertices(){//normalize each point to be at a distance of 0.5 from the p
	std::vector<Triangle> triangles_copy;
	triangles_copy.empty();
	for(int i=0; i<int(triangles.size()); i++){
		Triangle t = triangles[i];
		Point3 p[3] = {t.p1, t.p2, t.p3};
		Vector3 n[3];//the normals
		for(int j=0; j<3; j++){
			double mag = sqrt(p[j][0] * p[j][0] + p[j][1] * p[j][1] + p[j][2] * p[j][2]);
			p[j][0]*=0.5/mag;p[j][1]*=0.5/mag;p[j][2]*=0.5/mag;
			
			n[j]=Vector3(p[j][0],p[j][1],p[j][2]);n[j].normalize();
		}
		triangles_copy.push_back(Triangle(p[0],p[1],p[2],n[0],n[1],n[2]));
	}
	triangles.empty();
	triangles.swap(triangles_copy);
}

void Sphere::makeIcosahedron(){
	//let's find the vertices of this
	double a = sqrt(2.0/(5-sqrt(5.0)));
	double b = 1/5.0 * sqrt((50-10*sqrt(5.0))/(10-2*sqrt(5.0)));
	double c = 0.5;
	double d = (1+sqrt(5.0))/(2*sqrt(10-2*sqrt(5)));
	double e = (-1+sqrt(5.0))/(2*sqrt(10-2*sqrt(5)));
	double f = 1/2.0*sqrt((5+sqrt(5))/(5-sqrt(5)));
	
	Point3 v1 = Point3(0, 0, -1); 	Point3 v2 = Point3(0, 0, 1);
	Point3 v3 = Point3(-a, 0, -b);	Point3 v4 = Point3(a, 0, b);
	Point3 v5 = Point3(d, -c, -b);	Point3 v6 = Point3(d, c, -b);
	Point3 v7 = Point3(-d, -c, b);	Point3 v8 = Point3(-d, c, b);
	Point3 v9 = Point3(-e, -f, -b);	Point3 v10 = Point3(-e, f, -b);
	Point3 v11 = Point3(e, -f, b);	Point3 v12 = Point3(e, f, b);
	
	Vector3 n = Vector3(0,0,0);
	
	//scale
	v1.operator*=(0.5);v2.operator*=(0.5);v3.operator*=(0.5);v4.operator*=(0.5);
	v5.operator*=(0.5);v6.operator*=(0.5);v7.operator*=(0.5);v8.operator*=(0.5);
	v9.operator*=(0.5);v10.operator*=(0.5);v11.operator*=(0.5);v12.operator*=(0.5);
	
	triangles.push_back(Triangle(v2,v12,v8,n,n,n));	triangles.push_back(Triangle(v2,v8,v7,n,n,n));
	triangles.push_back(Triangle(v2,v7,v11,n,n,n));	triangles.push_back(Triangle(v2,v11,v4,n,n,n));
	triangles.push_back(Triangle(v2,v4,v12,n,n,n));
	
	triangles.push_back(Triangle(v5,v9,v1,n,n,n));	triangles.push_back(Triangle(v5,v1,v6,n,n,n));
	triangles.push_back(Triangle(v5,v6,v4,n,n,n));	triangles.push_back(Triangle(v5,v4,v11,n,n,n));
	triangles.push_back(Triangle(v5,v11,v9,n,n,n));
	
	triangles.push_back(Triangle(v3,v10,v1,n,n,n));	triangles.push_back(Triangle(v3,v1,v9,n,n,n));
	triangles.push_back(Triangle(v3,v9,v7,n,n,n));	triangles.push_back(Triangle(v3,v7,v8,n,n,n));
	triangles.push_back(Triangle(v3,v8,v10,n,n,n));
	
	triangles.push_back(Triangle(v6,v1,v10,n,n,n));	triangles.push_back(Triangle(v6,v10,v12,n,n,n));
	triangles.push_back(Triangle(v6,v12,v4,n,n,n));	triangles.push_back(Triangle(v7,v9,v11,n,n,n));
	triangles.push_back(Triangle(v12,v10,v8,n,n,n));
}

Cylinder::Cylinder(int t1_, int t2_){
	t1=t1_; t2=t2_; 
	if(t1<3)
		t1=3;
	if(t2<1)
		t2=1;
	double ta = double(t1), tb = double(t2);
	double x1,x2,z1,z2;
	double yT = 0.5, yB = -0.5 , f;
	int i,j;//iterators
	
	Point3 centerTop = Point3(0., 0.5, 0.), centerBot = Point3(0., -0.5, 0.);
	
	for(i=0; i<t1; i++){	
		x1 = 0.5*cos(2*(i%t1)*M_PI/ta); z1 = 0.5*sin(2*(i%t1)*M_PI/ta);
		x2 = 0.5*cos(2*((i+1)%t1)*M_PI/ta);  z2 = 0.5*sin(2*((i+1)%t1)*M_PI/ta);
		
		//one normal per vertex
		//normals
		Vector3 nT= Vector3(0,1,0), nB= Vector3(0,-1,0); nT.normalize(); nB.normalize();
		Vector3 n2= Vector3(x2,0,z2), n1= Vector3(x1,0,z1); n1.normalize(); n2.normalize();
		
		//tessellate the top and bottom caps
		//top cap
		triangles.push_back(
		  Triangle(Point3(x2,0.5,z2), Point3(x1,0.5,z1),centerTop,nT,nT,nT));
		//bottom cap
		triangles.push_back(
			Triangle(Point3(x1,-0.5,z1), Point3(x2,-0.5,z2),centerBot,nB,nB,nB));
		  
		//tessellate the barrel
		for(j=0; j<t2; j++){//tessellate the barrel
			f=1/tb;
			yT = 0.5 - j*f;
			yB = yT - f;
			triangles.push_back(
				Triangle(Point3(x2,yT,z2), Point3(x2,yB,z2), Point3(x1,yB,z1), n2, n2,n1));
			triangles.push_back(
				Triangle(Point3(x2,yT,z2), Point3(x1,yB,z1), Point3(x1,yT,z1),n2,n1,n1));
		}
	}
}

Cone::Cone(int t1_, int t2_){
	t1=t1_; t2=t2_; 
	if(t1<3)
		t1=3;
	if(t2<1)
		t2=1;
	double ta = double(t1), tb = double(t2);
	double x1,x2,z1,z2;
	double yT = 0.5, yB = -0.5 , f;
	int i,j;//iterators
	float r=0.5, r2, r3;//the radii
	
	Point3 centerTop = Point3(0., 0.5, 0.), centerBot = Point3(0., -0.5, 0.);
	
	for(i=0; i<t1; i++){//tessellate the top and bottom caps
	
		x1 = cos(2*(i%t1)*M_PI/ta); z1 = sin(2*(i%t1)*M_PI/ta);
		x2 = cos(2*((i+1)%t1)*M_PI/ta);  z2 = sin(2*((i+1)%t1)*M_PI/ta);
		
		//one normal per vertex
		//normals
		Vector3 nT=Vector3(0,1,0), nB= Vector3(0,-1,0); nB.normalize(); nT.normalize();
		Vector3 n2= Vector3(x2,0.5*sqrt(x2*x2+z2*z2),z2), n1= Vector3(x1,0.5*sqrt(x1*x1+z1*z1),z1); n1.normalize(); n2.normalize();
	
		//tesselate the bottom cap 
		triangles.push_back(
			Triangle(Point3(r*x1,-0.5,r*z1), Point3(r*x2,-0.5,r*z2),centerBot,nB,nB,nB));
		
		//tessellate the side
		for(j=0; j<t2; j++){
			f=1/tb;
			yT = 0.5 - j*f;
			yB = yT - f;
			r3 = r*(0.5-yT);
			r2 = r*(0.5-yB);
			if(j==0)
				triangles.push_back(
					Triangle(centerTop, Point3(r2*x2,yB,r2*z2), Point3(r2*x1,yB,r2*z1), nT, n2,n1));
			else{
				triangles.push_back(
					Triangle(Point3(r3*x2,yT,r3*z2),Point3(r2*x2,yB,r2*z2), Point3(r2*x1,yB,r2*z1), n2,n2,n1));
				triangles.push_back(
					Triangle(Point3(r3*x2,yT,r3*z2), Point3(r2*x1,yB,r2*z1), Point3(r3*x1,yT,r3*z1),n2,n1,n1));
			}
		}
		
	}
}

Cube::Cube(int t1_){//there is no difference in shading here so it doesn't matter
	t1 = t1_;	
	if(t1<1)
		t1=1;
	
	Vector3 nx1 = Vector3(1,0,0), nx2 = Vector3(-1,0,0);
	Vector3 ny1 = Vector3(0,1,0), ny2 = Vector3(0,-1,0);
	Vector3 nz1 = Vector3(0,0,1), nz2 = Vector3(0,0,-1);
		
	double t=double(t1);//ensure everything's a double by dividing by a double
	for(int i=0;i<t1;i++){
		for(int j=0;j<t1;j++){
			//positive z square
			triangles.push_back(
				Triangle(Point3(-0.5+(i+1)/t, -0.5+(j+1)/t, 0.5), 
						 Point3(-0.5+i/t, -0.5+j/t, 0.5), 
						 Point3(-0.5+(i+1)/t, -0.5+j/t, 0.5),nz1,nz1,nz1));
			triangles.push_back(
				Triangle(Point3(-0.5+(i+1)/t, -0.5+(j+1)/t, 0.5), 
						 Point3(-0.5+i/t, -0.5+(j+1)/t, 0.5), 
						 Point3(-0.5+i/t, -0.5+j/t, 0.5),nz1,nz1,nz1));
			
			//negative z (put points backward)
			triangles.push_back(
				Triangle(Point3(-0.5+(i+1)/t, -0.5+j/t, -0.5), 
						 Point3(-0.5+i/t, -0.5+j/t, -0.5), 
						 Point3(-0.5+(i+1)/t, -0.5+(j+1)/t, -0.5),nz2,nz2,nz2));
			triangles.push_back(
				Triangle(Point3(-0.5+i/t, -0.5+j/t, -0.5), 
						 Point3(-0.5+i/t, -0.5+(j+1)/t, -0.5), 
						 Point3(-0.5+(i+1)/t, -0.5+(j+1)/t, -0.5),nz2,nz2,nz2));
			
			//positive y square
			triangles.push_back(
				Triangle(Point3(-0.5+(i+1)/t, 0.5, -0.5+j/t), 
						 Point3(-0.5+i/t, 0.5, -0.5+j/t), 
						 Point3(-0.5+(i+1)/t, 0.5, -0.5+(j+1)/t),ny1,ny1,ny1));
			triangles.push_back(
				Triangle(Point3(-0.5+i/t, 0.5, -0.5+j/t), 
						 Point3(-0.5+i/t, 0.5, -0.5+(j+1)/t), 
						 Point3(-0.5+(i+1)/t, 0.5, -0.5+(j+1)/t),ny1,ny1,ny1));
			
			//negative y square
			triangles.push_back(
				Triangle(Point3(-0.5+(i+1)/t, -0.5, -0.5+(j+1)/t), 
						 Point3(-0.5+i/t, -0.5, -0.5+j/t), 
						 Point3(-0.5+(i+1)/t, -0.5, -0.5+j/t),ny2,ny2,ny2));
			triangles.push_back(
				Triangle(Point3(-0.5+(i+1)/t, -0.5, -0.5+(j+1)/t), 
						 Point3(-0.5+i/t, -0.5, -0.5+(j+1)/t), 
						 Point3(-0.5+i/t, -0.5, -0.5+j/t),ny2,ny2,ny2));
			
			//positive x square
			triangles.push_back(
				Triangle(Point3(0.5, -0.5+(i+1)/t, -0.5+(j+1)/t), 
						 Point3(0.5, -0.5+i/t, -0.5+j/t), 
						 Point3(0.5, -0.5+(i+1)/t, -0.5+j/t),nx1,nx1,nx1));
			triangles.push_back(
				Triangle(Point3(0.5, -0.5+(i+1)/t, -0.5+(j+1)/t), 
						 Point3(0.5, -0.5+i/t, -0.5+(j+1)/t), 
						 Point3(0.5, -0.5+i/t, -0.5+j/t),nx1,nx1,nx1));
			
			//negative x square
			triangles.push_back(
				Triangle(Point3(-0.5, -0.5+(i+1)/t, -0.5+j/t), 
						 Point3(-0.5, -0.5+i/t, -0.5+j/t), 
						 Point3(-0.5, -0.5+(i+1)/t, -0.5+(j+1)/t),nx2,nx2,nx2));
			triangles.push_back(
				Triangle(Point3(-0.5, -0.5+i/t, -0.5+j/t), 
						 Point3(-0.5, -0.5+i/t, -0.5+(j+1)/t), 
						 Point3(-0.5, -0.5+(i+1)/t, -0.5+(j+1)/t),nx2,nx2,nx2));
		}
	}
}

Torus::Torus(int t1_, int t2_){
	t1 = t1_;
	t2 = t2_;
	if(t1<3)
		t1=3;
	if(t2<2)
		t2=2;
	
	double ta = double(t1), tb = double(t2);
	double R = 0.5, r = R/4;
	Point3 p1,p2,p3,p4;//the points
	Vector3 n1,n2,n3,n4;//the normals
	
	double c1, s1, c2, s2, c11, s11, c22, s22;
	
	for(int i = 0; i < t1; i++){
		for(double j = 0; j < t2; j++){
			c1 = cos(i/ta*2*M_PI); s1 = sin(i/ta*2*M_PI);
			c2 = cos(j/tb*2*M_PI); s2 = sin(j/tb*2*M_PI);
			c11 = cos((i+1)/ta*2*M_PI); s11 = sin((i+1)/ta*2*M_PI);
			c22 = cos((j+1)/tb*2*M_PI); s22 = sin((j+1)/tb*2*M_PI);
			
			p1 = Point3((R+r*c2)*c1, (R+r*c2)*s1, r*s2);
			p2 = Point3((R+r*c22)*c1, (R+r*c22)*s1, r*s22);
			p3 = Point3((R+r*c2)*c11, (R+r*c2)*s11, r*s2);
			p4 = Point3((R+r*c22)*c11, (R+r*c22)*s11, r*s22);
			
			n1 = Vector3(c2*c1, c2*s1, s2);n1.normalize();
			n2 = Vector3(c22*c1, c22*s1, s22);n2.normalize();
			n3 = Vector3(c2*c11, c2*s11, s2);n3.normalize();
			n4 = Vector3(c22*c11, c22*s11, s22);n4.normalize();
			triangles.push_back(Triangle(p2,p1,p3,n2,n1,n3));
			triangles.push_back(Triangle(p4,p2,p3,n4,n2,n3));
		}
	}
}

//################# For the INTERSECTION LAB #####################
//################################################################

HitRecord MyShape::intersect(const Point3& p, Vector3& d){
	//can't figure how to make this method purely virtual, so this'll have to do
	HitRecord hitRec = HitRecord();
	return hitRec;
}

HitRecord Cube::intersect(const Point3& p, Vector3& d){
	HitRecord hitRec = HitRecord();
	double t;
	Point3 q;
	
	//normals n(nx,ny,nz): upY=(0,1,0),    downY=(0,-1,0),    rightX=(1,0,0),    leftX=(-1,0,0),   backZ=(0,0,1),    frontZ=(0,0,-1)
	//points g(x0,y0,z0):  upY=(0,0.5,0),  downY=(0,-0.5,0),  rightX=(0.5,0,0),  leftX=(-0.5,0,0), backZ=(0,0,0.5),  frontZ=(0,0,-0.5)
	
	//upY
	t=(0.5-p[1])/d[1];
	q = p + t*d;
	if(q[0] <= 0.5 && q[0] >= -0.5 && q[2] <= 0.5 && q[2] >= -0.5 && t >= 0)
		hitRec.addHit(t, 0., 0., q, Vector3(0., 1., 0.));

	//downY
	t = (0.5 + p[1])/-d[1];
	q = p + t*d;
	if(q[0] <= 0.5 && q[0] >= -0.5 && q[2] <= 0.5 && q[2] >= -0.5 && t >= 0)
		hitRec.addHit(t, 0., 0., q, Vector3(0., -1., 0.));

	//rightX
	t = (0.5 - p[0])/d[0];
	q = p + t*d;
	if(q[1] <= 0.5 && q[1] >= -0.5 && q[2] <= 0.5 && q[2] >= -0.5 && t >= 0)
		hitRec.addHit(t, 0., 0., q, Vector3(1., 0., 0.));

	//leftX
	t = (0.5 + p[0])/-d[0];
	q = p + t*d;
	if(q[1] <= 0.5 && q[1] >= -0.5 && q[2] <= 0.5 && q[2] >= -0.5 && t >= 0)
		hitRec.addHit(t, 0., 0., q, Vector3(-1., 0., 0.));

	//backZ
	t = (0.5 - p[2])/d[2];
	q = p + t*d;
	if(q[0] <= 0.5 && q[0] >= -0.5 && q[1] <= 0.5 && q[1] >= -0.5 && t >= 0)
		hitRec.addHit(t, 0., 0., q, Vector3(0., 0., 1.));

	//frontZ
	t = (0.5 + p[2])/-d[2];
	q = p + t*d;
	if(q[0] <= 0.5 && q[0] >= -0.5 && q[1] <= 0.5 && q[1] >= -0.5	&& t >= 0)
		hitRec.addHit(t, 0., 0., q, Vector3(0., 0., -1.));


	hitRec.sortHits();
	return hitRec;
}

HitRecord Sphere::intersect(const Point3& p, Vector3& d){
	HitRecord hitRec = HitRecord();

	double A,B,C, t1,t2, delta;
	Point3 q;
	Vector3 qN;
	
	A = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
	B = 2*(p[0]*d[0] + p[1]*d[1] + p[2]*d[2]);
	C = p[0]*p[0] + p[1]*p[1] + p[2]*p[2] - 0.25;

	delta = B*B - 4*A*C;
	
	if(delta < 0)
		return hitRec;
	
	if(delta==0){//t1=t2
		t1 = -B/(2*A);
		if(t1 < 0)//there is nothing to return
			return hitRec;
		q = p + t1*d;
		qN = Vector3(q[0], q[1], q[2]); qN.normalize();
		hitRec.addHit(t1, 0., 0., q, qN);
		return hitRec;
	}
	
	//if(delta>0)
	t1 = (-B + sqrt(delta))/(2*A);// t1 >= t2 always
	if(t1 < 0)//there is nothing to return
		return hitRec;
	//otherwise t1>0
	t2 = (-B - sqrt(delta))/(2*A);
	
	if(t2 < 0){//then only return t1
		q = p + t1*d;
		qN = Vector3(q[0], q[1], q[2]); qN.normalize();
		hitRec.addHit(t1, 0., 0., q, qN);
		return hitRec;
	}
	
	//otherwise t2 is also > 0
	q = p + t1*d;
	qN = Vector3(q[0], q[1], q[2]); qN.normalize();
	hitRec.addHit(t1, 0., 0., q, qN);
	
	q = p + t2*d;
	qN = Vector3(q[0], q[1], q[2]); qN.normalize();
	hitRec.addHit(t2, 0., 0., q, qN);
	
	hitRec.sortHits();
	
	return hitRec;
}

HitRecord Cylinder::intersect(const Point3& p, Vector3& d){
	HitRecord hitRec = HitRecord();
	
	double A,B,C, t1,t2, delta;
	Point3 q;
	Vector3 qN;
	
	//first, deal with the caps (otherwise I'd need to rewrite this method)
	t1 = (0.5 - p[1])/d[1];//top cap
	q = p + t1*d;
	if(q[0]*q[0] + q[2]*q[2] <= 0.25 && t1>=0)
		hitRec.addHit(t1, 0., 0., q, Vector3(0., 1., 0.));
		
	t1 = (0.5 + p[1])/-d[1];//bottom cap
	q = p + t1*d;
	if(q[0]*q[0] + q[2]*q[2] <= 0.25 && t1>=0)
		hitRec.addHit(t1, 0., 0., q, Vector3(0., -1., 0.));
	
	//the sides
	A = d[0]*d[0] + d[2]*d[2];
	B = 2*(p[0]*d[0] + p[2]*d[2]);
	C = p[0]*p[0] + p[2]*p[2] - 0.25;

	delta = B*B - 4*A*C;
	
	if(delta < 0)
		return hitRec;
	
	if(delta==0){//t1=t2
		t1 = -B/(2*A);
		if(t1 < 0)//there is nothing to return
			return hitRec;
		q = p + t1*d;
		if(q[1] <= 0.5 && q[1] >= -0.5){
			qN = Vector3(q[0], 0, q[2]); qN.normalize();
			hitRec.addHit(t1, 0., 0., q, qN);
		}
		return hitRec;
	}
	
	//if(delta>0)
	t1 = (-B + sqrt(delta))/(2*A);// t1 >= t2 always
	if(t1 < 0)//there is nothing to return
		return hitRec;
	//otherwise t1>0
	t2 = (-B - sqrt(delta))/(2*A);
	
	if(t2 < 0){//then only return t1
		q = p + t1*d;
		if(q[1] <= 0.5 && q[1] >= -0.5){
			qN = Vector3(q[0], 0, q[2]); qN.normalize();
			hitRec.addHit(t1, 0., 0., q, qN);
		}
		return hitRec;
	}
	
	//otherwise t2 is also > 0
	q = p + t1*d;
	if(q[1] <= 0.5 && q[1] >= -0.5){
		qN = Vector3(q[0], 0, q[2]); qN.normalize();
		hitRec.addHit(t1, 0., 0., q, qN);
	}
	
	q = p + t2*d;
	if(q[1] <= 0.5 && q[1] >= -0.5){
		qN = Vector3(q[0], 0, q[2]); qN.normalize();
		hitRec.addHit(t2, 0., 0., q, qN);
	}
	
	hitRec.sortHits();
	
	return hitRec;
}

HitRecord Cone::intersect(const Point3& p, Vector3& d){
	HitRecord hitRec = HitRecord();
	
	double A,B,C, t1,t2, delta;
	Point3 q;
	Vector3 qN;
	
	//first, deal with the base (otherwise I'd need to rewrite this method)
	t1 = (0.5 + p[1])/-d[1];//bottom cap
	q = p + t1*d;
	if(q[0]*q[0] + q[2]*q[2] <= 0.25 && t1>=0)
		hitRec.addHit(t1, 0., 0., q, Vector3(0., -1., 0.));
	
	//the sides
	A = d[0]*d[0] + d[2]*d[2] - 0.25*d[1]*d[1];
	B = 2*(p[0]*d[0] + p[2]*d[2]) + 0.5*d[1]*(0.5 - p[1]);;
	C = p[0]*p[0] + p[2]*p[2] - 0.25*(0.5 - p[1])*(0.5 - p[1]);

	delta = B*B - 4*A*C;
	
	if(delta < 0)
		return hitRec;
	
	if(delta==0){//t1=t2
		t1 = -B/(2*A);
		if(t1 < 0)//there is nothing to return
			return hitRec;
		q = p + t1*d;
		if(q[1] <= 0.5 && q[1] >= -0.5){
			qN = Vector3(q[0], 0.5*sqrt(q[0]*q[0]+q[2]*q[2]), q[2]); qN.normalize();
			hitRec.addHit(t1, 0., 0., q, qN);
		}
		return hitRec;
	}
	
	//if(delta>0)
	t1 = (-B + sqrt(delta))/(2*A);// t1 >= t2 always
	if(t1 < 0)//there is nothing to return
		return hitRec;
	//otherwise t1>0
	t2 = (-B - sqrt(delta))/(2*A);
	
	if(t2 < 0){//then only return t1
		q = p + t1*d;
		if(q[1] <= 0.5 && q[1] >= -0.5){
			qN = Vector3(q[0], 0.5*sqrt(q[0]*q[0]+q[2]*q[2]), q[2]); qN.normalize();
			hitRec.addHit(t1, 0., 0., q, qN);
		}
		return hitRec;
	}
	
	//otherwise t2 is also > 0
	q = p + t1*d;
	if(q[1] <= 0.5 && q[1] >= -0.5){
		qN = Vector3(q[0], 0.5*sqrt(q[0]*q[0]+q[2]*q[2]), q[2]); qN.normalize();
		hitRec.addHit(t1, 0., 0., q, qN);
	}
	
	q = p + t2*d;
	if(q[1] <= 0.5 && q[1] >= -0.5){
		qN = Vector3(q[0], 0.5*sqrt(q[0]*q[0]+q[2]*q[2]), q[2]); qN.normalize();
		hitRec.addHit(t2, 0., 0., q, qN);
	}
	
	hitRec.sortHits();
	
	return hitRec;
}

HitRecord Torus::intersect(const Point3& p, Vector3& d){
	HitRecord hitRec = HitRecord(); 
	
	double A,B,C,D,E;
	//double t1,t2, delta;
	//Point3 q;
	//Vector3 qN;
	
	double R = 0.5, r = R/4;//radii
	
	Vector3 pv = Vector3(p[0],p[1],p[2]);
	A = d*d*d*d;//always 1?
	B = 4*d*d*(pv*d);
	C = 4*(pv*d)*(pv*d)+2*(d*d)*((pv*pv)-R*R-r*r)+4*R*R*d[2]*d[2];
	D = 8*R*R*pv[2]*d[2] + 4*(pv*d)*(pv*pv-R*R-r*r);
	E = ((pv*pv)-r*r-R*R)*((pv*pv)-r*r-R*R)+4*R*R*p[2]*p[2]-4*R*R*r*r;
	
	A=A/A; B=B/A; C=C/A; D=D/A; E=E/A;
	
	
	
	hitRec.sortHits();
	
	return hitRec;
}