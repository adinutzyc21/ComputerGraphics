#include "cse452.h"
#include "ScreenPoint.h"
#include "BrushInterface.h"
#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/fl_draw.H>
#include <FL/gl.h>
#include <cstring>
#include <cmath>

MyBrush::MyBrush() 
{
  isMouseDown = false;

  imageWidth  = screenWidth = 0;
  imageHeight = screenHeight = 0;

  maskSize = 0;
}

MyBrush::~MyBrush() {
  // destroy your data here
}

void MyBrush::resize(int width, int height) {
  screenWidth  = width;
  screenHeight = height;

  // First time initialization
  if ( imageWidth == 0 ) {
    imageWidth = screenWidth;
    imageHeight = screenHeight;

    // Make image black
    pixelData.resize( width * height * 3, 0 );
  }
}

void MyBrush::loadImage(Fl_Image* image) {
  imageWidth = image->w();
  imageHeight = image->h();
  // Reset viewport
  resize( screenWidth, screenHeight );
  pixelData.resize( imageWidth * imageHeight * 3, 0 );

  // OpenGL's windows are reversed in y
  const int delta = imageWidth * 3;
  unsigned char* src = (unsigned char*) *image->data();
  for (int i = 0; i < imageHeight; i++) {
    // Ok, this is ugly
    unsigned char* dest = &pixelData[ ((imageHeight - 1 - i) * imageWidth * 3) ];
    memcpy(dest, src, delta);
    src += delta;
  }
}

void MyBrush::draw() {
  // Set up camera for drawing
  setup2DDrawing( Color(0,0,0), screenWidth, screenHeight );

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Draw a border around the actual image
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2i( 0,            0 );
  glVertex2i( imageWidth+1, 0 );
  glVertex2i( imageWidth+1, imageHeight+1 );
  glVertex2i( 0,            imageHeight+1 );
  glEnd();


  glRasterPos2i(0, 0);
  // Copy data into window
  //for ( int iX = 0; iX < 100; iX++ )
  //putPixel( iX, iX, Color(1,0,0) );

  glDrawPixels(imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, &pixelData[0]);

  // These 5 lines draw a white line across your canvas
  // Remove this and replace it with intelligent OpenGL preview code
  /*glLineWidth( 10);
  glBegin( GL_LINES );
  glVertex2i( 100, 100 );
  glVertex2i( 200, 200 );
  glEnd();*/

  // Add in your OpenGL pre-view code here
  ToolType toolType = brushUI->getToolType();
  Color colBrush = brushUI->getColor();
  int radius = brushUI->getRadius();
  switch(toolType){
		case TOOL_BRUSH:
				// no drawing happening here
				if (!isMouseDown) {
					int x=mouseDrag.getX(); int y=mouseDrag.getY();
					const float pi=float(3.1415926535898);
					int circle_points = 40;
					float angle = 2* pi/circle_points;
					float angle1;
					glLineWidth(1);
					glBegin(GL_LINE_LOOP);
						angle1 = 0.0;
						glColor3f(colBrush[0],colBrush[1],colBrush[2]);
						for(int i=0;i<=circle_points;i++){
							glVertex2f(x+radius*float(cos(angle1)), y+radius*float(sin(angle1)));
							angle1 += angle ;
						}
					glEnd();
				}
				break;
			case TOOL_LINE:
				if(isMouseDown){
					int x1=mouseDown.getX(); int y1=mouseDown.getY();
					int x2=mouseDrag.getX(); int y2=mouseDrag.getY();
					glLineWidth(radius);
					glBegin(GL_LINES);
						glColor3f(colBrush[0],colBrush[1],colBrush[2]);
						glVertex2f(x1,y1);
						glVertex2f(x2,y2);
					glEnd();
				}
				break;
			case TOOL_CIRCLE:
				if(isMouseDown){
					int x = mouseDown.getX();int y = mouseDown.getY();
					int x1 = mouseDrag.getX(); int y1 = mouseDrag.getY();
					int r = sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y));
					const float pi=float(3.1415926535898);
					int circle_points = 50;
					float angle = 2.0f * pi/circle_points;
					float angle1;
					
					glLineWidth(radius);
					glBegin(GL_LINE_LOOP);
						angle1 = 0.0;
						glColor3f(colBrush[0],colBrush[1],colBrush[2]);
						for(int i=0;i<=circle_points;i++){
							glVertex2f(x+r*float(cos(angle1)), y+r*float(sin(angle1)));
							angle1 += angle ;
						}
					glEnd();
				}
		  break;
  }
  endDrawing();
}

// This does pixel flow
void MyBrush::draw_callback( void *in_data )
{
  MyBrush *opMe = static_cast<MyBrush *>( in_data );

  // Repeat the time out if we're not done yet
  if ( opMe->isMouseDown == true ) {
    opMe->drawBrush();

    Fl::repeat_timeout( 0.05, MyBrush::draw_callback, (void *) opMe );

    RedrawWindow();
  }
}


int MyBrush::handle(int event) {
  // OpenGL & FLTK's y axes are oriented differently
  const ScreenPoint pt = ScreenPoint( Fl::event_x(), screenHeight - 1 - Fl::event_y() );

  switch (event) {
  case FL_PUSH: {
    mouseDrag = pt;
    mouseDown = pt;

    if (brushUI->getToolType() == TOOL_POLYGON) {
      if (isMouseDown == true) {
	polygon.push_back( mouseDrag );
      } else {
	isMouseDown = true;
	polygon.resize(0);
	polygon.push_back( mouseDrag );
      }
    } else {
      isMouseDown = true;
      if (brushUI->getToolType() == TOOL_BRUSH)
	Fl::add_timeout(0, draw_callback, this);
    }
    return 1;
  }
  case FL_DRAG: mouseDrag = pt; RedrawWindow(); return 1;
  case FL_MOVE: 
    mouseDrag = pt;
    if ( brushUI->getToolType() == TOOL_BRUSH || ( brushUI->getToolType() == TOOL_POLYGON && isMouseDown ) )
      RedrawWindow();
    return 1;
  case FL_RELEASE: {
    mouseDrag = pt;
    if (brushUI->getToolType() != TOOL_POLYGON) {
      isMouseDown = false;
      switch (brushUI->getToolType()) {
      case TOOL_BRUSH: 
	break;
      case TOOL_LINE: 
	drawLine( ); 
	break;
      case TOOL_CIRCLE: 
	drawCircle( );
	break;
      case TOOL_FILTER: 
	filterRegion( ); 
	break;
      default: break;
      }
    } else if ( Fl::event_button3() || Fl::event_state( FL_SHIFT ) ) {
      isMouseDown = false;
      drawPolygon();
    }
    RedrawWindow();
    return 1;
  }
  default: return 0;
  }
}
