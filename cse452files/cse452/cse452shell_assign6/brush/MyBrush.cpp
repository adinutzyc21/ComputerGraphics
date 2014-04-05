#include "cse452.h"
#include "MyBrush.h"
#include "BrushInterface.h"
#include <cmath>
#include <iostream>
#include <algorithm> // min
using namespace std; //cout instead of std::cout

void MyBrush::changedBrush() {
  // this is called anytime the brush type or brush radius changes
  // it should recompute the brush mask appropriately
  const int radius = brushUI->getRadius();
  maskSize = 2*radius+1;
  mask.resize(maskSize*maskSize);
  BrushType brushType = brushUI->getBrushType();
  switch(brushType){
	case BRUSH_CONSTANT:
		//printf("constant\n");
		constDist(radius);//use the constant distribution
		break;
	case BRUSH_LINEAR:
		//printf("linear\n");
		linDist(radius);//use the linear distribution
		break;
	case BRUSH_QUADRATIC:
		//printf("quadratic\n");
		quadDist(radius);//use the quadratic distribution
		break;
	case BRUSH_GAUSSIAN:
		//printf("gaussian\n");
		gausDist(radius);//use the gaussian distribution
		break;
	case BRUSH_SPECIAL:
		//printf("special\n");
		specDist(radius);//use the special distribution
		break;
  }
}

void MyBrush::drawBrush( ) {
  // apply the current brush mask to image location (x,y)
  // the mouse location is in mouseDrag

  const int radius = brushUI->getRadius();
  const float pixelFlow = brushUI->getFlow();
  const Color colBrush = brushUI->getColor();

  int mouseX = mouseDrag.getX();
  int mouseY = mouseDrag.getY();
  
  int Xlim0 = max(mouseX-radius, 0);
  int Xlim1 = min(screenWidth, mouseX+radius);
  int Ylim0 = max(mouseY-radius, 0);
  int Ylim1 = min(screenHeight, mouseY+radius);
  float m;
  
  for(int j = Ylim0; j < Ylim1; j++){
	  for(int i = Xlim0; i < Xlim1 ; i++){
		  m = getVal(i-mouseX+radius,j-mouseY+radius);
		  putPixel(i, j, m * pixelFlow * colBrush + (1 - m * pixelFlow) * getPixel(i,j));
	  }
  }
}

