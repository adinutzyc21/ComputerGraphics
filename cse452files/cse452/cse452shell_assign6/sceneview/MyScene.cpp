#include "../cse452.h"
#include "MyScene.h"
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <cmath>

// string constants used for parsing
// feel free to add any new keywords that you think may be helpfull
const std::string TOKEN_BACKGROUND     = "background";
const std::string TOKEN_COLOR          = "color";
const std::string TOKEN_OB             = "[";
const std::string TOKEN_CB             = "]";
const std::string TOKEN_CAMERA         = "camera";
const std::string TOKEN_EYE            = "eye";
const std::string TOKEN_LOOK           = "look";
const std::string TOKEN_FOCUS          = "focus";
const std::string TOKEN_UP             = "up";
const std::string TOKEN_ANGLE          = "angle";
const std::string TOKEN_NEAR_FAR       = "near-far";
const std::string TOKEN_LIGHT          = "light";
const std::string TOKEN_LIGHTTYPE      = "type";
const std::string TOKEN_POINT          = "point";
const std::string TOKEN_DIRECTIONAL    = "directional";
const std::string TOKEN_SPOTLIGHT      = "spotlight";
const std::string TOKEN_POSITION       = "position";
const std::string TOKEN_DIRECTION      = "direction";
const std::string TOKEN_FUNCTION       = "function";
const std::string TOKEN_APERTURE       = "aperture";
const std::string TOKEN_EXPONENT       = "exponent";
const std::string TOKEN_MASTERSUBGRAPH = "mastersubgraph";
const std::string TOKEN_TRANS          = "trans";
const std::string TOKEN_ROTATE         = "rotate";
const std::string TOKEN_TRANSLATE      = "translate";
const std::string TOKEN_SCALE          = "scale";
const std::string TOKEN_MATRIXRC       = "matrixRC";
const std::string TOKEN_MATRIXCR       = "matrixCR";
const std::string TOKEN_OBJECT         = "object";
const std::string TOKEN_CUBE           = "cube";
const std::string TOKEN_CYLINDER       = "cylinder";
const std::string TOKEN_CONE           = "cone";
const std::string TOKEN_SPHERE         = "sphere";
const std::string TOKEN_COW            = "cow";
const std::string TOKEN_AMBIENT        = "ambient";
const std::string TOKEN_DIFFUSE        = "diffuse";
const std::string TOKEN_SPECULAR       = "specular";
const std::string TOKEN_REFLECT        = "reflect";
const std::string TOKEN_TRANSPARENT    = "transparent";
const std::string TOKEN_EMIT           = "emit";
const std::string TOKEN_SHINE          = "shine";
const std::string TOKEN_IOR            = "ior";
const std::string TOKEN_TEXTURE        = "texture";
const std::string TOKEN_SUBGRAPH       = "subgraph";
const std::string TOKEN_ALPHA          = "alpha";
const std::string TOKEN_TORUS           = "torus";

MyScene::MyScene() :
ambientLight(0,0,0)
{
    // initialize your variables here
	cube = new Cube(1);
	sphere = new Sphere(5);
	cone = new Cone(30,1);
	cylinder = new Cylinder(30,1);	
	torus = new Torus(30,30);	
    
    resetScene();
}

MyScene::~MyScene() {
    // destroy your variables here
    clearMasterSubgraph();
	delete cube; cube=0;
	delete sphere; sphere=0;
	delete cylinder; cylinder=0;
	delete cone; cone=0;
	delete torus; torus=0;
}

void MyScene::clearMasterSubgraph(){//deletes the Tree and clears the subgraph
    //need to delete the Tree before clearing the map
    for(map<string, Tree*>::iterator it = masterSubgraphs.begin(); it != masterSubgraphs.end(); it++){
    	delete it->second; //it->second is a Tree*
	}
    masterSubgraphs.clear();
}

