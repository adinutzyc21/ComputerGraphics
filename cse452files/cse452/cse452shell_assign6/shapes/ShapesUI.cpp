#include "../cse452.h"
#include "ShapesUI.h"
#include "ShapesInterface.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>

ShapesUI::ShapesUI() {
    width = height = 0;

    // ToDo: initialize your variables here
	currentShape = 0;//we have no shape yet
}

ShapesUI::~ShapesUI() {
    // ToDo: delete your variables here
    if(currentShape!=0)//if we have a shape, delete it
    	delete currentShape;
    currentShape = 0;
}

void ShapesUI::resize(int w, int h) {
    width = w;
    height = h;
}

void ShapesUI::draw() {
    // Sets up the viewport and background color
    setup3DDrawing( Color( 0,0,0 ), width, height, true );

    // Changes the way triangles are drawn
    switch ( shapesUI->getDisplayType()) {
        case DISPLAY_WIREFRAME: {
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_LINE);
            glColor3f(1.0f, 1.0f, 1.0f);
        } break;
        case DISPLAY_FLAT_SHADING: {
            glEnable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_FILL);
            glColor3f(1.0f, 1.0f, 1.0f);
            glShadeModel(GL_FLAT);
        } break;
        case DISPLAY_SMOOTH_SHADING: {
            glEnable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_FILL);
            glColor3f(1.0f, 1.0f, 1.0f);
            glShadeModel(GL_SMOOTH);
        } break;
        default: break;
    }

    // Setup the camera
    gluLookAt( 3.5 * cos( shapesUI->getYRot() ) * cos( shapesUI->getXRot() ), 
               3.5 * sin( shapesUI->getYRot() ), 
               3.5 * cos( shapesUI->getYRot() ) * sin( shapesUI->getXRot() ), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // ToDo: draw your shape here
    // DO NOT put the actual draw OpenGL code here - put it in the shape class and call the draw method
	
	currentShape->drawShape();
    RedrawWindow();
    endDrawing();
}

int ShapesUI::handle(int event) {
    return 0;
}

void ShapesUI::changedShape()
{
    // ToDo: Switch shapes
	switchShape();
}

void ShapesUI::changedTessel( ) {
    // ToDo: tessellate your shape here
    switchShape();
}

void ShapesUI::switchShape(){//re-calculates the shape based on the parameters given
	if(currentShape!=0)//if we do have a current shape
		delete currentShape;//delete it
	currentShape = 0;
	switch(shapesUI->getShapeType()){
		case SHAPE_CUBE:
			currentShape = new Cube(shapesUI->getTessel1());
			break;
		case SHAPE_SPHERE:
			currentShape = new Sphere(shapesUI->getTessel1());
			break;
		case SHAPE_CYLINDER:
			currentShape = new Cylinder(shapesUI->getTessel1(),shapesUI->getTessel2());
			break;
		case SHAPE_CONE:
			currentShape = new Cone(shapesUI->getTessel1(),shapesUI->getTessel2());
			break;
		case SHAPE_TORUS:
			currentShape = new Torus(shapesUI->getTessel1(),shapesUI->getTessel2());
			break;
	}    
	
    RedrawWindow();
}