void MyBrush::drawLine( ) {
  // draw a thick line from mouseDown to mouseDrag
  // the width of the line is given by the current brush radius
  const int radius = brushUI->getRadius();
  const Color colBrush = brushUI->getColor();
  
  int x0 = mouseDown.getX();
  int y0 = mouseDown.getY();
  int x1 = mouseDrag.getX();
  int y1 = mouseDrag.getY();
  
  if(x1==x0){//vertical line
	  int x;
	  for(int w=-radius/2; w<=radius/2; w++){
		  x=x0+w;
		  for(int y=min(y0,y1); y<max(y0,y1); y++)
			  if(x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
				  putPixel(x,y,colBrush);
	  }
  }
  else if(y1==y0){//horizontal line
	  int y;
	  for(int w=-radius/2; w<=radius/2; w++){
		  y=y0+w;
		  for(int x=min(x0,x1); x<max(x0,x1); x++)
			  if(x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
				  putPixel(x,y,colBrush);
	  }
  }
  else{
	float m = (y1-y0)/float(x1-x0);
	  
	float m2 = -1/m; //the slope of the perpendicular line
	
	float xaux = (radius/float(2*sqrt(m2*m2+1)))+x0;
	float yaux = m2*(xaux-x0)+y0;
	int x00=int(xaux),y00=int(yaux);
	xaux = -(radius/float(2*sqrt(m2*m2+1)))+x0; yaux = m2*(xaux-x0)+y0;
	int x01=int(xaux),y01=int(yaux);

	xaux = (radius/float(2*sqrt(m2*m2+1)))+x1; yaux = m2*(xaux-x1)+y1;
	int x10=int(xaux),y10=int(yaux);
	xaux = -(radius/float(2*sqrt(m2*m2+1)))+x1; yaux = m2*(xaux-x1)+y1;
	int x11=int(xaux),y11=int(yaux);
	  
	//draw the 4 lines
	  if(abs(m)<1){
		drawThinLine1(colBrush,x00,y00,x10,y10);
		drawThinLine1(colBrush,x01,y01,x11,y11);  
	  }
	  else {
		  drawThinLine2(colBrush,x00,y00,x10,y10);
		  drawThinLine2(colBrush,x01,y01,x11,y11); 
	  }
	  if(abs(m2)<1){
		  drawThinLine1(colBrush,x00,y00,x01,y01);
		  drawThinLine1(colBrush,x10,y10,x11,y11);  
	  }
	  else {
		  drawThinLine2(colBrush,x00,y00,x01,y01);
		  drawThinLine2(colBrush,x00,y00,x01,y01); 
	  }

	
	// fill the polygon betweeen the lines
	fillLine(colBrush,x00,y00,x01,y01,x10,y10,x11,y11);
  }
}


void MyBrush::drawCircle() {
  // drawa thick circle at mouseDown with radius r 
  // the width of the circle is given by the current brush radius
	const int thickness = brushUI->getRadius();
	const Color colBrush = brushUI->getColor();
	
	int x0 = mouseDown.getX();
	int y0 = mouseDown.getY();
	int x1 = mouseDrag.getX();
	int y1 = mouseDrag.getY();
	
	int radius = round(sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0)));	
	
	int thickness2;
	if(thickness>=radius)
		thickness2=1;
	else
		thickness2=radius-thickness;
	
	maskSize = 2*radius+1;
	mask.resize(maskSize*maskSize);
	circle(radius,(thickness2));
	
	int Xlim0 = max(x0-radius, 0);
	int Xlim1 = min(screenWidth, x0+radius);
	int Ylim0 = max(y0-radius, 0);
	int Ylim1 = min(screenHeight, y0+radius);
	float m;
	
	for(int j = Ylim0; j < Ylim1; j++){
		for(int i = Xlim0; i < Xlim1 ; i++){
			m = getVal(i-x0+radius,j-y0+radius);
			putPixel(i, j, m * colBrush + (1 - m) * getPixel(i,j));
		}
	}
		
	
}

void MyBrush::drawPolygon() {
  // draw a polygon with numVertices whos coordinates are stored in the
  // polygon array: {x0, y0, x1, y1, ...., xn-1, yn-1}
}

void MyBrush::filterRegion( ) {
  // apply the filter indicated by filterType to the square
  // defined by the two corner points mouseDown and mouseDrag
  // these corners are not guarenteed to be in any order
  // The filter width is given by the brush radius
	
}

//special distribution: circle
void MyBrush::circle(int radius, int thickness){
	int u2,v2;
	int centerX=maskSize-radius-1;
	int centerY=maskSize-radius-1;
	
	int r2=radius*radius;
	
	const float c2 = thickness*thickness; //the width of the donut
	
	for(int i=0; i<maskSize; i++){
		u2=(i-centerX)*(i-centerX);
		for(int j=0; j<maskSize; j++){
			v2=(j-centerY)*(j-centerY);
			
			//1 if in circle, 0 otherwise
			if(u2+v2<r2 && u2+v2>c2){
				setVal(i,j,1);
			}
			else
				setVal(i,j,0);
		}
	}
}

/*MASK DISTRIBUTIONS*/
//constant distribution
void MyBrush::constDist(int radius){;
	int u2,v2;
	int centerX=maskSize-radius-1;
	int centerY=maskSize-radius-1;
	int r2=int(pow(float(radius),2));

	for(int i=0; i<maskSize; i++){
		u2=int(pow(float(i-centerX),2));
		for(int j=0; j<maskSize; j++){
			v2=int(pow(float(j-centerY),2));
			
			//1 if in circle, 0 otherwise
			if(u2+v2<r2)
				setVal(i,j,1);
			else
				setVal(i,j,0);
		}
	}
}