void MyScene::resetScene() {
    // reset scene to defaults (empty the scene tree)
    background = Color();
    camera.setFrom(Point3(0, 0, 1));
    camera.setLook(Vector3(0, 0, -1));
    camera.setUp(Vector3(0, 1, 0));
    camera.setZoom(60.0);
    camera.setNearFar(0.01, 100.0);
    camera.setProjectionCenter( Point3(0,0,0) );
    camera.setSkew(0.0);
    camera.setAspectRatioScale(1.0);

    lights.clear();

    ambientLight = Color(0,0,0);

    // reset your data here
    objectL.clear();//clear the object list
    
    //delete the transformation list
	for(map<string, Tree*>::iterator it = masterSubgraphs.begin(); it != masterSubgraphs.end();	it++){
    	(it->second)->clearTransL(); //it->second is a Tree*
	}
	//clear and delete the master subgraph map
    clearMasterSubgraph();
}

bool MyScene::loadSceneFile(std::string filename) {
    // load the scenefile with the given filename
    // return true if the file was loaded succesfully
    resetScene();
    // parse file:
    Parser p(filename);
    bool done = false;
    errorMessage = "";
    do {
        done = p.nextToken();
        if (p.getToken() == TOKEN_BACKGROUND)
            done = !parseBackground(p);
        else if (p.getToken() == TOKEN_CAMERA)
            done = !parseCamera(p);
        else if (p.getToken() == TOKEN_LIGHT)
            done = !parseLight(p);
        else if (p.getToken() == TOKEN_MASTERSUBGRAPH)
            done = (parseMasterSubgraph(p) == 0);
        else if (!done) {
            errorMessage = "Unrecognized token at root level: \"" + p.getToken() + "\"";
            done = true;
        }
    } while (!done);    
    // ToDo: check that there is a root mastersubgraph
    // and that no errors occured while loading
    isLoaded = (errorMessage.length() == 0) && (masterSubgraphs.find(string("root")) != masterSubgraphs.end());
    if (isLoaded) {
        // perform any post-loading computations here
        // (such as flattening the tree or building
        // ray-acceleration structures)
        Matrix4 id=Matrix4::identity();
		if(masterSubgraphs.find(string("root")) != masterSubgraphs.end())
			masterSubgraphs[string("root")]->createChildList(id, &objectL);
	
    } else if (errorMessage.length() == 0)
        errorMessage = "Unable to locate root mastersubgraph";

    return isLoaded;
}

bool MyScene::parseBackground(Parser& p) {
    // parse the background block
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_COLOR) {
          p.nextToken(); background[0] = static_cast<float>(p.getValue());
          p.nextToken(); background[1] = static_cast<float>(p.getValue());
          p.nextToken(); background[2] = static_cast<float>(p.getValue());
        } else {
            errorMessage = "Unrecognized token in background block: \"" + p.getToken() + "\"";
            return false;
        }
    } while (true);
    return true;
}

bool MyScene::parseCamera(Parser& p) {
    // parse the camera
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_EYE) {
            Point3 eye;
            p.nextToken(); eye[0] = p.getValue();
            p.nextToken(); eye[1] = p.getValue();
            p.nextToken(); eye[2] = p.getValue();
            camera.setFrom(eye);
        } else if (p.getToken() == TOKEN_LOOK) {
            Vector3 look;
            p.nextToken(); look[0] = p.getValue();
            p.nextToken(); look[1] = p.getValue();
            p.nextToken(); look[2] = p.getValue();
            camera.setLook(look);
        } else if (p.getToken() == TOKEN_FOCUS) {
            Point3 focus;
            p.nextToken(); focus[0] = p.getValue();
            p.nextToken(); focus[1] = p.getValue();
            p.nextToken(); focus[2] = p.getValue();
            camera.setAt(focus);
        } else if (p.getToken() == TOKEN_UP) {
            Vector3 up;
            p.nextToken(); up[0] = p.getValue();
            p.nextToken(); up[1] = p.getValue();
            p.nextToken(); up[2] = p.getValue();
            camera.setUp(up);
        } else if (p.getToken() == TOKEN_ANGLE) {
            p.nextToken(); camera.setZoom(p.getValue());
        } else if (p.getToken() == TOKEN_NEAR_FAR) {
            double n, f;
            p.nextToken(); n = p.getValue();
            p.nextToken(); f = p.getValue();
            camera.setNearFar(n, f);
        } else {
            errorMessage = "Unrecognized token in camera block: \"" + p.getToken() + "\"";
            return false;
        }
    } while (true);
    return true;
}

