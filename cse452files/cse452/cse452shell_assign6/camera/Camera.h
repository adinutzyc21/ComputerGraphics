#ifndef _MY_CAMERA_H_
#define _MY_CAMERA_H_

#include "cse452.h"
#include "Matrix4.h"

// you must implement all of the following methods

class Camera {
public:
    Camera();
    ~Camera();
    
    // Perspective plus scale (x,y, and z)
    Matrix4 getProjection() const;

    // Rotation and translation from world to camera
    Matrix4 getWorldToCamera() const;
    // Rotation, translation and scale from camera to world
    Matrix4 getCameraToWorld() const;
    // Just rotation from x,y,z axes to u,v,n
    Matrix4 getRotationFromXYZ() const;
    // Just rotation from u,v,n to x,y,z axes (canonical)
    Matrix4 getRotationToXYZ() const;

    // screen width/height
    int getWidth() const;
    int getHeight() const;

    // Camera orientation, position _after_ normalization
    Point3  getEye() const;
    // These should be unit length and orthogonal to each other
    // u vector
    Vector3 getRight() const;
    // v vector
    Vector3 getUp() const;
    // -n vector
    Vector3 getLook() const;

    // Perspective data
    Point3 getProjectionCenter() const;//****extra
    double getZoom() const;//****extra
    double getSkew() const;//****extra
    // "Squishing" in x,y (not Width/Heigh)
    double getAspectRatioScale() const;//****extra

    // For setting camera
    void setFrom(const Point3& from);
    void setAt(const Point3& at);
    void setLook(const Vector3& l);
    void setUp(const Vector3& up);
    void setWidthHeight(int w, int h);
    void setZoom(double z);
    void setNearFar(double n, double f);
    void setProjectionCenter( const Point3 &in_pt );//****extra
    // Extra "squishing" in x,y (not Width/Height)
    void setAspectRatioScale( double );//****extra
    void setSkew( double );//****extra

    // This is what gets called when a key is pressed
    void moveKeyboard();

    // user interaction methods
    // These rotate the camera around itself
    void moveForward(double dist);
    void moveSideways(double dist);
    void moveVertical(double dist);
    void rotateYaw(double angle);
    void rotatePitch(double angle);

    // IBar: This rotates the camera around a point at a distance focusDist
    // from the eye point of the camera. If axis is 0, it rotates the camera itself
    // around the Right vector, 1 is around the up axis, and 2 is around the look vector
    void rotateAroundAtPoint(int axis, double angle, double focusDist);

private:
    // declare your variables here:
    Point3 p;//eye point
	Vector3 up, look, right, u, v, n;//up, look, right, u, v, n (in camera coordinates)
	Matrix4 T, T_inv; //translation by (-px,-py,-pz) and (px,py,pz)
	Matrix4 R, R_inv; //rotation and rotation transpose
	Matrix4 Sxy, Sxy_inv;//scale by(cot(Tw/2), cot(Th/2), 1) and (alpha*tan(Th/2), tan(Th/2), 1)
	Matrix4 Sxyz, Sxyz_inv; //uniform scale by 1/df and df 
	Matrix4 D; //perspective transform (get trapezoid into box)
	Matrix4 Proj; //projection 
	Matrix4 W2c, C2w;//world-to-camera, camera-to-world
	double dn, df, alpha, zoom;//near far distances, aspect ratio, field of view (in degrees) = theta h
	int width, height;
	
	//recalculate these matrices
	void translation();
	void translation_inv();
	void rotation();
	void rotation_inv();
	void scaleXY();
	void scaleXY_inv();
	void scaleXYZ();
	void scaleXYZ_inv();
	void perspective();
	void worldToCam();
	void camToWorld();
	void projection();
	void vectors();
	
	//helper functions
	double cot(double v){
		return 1./tan(v);
	};//cotangent
	double rad(double a){
		return a*M_PI/180.;
	};//change from degrees to radians
	
	//extra credit
	double skew, aspScale;
	Point3 projCtr;
};

#endif /* _MY_CAMERA_H_ */
