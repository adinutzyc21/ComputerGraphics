#include "../cse452.h"
#include "Camera.h"
#include <cmath>
#include <FL/Fl.H>
using namespace std;

Camera::Camera() 
{
    // initialize your data here
    skew = 0.0;
    aspScale = 1.0;
    projCtr = Point3(0.,0.,0.);
    
    R = Matrix4::identity();
    R_inv = Matrix4::identity();
    T = Matrix4::identity();
    T_inv = Matrix4::identity();
    Sxy = Matrix4::identity();
    Sxy_inv = Matrix4::identity();
    Sxyz = Matrix4::identity();
    Sxyz_inv = Matrix4::identity();
    
    D = Matrix4::identity();
    Proj = Matrix4::identity();
    W2c = Matrix4::identity();
    C2w = Matrix4::identity();
    
    p=Point3(0.,0.,0.);
    up = Vector3(0.,1.,0.);
    look = Vector3(0.,0.,-1.);
    right=look^up;
    
    dn=0.;df=1.;
    
    vectors();
}

Camera::~Camera() {
    // destroy your data here
}

// set methods
void Camera::setFrom(const Point3& from) {
    // set the current viewpoint (eye point)
    p = from;
    
    translation();//T
	worldToCam();//W2c=R*T
	translation_inv();//T_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::setAt(const Point3& at) {
    // set the point the camera is looking at
    // calling this requires that the from (or eye) point already be valid
    look = at - p;
    
    vectors();//u,v,n
    rotation();//R
	worldToCam();//W2c=R*T
	rotation_inv();//R_inv	
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::setLook(const Vector3& l) {
    // set the direction the camera is looking at
    look = l;
    right = look^up;
    
    vectors();//u,v,n
	rotation();//R
	worldToCam();//W2c=R*T
	rotation_inv();//R_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::setUp(const Vector3& up_) {
    // set the upwards direction of the camera
    up=up_;
	right = look^up;
	
    vectors();//u,v,n
	rotation();//R
	worldToCam();//W2c=R*T
	rotation_inv();//R_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::setWidthHeight(int w, int h) {
    // set the current width and height of the film plane
    width = w; height=h;
    
    scaleXY();
    projection();//Proj = D * Sxyz * Sxy
	scaleXY_inv();//Sxy_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::setZoom(double z) {//z=thetaH
    // set field of view (specified in degrees)
    zoom = z;
    
    scaleXY();
    projection();//Proj = D * Sxyz * Sxy
	scaleXY_inv();//Sxy_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::setNearFar(double n, double f) {
    // set the near and far clipping planes
    df=f; dn=n;
    
    scaleXYZ();//Sxyz
	perspective();//D
	projection();//Proj = D * Sxyz * Sxy
	scaleXYZ_inv();//Sxyz_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::moveForward(double dist) {
    // move the camera forward (in the viewing direction)
    // by the amount dist
    p -= dist*n;
    
    translation();//T
	worldToCam();//W2c=R*T
	translation_inv();//T_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::moveSideways(double dist) {
    // move the camera sideways (orthogonal to the viewing direction)
    // by the amount dist
    p += dist*u;
    
    translation();//T
	worldToCam();//W2c=R*T
	translation_inv();//T_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::moveVertical(double dist) {
    // move the camera vertically (along the up vector)
    // by the amount dist
    p += dist*v;
    
    translation();//T
	worldToCam();//W2c=R*T
	translation_inv();//T_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::rotateYaw(double angle) {
    // rotate the camera left/right (around the up vector)
    Matrix4 rot = Matrix4::rotation(v, angle);
	look = rot*look;
	right = look^up;
	
    vectors();//u,v,n
	rotation();//R
	worldToCam();//W2c=R*T
	rotation_inv();//R_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

void Camera::rotatePitch(double angle) {
    // rotate the camera up/down (pitch angle)
	Matrix4 rot = Matrix4::rotation(u, angle);
	look = rot*look;
	up = rot*up;
	
    vectors();//u,v,n
	rotation();//R
	worldToCam();//W2c=R*T
	rotation_inv();//R_inv
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

// GET METHODS
// The following three should be unit length and orthogonal to each other
// u vector
Vector3 Camera::getRight() const
{
    // Change this
    return u;
}

// v vector
Vector3 Camera::getUp() const
{
    // Change this
    return v;
}

// - n vector
Vector3 Camera::getLook() const
{
    // Change this
    return -n;
}

Matrix4 Camera::getProjection() const {
    // return the current projection and scale matrix

    // Change this
    return Proj;
}

Matrix4 Camera::getWorldToCamera() const {
    // return the current world to camera matrix
    // Rotation and translation

    // Change this
    return W2c;
}

Matrix4 Camera::getRotationFromXYZ() const
{
    // return just the rotation matrix

    // Change this
    return R;
}

Matrix4 Camera::getRotationToXYZ() const
{
    // return just the rotation matrix

    // Change this
    return R_inv;
}

Matrix4 Camera::getCameraToWorld() const {
    // return the current camera to world matrix
    // This is the inverse of the rotation, translation, and scale

    // Change this
    return C2w;
}

int Camera::getWidth()  const{
    // return the current image width

    // Change this
    return width;
}

int Camera::getHeight()  const{
    // return the current image height

    // Change this
    return height;
}

Point3 Camera::getEye()  const{
    // return the current eye location

    // Change this
    return p;
}

double Camera::getZoom() const
{
    // Change this
    return zoom;
}

//keyboard
void Camera::moveKeyboard( )
{
    // you may change key controls for the interactive
    // camera controls here, make sure you document your changes
    // in your README file

	if (Fl::event_key('W') || Fl::event_key('w'))
		moveForward(+0.05);
	if (Fl::event_key('S') || Fl::event_key('s'))
		moveForward(-0.05);
	if (Fl::event_key('A') || Fl::event_key('a'))
		moveSideways(-0.05);
	if (Fl::event_key('D') || Fl::event_key('d'))
		moveSideways(+0.05);
    if (Fl::event_key('I') || Fl::event_key('I'))
        moveVertical(+0.05);
    if (Fl::event_key('K') || Fl::event_key('k'))
        moveVertical(-0.05);
    if (Fl::event_key('J') || Fl::event_key('j'))
        rotateYaw(+0.05);
    if (Fl::event_key('L') || Fl::event_key('l'))
        rotateYaw(-0.05);
    if (Fl::event_key('Z') || Fl::event_key('z'))
        rotatePitch(+0.05);
    if (Fl::event_key('X') || Fl::event_key('x'))
        rotatePitch(-0.05);
	//added for skew
	if (Fl::event_key('V') || Fl::event_key('v'))
        setSkew(skew+0.05);
    if (Fl::event_key('B') || Fl::event_key('b'))
        setSkew(skew-0.05);
	//added for aspectScale
	if (Fl::event_key('N') || Fl::event_key('n'))
        setAspectRatioScale(aspScale+0.05);
    if (Fl::event_key('M') || Fl::event_key('m'))
        setAspectRatioScale(aspScale-0.05);
}

//######Extra Credit######

double Camera::getSkew() const
{
    // Change this to implement the extra credit
    return skew;
}

double Camera::getAspectRatioScale() const
{
    // Change this to implement the extra credit
    return aspScale;
}

Point3 Camera::getProjectionCenter() const
{
    // Change this to implement the extra credit
    return projCtr;
}

void Camera::setSkew( double d )
{
	skew = d;
    perspective();
    projection();
}

void Camera::setAspectRatioScale( double d )
{
	aspScale = d;
    perspective();
    projection();
}

void Camera::setProjectionCenter( const Point3 &p )
{
	projCtr = p;
    perspective();
    projection();
}

void Camera::rotateAroundAtPoint(int axis, double angle, double focusDist) {
    // Rotate the camera around the right (0), up (1), or look (2) vector
    // around the point at eye + look * focusDist
	Point3 Q,Qm;
	Matrix4 t, t_inv, R_IBAR, M;
	Q.operator=(p + look*focusDist);
	Qm = Point3(-Q[0],-Q[1],-Q[2]);
	t = Matrix4::translation(Qm);//translate Q to the origin
	
	//need to rotate by R
	
	if(axis == 0)
		R_IBAR = Matrix4::rotation(right, angle);
	else if(axis == 1)
		R_IBAR = Matrix4::rotation(up, angle);
	else
		R_IBAR = Matrix4::rotation(look, angle);
	
	//need to rotate by R_inv
	
	t_inv =  Matrix4::translation(Q);//translate back
	
	M = t_inv * R_inv * R_IBAR * R * t;//the transformation matrix
	
	p = M * p;
	up = M * up;
	look = M * look;
	right = look^up;
	
	vectors();
	translation();
	rotation();
	worldToCam();//W2c=R*T
	translation_inv();
	rotation_inv();
	camToWorld();//C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv
}

//################ my methods to calculate the matrices######################
void Camera::translation(){
	Point3 pm = Point3(-p[0],-p[1],-p[2]);
	T = Matrix4::translation(pm);
}
void Camera::translation_inv(){
	T_inv = Matrix4::translation(p);
	/*
	if(!(Matrix4::identity().approxEqual(T * T_inv)))
		cout<<"Inverse translation incorrect!\n";*/
}
void Camera::rotation(){
	R = Matrix4::rotation(u,v,n);
}
void Camera::rotation_inv(){
	R_inv = R.transpose();
	/*
	if(!(Matrix4::identity().approxEqual(R * R_inv)))
		cout<<"Inverse rotation incorrect!\n";*/
}
void Camera::scaleXY(){
	double alpha = double(width)/double(height);
	double thetaH = rad(zoom);//the zoom in radians
	double thetaW = 2 * atan(alpha * tan(thetaH/2));//thetaW
	Sxy = Matrix4::scaling(cot(thetaW/2),cot(thetaH/2),1.);
}
void Camera::scaleXY_inv(){
	double alpha = double(width)/double(height);
	double thetaH = rad(zoom);//thetaH = the zoom in radians
	double thetaW = 2 * atan(alpha * tan(thetaH/2));//thetaW
	Sxy_inv = Matrix4::scaling(tan(thetaW/2),tan(thetaH/2),1.);
	/*
	if(!(Matrix4::identity().approxEqual(Sxy * Sxy_inv)))
		cout<<"Inverse Sxy incorrect!\n";*/
}
void Camera::scaleXYZ(){
	Sxyz = Matrix4::scaling(1./df);
}
void Camera::scaleXYZ_inv(){
	Sxyz_inv = Matrix4::scaling(df);
	/*
	if(!(Matrix4::identity().approxEqual(Sxyz * Sxyz_inv)))
		cout<<"Inverse Sxyz incorrect!\n";*/
}
void Camera::perspective(){
	double k = dn/df;
	D = Matrix4(Vector4(aspScale,skew,projCtr[0],0.),Vector4(0.,1.,projCtr[1],0.),Vector4(0.,0.,1./(k-1.),k/(k-1.)),Vector4(0.,0.,-1.,0.));
	//D = Matrix4(Vector4(1,0,0,0),Vector4(0,1,0,0),Vector4(0,0,1/(k-1),k/(k-1)),Vector4(0,0,-1,0));
}
void Camera::worldToCam(){
	W2c = R * T;
}
void Camera::camToWorld(){
	C2w = T_inv * R_inv * Sxy_inv * Sxyz_inv;
}
void Camera::projection(){
	Proj = D * Sxyz * Sxy;
}

void Camera::vectors(){//n(look), v(up,n), u(v,n)
	n = Vector3(-look[0],-look[1],-look[2]); n.normalize();
	v = up - n*(up*n); v.normalize();
	u = v^n; u.normalize();
}