bool MyScene::parseLight(Parser& p) {
    // parse a light source block
    Light l;
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_LIGHTTYPE) {
            p.nextToken();
            if (p.getToken() == TOKEN_POINT)
                l.setType( Light::POINTLIGHT );
            else if (p.getToken() == TOKEN_DIRECTIONAL)
                l.setType( Light::DIRECTIONAL );
            else if (p.getToken() == TOKEN_SPOTLIGHT)
                l.setType( Light::SPOTLIGHT );
            else {
                errorMessage = "Unknown light type: \"" + p.getToken() + "\"";
                return false;
            }
        } else if (p.getToken() == TOKEN_POSITION) {
            Point3 pos;
            p.nextToken(); pos[0] = p.getValue();
            p.nextToken(); pos[1] = p.getValue();
            p.nextToken(); pos[2] = p.getValue();
            l.setPosition( pos );
        } else if (p.getToken() == TOKEN_COLOR) {
            Color color; 
            p.nextToken(); color[0] = static_cast<float>(p.getValue());
            p.nextToken(); color[1] = static_cast<float>(p.getValue());
            p.nextToken(); color[2] = static_cast<float>(p.getValue());
            color = MyScene::normCol(color);
            l.setColor( color );
        } else if (p.getToken() == TOKEN_FUNCTION) {
            float f0,f1,f2;
            p.nextToken(); f0 = static_cast<float>(p.getValue());
            p.nextToken(); f1 = static_cast<float>(p.getValue());
            p.nextToken(); f2 = static_cast<float>(p.getValue());
            l.setFalloff( f0, f1, f2 );
        } else if (p.getToken() == TOKEN_DIRECTION) {
            Vector3 dir;
            p.nextToken(); dir[0] = p.getValue();
            p.nextToken(); dir[1] = p.getValue();
            p.nextToken(); dir[2] = p.getValue();
            dir.normalize();
            l.setDirection( dir );
        } else if (p.getToken() == TOKEN_APERTURE) {
            p.nextToken(); 
        	float apt = p.getValue();
        	if(apt<=0.0) apt = 0.01;
        	else if(apt>180.0) apt = 180.0;
            l.setAperture( apt );
        } else if (p.getToken() == TOKEN_EXPONENT) {
            p.nextToken();
        	double exp = p.getValue();
        	if(exp<0.0) exp = 0.0; 
        	l.setExponent( exp );
        } else if (p.getToken() == TOKEN_AMBIENT) {
            p.nextToken(); ambientLight[0] = static_cast<float>(p.getValue());
            p.nextToken(); ambientLight[1] = static_cast<float>(p.getValue());
            p.nextToken(); ambientLight[2] = static_cast<float>(p.getValue());
        } else {
            errorMessage = "Unrecognized token in light block: \"" + p.getToken() + "\"";
            return false;
        }

    } while (true);
    assert( l.getColor()[0] >= 0.0 && l.getColor()[0] <= 1.0 );
    assert( l.getColor()[1] >= 0.0 && l.getColor()[1] <= 1.0 );
    assert( l.getColor()[2] >= 0.0 && l.getColor()[2] <= 1.0 );
    assert( l.getAperture() >  0.0 && l.getAperture() <= 180.0 );
    assert( l.getExponent() >= 0.0 );
    lights.push_back(l);
    return true;
}

Tree* MyScene::parseMasterSubgraph(Parser& p) {
    // ToDo: parse a named subgraph tree
    Tree* subgraph = new Tree();
    Node* trans = new Node();
    
    //we haven't seen anything yet
    
    p.nextToken();
    std::string name = p.getToken();

    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {        	
            break;
        } else if (p.getToken() == TOKEN_TRANS) {
            // parse node and add it to the tree
            // call parseTrans(p);
            trans = parseTrans(p);
            subgraph->addTrans(static_cast<Trans*>(trans));
        } else {
            errorMessage = "Unrecognized token in mastersubgraph \"" + name + "\": \"" + p.getToken() + "\"";
            return 0;
        }
    } while (true);

    // add the new master subgraph to the master subgraph list
	masterSubgraphs[name] = subgraph;
    // ToDo: Fix this
    return subgraph;
}