//linear distribution: 1-d/r
void MyBrush::linDist(int radius){
	int u2,v2;
	int centerX=maskSize-radius-1;
	int centerY=maskSize-radius-1;
	int r2=int(pow(float(radius),2));

	for(int i=0; i<maskSize; i++){
		u2=int(pow(float(i-centerX),2));
		for(int j=0; j<maskSize; j++){
			v2=int(pow(float(j-centerY),2));

			//1-d/r if in circle, 0 otherwise
			if(u2+v2<r2)
				setVal(i,j,1-sqrt(float(float(u2+v2)/float(r2))));
			else	
				setVal(i,j,0);
		}
	}
}

//quadratic distribution: 1-(d/r)^2
void MyBrush::quadDist(int radius){
	int u2,v2;
	int centerX=maskSize-radius-1;
	int centerY=maskSize-radius-1;
	int r2=int(pow(float(radius),2));

	for(int i=0; i<maskSize; i++){
		u2=int(pow(float(i-centerX),2));
		for(int j=0; j<maskSize; j++){
			v2=int(pow(float(j-centerY),2));

			//1-(d/r)^2 if in circle, 0 otherwise
			if(u2+v2<r2)
				setVal(i,j,1-float(u2+v2)/float(r2));
			else
				setVal(i,j,0);
		}
	}
}

//gaussian distribution: f(x) = a*e^(-((x-b)^2)/(2*c^2))
void MyBrush::gausDist(int radius){
	int u2,v2;
	float num;
	int centerX=maskSize-radius-1;
	int centerY=maskSize-radius-1;
	int r2=int(pow(float(radius),2));

	for(int i=0; i<maskSize; i++){
		u2=int(pow(float(i-centerX),2));
		for(int j=0; j<maskSize; j++){
			v2=int(pow(float(j-centerY),2));

			//f(x) if in circle, 0 otherwise
			if(u2+v2<r2){
				//exponent
				//numerator
				num = radius-sqrt(float(u2+v2));
				num = pow(num,2);
				//denominator=r2
			
				setVal(i,j,1-exp(-num/r2));
			}
			else
				setVal(i,j,0);
		}
	}
}

//special distribution: doughnut //maybe make a star
void MyBrush::specDist(int radius){
	int u2,v2;
	float num;
	int centerX=maskSize-radius-1;
	int centerY=maskSize-radius-1;
	int r2=int(pow(float(radius),2));
	const float c2 = pow(float(radius/2+1),2); //the width of the donut

	float val = (c2+r2)/1;
	for(int i=0; i<maskSize; i++){
		u2=int(pow(float(i-centerX),2));
		for(int j=0; j<maskSize; j++){
			v2=int(pow(float(j-centerY),2));

			//1 if in donut, 0 otherwise
			if(u2+v2<r2 && u2+v2>c2){
				setVal(i,j,float(u2+v2)/float(r2));//quadratic distribution
			}
			else
				setVal(i,j,0);
		}
	}
}

//helper functions
void MyBrush::setVal(int row, int col, float value){
	mask[maskSize * row + col] = value;
}

float MyBrush::getVal(int row, int col){
	return mask[maskSize * row + col];
}

int MyBrush::round(float d){
  return int(floor(d + 0.5));
}


