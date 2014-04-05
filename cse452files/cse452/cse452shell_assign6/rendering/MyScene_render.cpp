#include "../cse452.h"
#include "../sceneview/MyScene.h"
#include "RenderingInterface.h"
#include <FL/gl.h>
#include <cfloat>

void MyScene::render(int type, int width, int height, unsigned char* pixels) {
    if (!isLoaded) {
        return;
    }
	isRendering=true;
	rProgress=0.0;
	rWidth=width; rHeight=height;
	
    // Add your rendering code here.
    // Keep track of your rProgress as a value between 0 and 1
    // so the progress bar can update as the rendering progresses
    switch (type) {
        case RenderingUI::RENDER_SCANLINE: 
        	renderPixels(width, height, pixels);
        	break;
        case RenderingUI::RENDER_RAY_TRACING:  
        	renderRayTracing(width, height, pixels);
        	break;
        case RenderingUI::RENDER_PATH_TRACING: 
        	renderPixels(width, height, pixels);
        	break;
        default: break;
    }
}

void MyScene::stopRender()
{
    // Because this is threaded code, this function
    // can be called in the middle of your rendering code.
    // You should then stop at the next scanline
    isRendering=false;
}

double MyScene::getRenderProgress() {
    // return the current rProgress as a value between 0 and 1
    return rProgress;
}

////////////////////////////////////////
// add extra methods here
///////////////////////////////////////

Color MyScene::illuminationModel(Vector3 ray, Point3 eye, int limit){
	Color color, colAmb, colDiff, colSpec, colRefl, colTransp;
		
	color = Color(0,0,0);
	
	colAmb = Color(0,0,0);
	colDiff = Color(0,0,0);
	colSpec = Color(0,0,0);
	colRefl = Color(0,0,0);
	colTransp = Color(0,0,0);
	
	MatObj *matObj = 0;
	Point3 pHit; Vector3 nHit;
	
	rayObjectIntersect(eye, ray, pHit, nHit, matObj);
	
	bool renderObj=true;//do I render the current pixel
	if(nHit*camera.getLook() > 0)
		renderObj=false;
	
	if(matObj != 0 && renderObj){
		//reflection
		Object *object = matObj->obj;
		Color refl = object -> getReflect();
		if(limit > 0 && refl.getMax() > 0.){
			Vector3 R=ray;//reflected ray
			Color c2 = Color(0,0,0);
			R = 2 * (R * nHit) * nHit - R;
			//R = -R;
			R.normalize();
			c2 = illuminationModel(R, pHit+R*0.000000001, limit-1);
			colRefl = c2 * refl;
		}
		//transparency
		Color transp = object -> getTransparent();
		double alpha = object -> getAlpha();
		if(limit > 0 && alpha < 1){
			Color c3 = Color(0,0,0);
			c3 = illuminationModel(ray, pHit+ray*0.000000001, limit-1);
			colTransp = c3 * transp *alpha;
		}
		//ambient lighting
		colAmb = ambientIllumination(matObj);
		//diffuse lighting
		for(std::vector<Light>::iterator light = lights.begin(); light != lights.end(); light++){
			colDiff += diffuseIllumination(matObj, pHit, nHit, &(*light));
			colSpec += specularIllumination(matObj, ray, pHit, nHit, &(*light));
		}
		//the final color
		color = colAmb + colDiff + colSpec + colRefl;
		color = MyScene::normCol(color);
		return color;
	}
	else
		return background;
}

//this method just renders a colored screen, where each pixel has a random value
// no matter what scene you're loading - default for RENDER_SCANLINE and RENDER_PATH_TRACING
void MyScene::renderPixels(int width, int height, unsigned char* pixels){
	for(int j=0; j<height; j++){		
		if(isRendering){
			for(int i=0; i<width; i++){
				GLfloat r = static_cast<GLfloat>((float) rand() / RAND_MAX);
				GLfloat g = static_cast<GLfloat>((float) rand() / RAND_MAX);
				GLfloat b = static_cast<GLfloat>((float) rand() / RAND_MAX);
				
				putPixel(i, j, Color(r,g,b), pixels);
			}
			rProgress = (double)(j+1)/(double)(height);
			Fl::check();
		}
	}
}

//putPixel method from the brush lab, a little bit tweaked
inline
void MyScene::putPixel(int x, int y, const Color & col, unsigned char* pixels) {
  const int i = ((rHeight-y-1) * rWidth + x) * 3;
  pixels[i] = (unsigned char) (col[0] * 255.0f);
  pixels[i + 1] = (unsigned char) (col[1] * 255.0f);
  pixels[i + 2] = (unsigned char) (col[2] * 255.0f);
}

//returns the (unit) direction from the eye to a pixel in 3D world coordinates
Vector3 MyScene::generateDir(int width, int height, double x, double y){
	Point3 qs;
	Vector3 d;
	double px,py;
	
	px = (x + 0.5)* 2/width - 1;
	py = (1 - (y + 0.5) * 2/height);
	qs = Point3(px, py, -1);
	
	Matrix4 CTW = camera.getCameraToWorld();
	Point3  qw = CTW * qs;
	
	Point3 eye = camera.getEye();
	
	d = qw - eye;
	
	d.normalize();
	
	return d;
	//ray parametric equation: ray = eye + t * d
}