Node* MyScene::parseTrans(Parser& p) {
    // parse a trans block node
	Trans* trans = new Trans();
	Object* object = new Object();
	
    seenTransform=false; seenObject=false;
	
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
        	if(seenObject==false&&seenTransform==true){
				errorMessage = "No object to apply transformations to! \"";
				return 0;
			}
            break;
        } else if (p.getToken() == TOKEN_ROTATE) {
        	seenTransform=true;
            Vector3 axis;
            p.nextToken(); axis[0] = p.getValue();
            p.nextToken(); axis[1] = p.getValue();
            p.nextToken(); axis[2] = p.getValue();
            p.nextToken(); double angle = p.getValue() * M_PI / 180.0;
            // ToDo: add a rotation to the matrix stack here
            trans->addMat(Matrix4::rotation(axis,angle));            

        } else if (p.getToken() == TOKEN_TRANSLATE) {
        	seenTransform=true;
            Vector3 v;
            p.nextToken(); v[0] = p.getValue();
            p.nextToken(); v[1] = p.getValue();
            p.nextToken(); v[2] = p.getValue();
            // ToDo: add a translation to the matrix stack here
            trans->addMat(Matrix4::translation(v));

        } else if (p.getToken() == TOKEN_SCALE) {
        	seenTransform=true;
            Vector3 v;
            p.nextToken(); v[0] = p.getValue();
            p.nextToken(); v[1] = p.getValue();
            p.nextToken(); v[2] = p.getValue();
            // ToDo:add a scale to the matrix stack here
            trans->addMat(Matrix4::scaling(v));

        } else if (p.getToken() == TOKEN_MATRIXRC) {
        	seenTransform=true;
            Vector4 r0, r1, r2, r3;
            p.nextToken(); r0[0] = p.getValue();
            p.nextToken(); r0[1] = p.getValue();
            p.nextToken(); r0[2] = p.getValue();
            p.nextToken(); r0[3] = p.getValue();
      
            p.nextToken(); r1[0] = p.getValue();
            p.nextToken(); r1[1] = p.getValue();
            p.nextToken(); r1[2] = p.getValue();
            p.nextToken(); r1[3] = p.getValue();
      
            p.nextToken(); r2[0] = p.getValue();
            p.nextToken(); r2[1] = p.getValue();
            p.nextToken(); r2[2] = p.getValue();
            p.nextToken(); r2[3] = p.getValue();
      
            p.nextToken(); r3[0] = p.getValue();
            p.nextToken(); r3[1] = p.getValue();
            p.nextToken(); r3[2] = p.getValue();
            p.nextToken(); r3[3] = p.getValue();
            // ToDo: add the arbitrary matrix to the matrix stack here
            trans->addMat(Matrix4(r0, r1, r2, r3));

        } else if (p.getToken() == TOKEN_MATRIXCR) {
        	seenTransform=true;
            Vector4 r0, r1, r2, r3;
            p.nextToken(); r0[0] = p.getValue();
            p.nextToken(); r1[0] = p.getValue();
            p.nextToken(); r2[0] = p.getValue();
            p.nextToken(); r3[0] = p.getValue();
      
            p.nextToken(); r0[1] = p.getValue();
            p.nextToken(); r1[1] = p.getValue();
            p.nextToken(); r2[1] = p.getValue();
            p.nextToken(); r3[1] = p.getValue();
      
            p.nextToken(); r0[2] = p.getValue();
            p.nextToken(); r1[2] = p.getValue();
            p.nextToken(); r2[2] = p.getValue();
            p.nextToken(); r3[2] = p.getValue();
      
            p.nextToken(); r0[3] = p.getValue();
            p.nextToken(); r1[3] = p.getValue();
            p.nextToken(); r2[3] = p.getValue();
            p.nextToken(); r3[3] = p.getValue();
            // ToDo: add the arbitrary matrix to the matrix stack here
            trans->addMat(Matrix4(r0, r1, r2, r3));

        } else if (p.getToken() == TOKEN_OBJECT) {
            // ToDo: parse the object and add it to the node
            // call parseObject(p) here;
            if(seenObject==false){
				object = parseObject(p);
				trans->AddNode(static_cast<Node*>(object));
				seenObject=true;
			}
			else{
				while(p.getToken() != TOKEN_CB)
					p.nextToken();
			}

        } 
        else if (p.getToken() == TOKEN_SUBGRAPH) {
            // ToDo: find the subgraph and add it to the current node
            if(seenObject==false){
            	p.nextToken();
            	std::string subgraphName = p.getToken();
				if(masterSubgraphs.find(subgraphName) != masterSubgraphs.end())
                	trans->AddNode(masterSubgraphs[subgraphName]);
            	else{
            		errorMessage = "Unrecognized subgraph name: \"" + subgraphName + "\"";
					// ToDo: Clean up
					resetScene();
					return 0;
            	}
        		seenObject=true;
        	}
        	else{
				p.nextToken();
			}
        }
         else {
            errorMessage = "Unrecognized token in trans block: \"" + p.getToken() + "\"";
            // ToDo: Clean up
            resetScene();
            return 0;
        }
    } while (true);

    // ToDo: Fix this
    return trans;
}