//thin lines
//draw a 1px width line between 2 points 
//-1<=slope<=1
void MyBrush::drawThinLine1(Color col, int x0, int y0, int x1, int y1){
	if (x0 > x1){//make sure you only need one call by swapping
		int aux=x1;x1=x0;x0=aux;//swap
		aux=y1;y1=y0;y0=aux;//swap
	}
	int dx = x1 - x0, dy = y1 - y0;
	int slope = 1;
	int incrE, incrNE, d;
	if (dy < 0){
		slope = -1;
		dy = -dy;
	}
	
	incrE = 2*dy;
	incrNE = 2*(dy-dx);;
	d = 2*dy-dx;
	int y = y0;
	
	for (int x = x0; x <= x1; x++){
		if(x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
			putPixel(x,y,col);
		if (d <= 0)
			d += incrE;
		else{
			d += incrNE;
			y += slope;
		}
	}
}

//draw a 1px width line between 2 points 
//other case 
void MyBrush::drawThinLine2(Color col, int x0, int y0, int x1, int y1){
	int aux;
	//swap x and y values
	aux=x1;x1=y1;y1=aux;
	aux=x0;x0=y0;y0=aux;
	
	if (x0 > x1){//make sure you only need one call by swapping
		aux=x1;x1=x0;x0=aux;//swap
		aux=y1;y1=y0;y0=aux;//swap
	}
	
	int dx = x1 - x0, dy = y1 - y0;
	int slope = 1;
	int incrE, incrNE, d;
	if (dy < 0){
		slope = -1;
		dy = -dy;
	}
	
	incrE = 2*dy;
	incrNE = 2*(dy-dx);;
	d = 2*dy-dx;
	int y = y0;
	
	for (int x = x0; x <= x1; x++){
		if(y >= 0 && y < screenWidth && x >= 0 && x < screenHeight)
			putPixel(y,x,col);
		if (d <= 0)
			d += incrE;
		else{
			d += incrNE;
			y += slope;
		}
	}
}

void MyBrush::fillLine(Color col, int x00, int y00, int x01, int y01, int x10, int y10, int x11, int y11){
	//l1: (x00,y00) to (x01,y01) y=mx+b 
	float m1 = (y01-y00)/float(x01-x00);
	float b1 = y00-m1*x00;
	//l2: (x00,y00) to (x10,y10) y=mx+b 
	float m2 = (y10-y00)/float(x10-x00);
	float b2 = y00-m2*x00;
	//l3: (x10,y10) to (x11,y11) y=mx+b 
	float m3 = (y11-y10)/float(x11-x10);
	float b3 = y10-m3*x10;
	//l4: (x01,y01) to (x11,y11) y=mx+b 
	float m4 = (y11-y01)/float(x11-x01);
	float b4 = y01-m4*x01;
	
	int y_min = min(min(y00,y01),min(y10,y11));
	int y_max = max(max(y00,y01),max(y10,y11));
	std::vector< int > inters; int numinters;
	
	//x=(y-b)/m
	for(int y=y_min; y<=y_max; y++){
		//between the limits of l1
		if(y>=min(y00,y01)&&y<=max(y00,y01)){
			inters.push_back((y-b1)/m1);
		}
		//between the limits of l2
		if(y>=min(y00,y10)&&y<=max(y00,y10)){
			inters.push_back((y-b2)/m2);
		}
		//between the limits of l3
		if(y>=min(y10,y11)&&y<=max(y10,y11)){
			inters.push_back((y-b3)/m3);
		}
		//between the limits of l4
		if(y>=min(y11,y01)&&y<=max(y11,y01)){
			inters.push_back((y-b4)/m4);
		}
		numinters=inters.size();// cout<<numinters<<"\n";
		if(numinters==2){
			//draw line between intes[0] and inters[1]
			int x_min=min(inters[0],inters[1]),x_max=max(inters[0],inters[1]);
			for(int x=x_min; x<=x_max; x++)
				if(x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
					putPixel(x,y,col);
		}
		else if(numinters==1){
			int x=inters[0];
			if(x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
				putPixel(x,y,col);
		}
		
		else if(numinters==3){
			//draw line between inters[0] and inters[1]
			int x1 = inters[0], x2=inters[1], x3=inters[2];
			int x_min=min(min(x1,x2),x3),x_max=max(max(x1,x2),x3);
			for(int x=x_min; x<=x_max; x++)
				if(x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
					putPixel(x,y,col);
		}
		else {
			cout<<"uhm\n";
		}
		
		
		inters.resize(0);
	}
	
}