//Find the first positive intersection with the object 
//(so the point and normal for the first hit corresponding to the smallest t greater than 0)
//hitP and hitN: the ray intersection and normal in the world space
//returns the object we're intersecting with
void MyScene::rayObjectIntersect(Point3 p, Vector3 d, Point3& hitP, Vector3& hitN, MatObj*& hitObj){
	double tMin=99999999, t, u, v;
	Point3 hP, objP; Vector3 hN, objD;
	
	Matrix4 matrix, matrix_inv, hitMat, hitMat_inv;
	Object *object; MyShape *shape;
	
	HitRecord objHits;
	
	for(list<MatObj>::iterator it = objectL.begin(); it != objectL.end(); it++){
		matrix = it->mat;
		matrix_inv = matrix.inverse();
		object = it->obj;
		objD = matrix_inv * d;
		objP = matrix_inv * p;
		shape = object->getShape();
		
		objHits = shape->intersect(objP, objD);
		objHits.sortHits();
		
		if(objHits.getFirstHit(t, u, v, hP, hN)){//if there are any hits, get the first one
			if(t>0 && t<tMin){//check for the minimum positive t
				tMin = t;
				hitP = hP;
				hitN = hN;
				hitObj = &(*it);
				hitMat = matrix; hitMat_inv = matrix_inv;
			}
		}
	}
	
	hitP = hitMat * hitP;
	hitN = ((hitMat_inv).transpose()) * hitN;
	hitN.normalize();
}


//get the ambient color
Color MyScene::ambientIllumination(MatObj *matObj){
	Object *object = matObj->obj;
	Color ka = object->getAmbient();
	return ambientLight * ka;
}

//get the diffuse color to light source light
Color MyScene::diffuseIllumination(MatObj *matObj, Point3 pHit, Vector3 nHit, Light *light){
	
	Vector3 L = light->getPos() - pHit;//the vector from the point to the light source
	double d1 = L.length(), d2=d1*d1;
	L.normalize();
	
	bool inShadow = false;
	
	//use to calculate if in shadow or not
	MatObj *matObjNew = 0;
	Point3 pHitNew; Vector3 nHitNew;
	Point3 pNew = pHit + L * 0.000000001;
	rayObjectIntersect(pNew, L, pHitNew, nHitNew, matObjNew);
	
	bool renderObj=true;//do I render the current pixel
	if(nHitNew*camera.getLook() > 0)
		renderObj=false;
	
	if(matObjNew != 0)
		inShadow = true;
	
	if(inShadow == false  && renderObj){
		Object *object = matObj->obj;
		Color lightCol = light->getColor();
			
		double fatt;//attenuation
		Point3 falloff = light->getFalloff();
		
		fatt = 1./(falloff[0]+falloff[1]*d1+falloff[2]*d2);
		
		double nDotL = nHit * L;
		Color kd = object->getDiffuse();
		if(nDotL < 0.)
			nDotL = 0.;
		return lightCol * kd * nDotL * fatt;	
	}
	return Color(0,0,0);
}

//get the specular color to light source light
Color MyScene::specularIllumination(MatObj *matObj, Vector3 ray, Point3 pHit, Vector3 nHit, Light *light){
	
	Vector3 L = light->getPos() - pHit;//the vector from the point to the light source
	double d1 = L.length(), d2=d1*d1;
	L.normalize();
	
	bool inShadow = false;
	
	//use to calculate if in shadow or not
	MatObj *matObjNew = 0;
	Point3 pHitNew; Vector3 nHitNew;
	Point3 pNew = pHit + L * 0.000001;
	rayObjectIntersect(pNew, L, pHitNew, nHitNew, matObjNew);
	
	bool renderObj=true;//do I render the current pixel
	if(nHitNew*camera.getLook() > 0)
		renderObj=false;
	
	if(matObjNew != 0)
		inShadow = true;
	
	if(inShadow == false  && renderObj){
		Object *object = matObj->obj;
		Color lightCol = light->getColor();

		Vector3 R; //the reflection vector
		R = 2 * (L * nHit) * nHit - L;
		//R = -R;
		R.normalize();
	
		double fatt;//attenuation
		Point3 falloff = light->getFalloff();
		fatt = 1./(falloff[0]+falloff[1]*d1+falloff[2]*d2);
	
		double rayDotR = ray * R;
		double shine = object->getShine();
		Color ks = object->getSpecular();
		if(rayDotR < 0.)
			rayDotR = 0.;
			
		return lightCol * fatt * ks * pow(rayDotR,shine);	
	}
	return Color(0,0,0);
}

void MyScene::renderRayTracing(int width, int height, unsigned char* pixels){
	Vector3 ray;
	Point3 eye = camera.getEye();
	Color col;
	for(int j=0; j<height; j++){
		if(isRendering){
			for(int i=0; i<width; i++){
				ray = generateDir(width, height, i, j);
				col = illuminationModel(ray, eye, RECURSIVE_LIMIT);
				putPixel(i, j, col, pixels);
			}
			rProgress = (double)(j+1)/(double)(height);
			Fl::check();
		}
	}
}