Object* MyScene::parseObject(Parser& p) {
    // ToDo: parse an object block
	Object* object = new Object();

    p.nextToken();
    if (p.getToken() == TOKEN_CUBE) {
        // object is a cube
        object->setShape(cube);

    } else if (p.getToken() == TOKEN_CYLINDER) {
        // object is a cylinder
        object->setShape(cylinder);

    } else if (p.getToken() == TOKEN_CONE) {
        // object is a cone
        object->setShape(cone);

    } else if (p.getToken() == TOKEN_SPHERE) {
        // object is a sphere
        object->setShape(sphere);

    } else if (p.getToken() == TOKEN_TORUS) {
        object->setShape(torus);

    } else {
        errorMessage = "Unrecognized object type: \"" + p.getToken() + "\"";
        return 0;
    }
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_AMBIENT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store ambient color here
            Color color = Color(r, g, b);
            color = MyScene::normCol(color);
            object->setAmbient(color);

        } else if (p.getToken() == TOKEN_DIFFUSE) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store diffuse color here
            Color color = Color(r, g, b);
            color = MyScene::normCol(color);
            object->setDiffuse(color);

        } else if (p.getToken() == TOKEN_SPECULAR) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store specular color here
            Color color = Color(r, g, b);
            color = MyScene::normCol(color);
            object->setSpecular(color);

        } else if (p.getToken() == TOKEN_REFLECT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store reflect color here
            Color color = Color(r, g, b);
            color = MyScene::normCol(color);
            object->setReflect(color);

        } else if (p.getToken() == TOKEN_TRANSPARENT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store transparent color here
            Color color = Color(r, g, b);
            color = MyScene::normCol(color);
            object->setTransparent(color);
            object->isTransparent=true;

        } else if (p.getToken() == TOKEN_EMIT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store emitted color here
            Color color = Color(r, g, b);
            color = MyScene::normCol(color);
            object->setEmit(color);

        } else if (p.getToken() == TOKEN_SHINE) {
            double s;
            p.nextToken(); s = p.getValue();
            // store shine here
            object->setShine(s);

        } else if (p.getToken() == TOKEN_ALPHA) {
            double a;
            p.nextToken(); a = p.getValue();
            // store alpha here
            object->setAlpha(a);

        } else if (p.getToken() == TOKEN_IOR) {
            double ior;
            p.nextToken(); ior = p.getValue();
            // store ior here
            object->setIor(ior);

        } else if (p.getToken() == TOKEN_TEXTURE) {
            std::string filename;
            double textureU, textureV;
            p.nextToken(); filename = p.getToken();
            p.nextToken(); textureU = p.getValue();
            p.nextToken(); textureV = p.getValue();
            // store texture information here
			object->setTexture(filename);
			object->setU(textureU);
			object->setV(textureV);

        } else {
            errorMessage = "Unrecognized token in object block: \"" + p.getToken() + "\"";
            return 0;
        }
    } while (true);

    // ToDo: Fix this
    return object;
}

std::string MyScene::getErrorMessage() const {
    // return the current error message
    return errorMessage;
}

const Camera & MyScene::getCamera() const {
    // return the current camera
    return camera;
}

Camera & MyScene::changeCamera() {
    // return the current camera
    return camera;
}

const Color &MyScene::getBackground() const {
    return background;
}

//////////////////////
// These are for the classes

Color white=Color(1.,1.,1.);//default for diffuse
Color black=Color(0.,0.,0.);//default for ambient reflect transparent and specular

/* MATRIXOBJ */
MatObj::MatObj(Matrix4 mat_, Object* obj_){
	mat = mat_; obj = obj_;
}

MatObj::~MatObj(){}

/* NODE */
Node::Node(){
	Node::isObj=false;
}
Node::~Node(){}

void Node::createChildList(Matrix4& m, list<MatObj>* l){}

/* OBJECT */
Object::Object(MyShape* s){
	Node::isObj=true;
	isTransparent=false;
	shape = s;
	ambient = black;
	diffuse = white;
	specular = black;
	reflect = black;
	transparent = black;
	emit = black;
	shine = 1.0;
	alpha = 0.5;
	ior = 1.0;
}

Object::Object(){
	Node::isObj=true;
	isTransparent=false;
	shape = 0;
	ambient = black;
	diffuse = white;
	specular = black;
	reflect = black;
	transparent = black;
	emit = black;
	shine = 1.0;
	alpha = 0.5;
	ior = 1.0;
}

Object::~Object(){}

void Object::createChildList(Matrix4& m, list<MatObj>* l){
		l->push_back(MatObj(m, this));
}

void Object::drawObj(){
		if(isTransparent){
			GLfloat transpMt[] = {transparent[0], transparent[1], transparent[2], alpha};//set transparency to alpha
			glMaterialfv(GL_FRONT, GL_DIFFUSE, transpMt);
		}
		GLfloat ambMt[] = {ambient[0], ambient[1], ambient[2], 1};
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambMt);
		GLfloat difMt[] = {diffuse[0], diffuse[1], diffuse[2], 1};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, difMt);
		GLfloat specMt[] = {specular[0], specular[1], specular[2], 1};
		glMaterialfv(GL_FRONT, GL_SPECULAR, specMt);
		GLfloat shininess = static_cast<GLfloat>(shine);
		glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
		GLfloat emMt[] = {emit[0], emit[1], emit[2], 1};
		glMaterialfv(GL_FRONT, GL_EMISSION, emMt);
	
		shape->drawShape();	
}

/* TREE */
Tree::Tree(){
	Node::isObj=false;
}

Tree::~Tree(){
	clearTransL();
}

void Tree::createChildList(Matrix4& m, list<MatObj>* l){
	for(list<Trans*>::iterator it = transL.begin(); it != transL.end(); it++){
		(*it)->createChildList(m, l);
	}
}

void Tree::addTrans(Trans* t){
	transL.push_back(t);
}

void Tree::clearTransL(){//clear the trans list
	//delete each trans
	for(list<Trans*>::iterator it = transL.begin(); it != transL.end(); it++)
		delete (*it);
	transL.clear();//clear the list
}

/* TRANS */
Trans::Trans(){
	mat = Matrix4::identity();
	node = 0;
}

Trans::~Trans(){
	if(node != 0 && Node::isObj==true)
		delete node;
}

void Trans::createChildList(Matrix4& m, list<MatObj>* l){
	Matrix4 matrix = m*mat;
	node->createChildList(matrix, l);
}

void Trans::addMat(Matrix4 m){ //row-major order
	mat *= m;
}

void Trans::AddNode(Node *n){//what node to we apply the trans to
	node=n;
}

Color MyScene::normCol(Color color){
// color normalization and bounds setting
	int max = 1;
	for(int i=0; i<3;i++){
		if(color[i]<0.0)
			color[i]=0.0;
		else{
			if(color[i]>max){
				max=color[i];
			}
		}
	}
	if(max>1){
		for(int i=0; i<3;i++){
			color[i]/=max;
		}
	}
	return color;
}