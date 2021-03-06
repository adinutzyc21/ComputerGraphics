// generated by Fast Light User Interface Designer (fluid) version 1.0302

#include "IntersectionInterface.h"

void IntersectionInterface::cb_Sphere_i(Fl_Menu_*, void*) {
  intersectionUI.changeShape( (ShapesUI::ShapeType) m_iShapeType->value());
RedrawWindow();
}
void IntersectionInterface::cb_Sphere(Fl_Menu_* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_Sphere_i(o,v);
}

void IntersectionInterface::cb_Cone_i(Fl_Menu_*, void*) {
  intersectionUI.changeShape( (ShapesUI::ShapeType) m_iShapeType->value());
RedrawWindow();
}
void IntersectionInterface::cb_Cone(Fl_Menu_* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_Cone_i(o,v);
}

void IntersectionInterface::cb_Cylinder_i(Fl_Menu_*, void*) {
  intersectionUI.changeShape( (ShapesUI::ShapeType) m_iShapeType->value());
RedrawWindow();
}
void IntersectionInterface::cb_Cylinder(Fl_Menu_* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_Cylinder_i(o,v);
}

void IntersectionInterface::cb_Cube_i(Fl_Menu_*, void*) {
  intersectionUI.changeShape( (ShapesUI::ShapeType) m_iShapeType->value());
RedrawWindow();
}
void IntersectionInterface::cb_Cube(Fl_Menu_* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_Cube_i(o,v);
}

void IntersectionInterface::cb_Torus_i(Fl_Menu_*, void*) {
  intersectionUI.changeShape( (ShapesUI::ShapeType) m_iShapeType->value());
RedrawWindow();
}
void IntersectionInterface::cb_Torus(Fl_Menu_* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_Torus_i(o,v);
}

Fl_Menu_Item IntersectionInterface::menu_m_iShapeType[] = {
 {"Sphere", 0,  (Fl_Callback*)IntersectionInterface::cb_Sphere, 0, 4, FL_NORMAL_LABEL, 0, 14, 0},
 {"Cone", 0,  (Fl_Callback*)IntersectionInterface::cb_Cone, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Cylinder", 0,  (Fl_Callback*)IntersectionInterface::cb_Cylinder, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Cube", 0,  (Fl_Callback*)IntersectionInterface::cb_Cube, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Torus", 0,  (Fl_Callback*)IntersectionInterface::cb_Torus, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

void IntersectionInterface::cb_m_dXAt_i(Fl_Value_Slider*, void*) {
  RedrawWindow();
}
void IntersectionInterface::cb_m_dXAt(Fl_Value_Slider* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_m_dXAt_i(o,v);
}

void IntersectionInterface::cb_m_dYAt_i(Fl_Value_Slider*, void*) {
  RedrawWindow();
}
void IntersectionInterface::cb_m_dYAt(Fl_Value_Slider* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_m_dYAt_i(o,v);
}

void IntersectionInterface::cb_m_dZAt_i(Fl_Value_Slider*, void*) {
  RedrawWindow();
}
void IntersectionInterface::cb_m_dZAt(Fl_Value_Slider* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_m_dZAt_i(o,v);
}

void IntersectionInterface::cb_m_dTheta_i(Fl_Value_Slider*, void*) {
  RedrawWindow();
}
void IntersectionInterface::cb_m_dTheta(Fl_Value_Slider* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_m_dTheta_i(o,v);
}

void IntersectionInterface::cb_m_dPhi_i(Fl_Value_Slider*, void*) {
  RedrawWindow();
}
void IntersectionInterface::cb_m_dPhi(Fl_Value_Slider* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_m_dPhi_i(o,v);
}

void IntersectionInterface::cb_Write_i(Fl_Button*, void*) {
  intersectionUI.writeTest();
}
void IntersectionInterface::cb_Write(Fl_Button* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_Write_i(o,v);
}

void IntersectionInterface::cb_m_dXRot_i(Fl_Value_Slider*, void*) {
  RedrawWindow();
}
void IntersectionInterface::cb_m_dXRot(Fl_Value_Slider* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_m_dXRot_i(o,v);
}

void IntersectionInterface::cb_m_dYRot_i(Fl_Value_Slider*, void*) {
  RedrawWindow();
}
void IntersectionInterface::cb_m_dYRot(Fl_Value_Slider* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_m_dYRot_i(o,v);
}

void IntersectionInterface::cb_m_bGrid_i(Fl_Check_Button*, void*) {
  RedrawWindow();
}
void IntersectionInterface::cb_m_bGrid(Fl_Check_Button* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_m_bGrid_i(o,v);
}

void IntersectionInterface::cb_m_bRay_i(Fl_Check_Button*, void*) {
  RedrawWindow();
}
void IntersectionInterface::cb_m_bRay(Fl_Check_Button* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_m_bRay_i(o,v);
}

void IntersectionInterface::cb_m_bRayShadow_i(Fl_Check_Button*, void*) {
  RedrawWindow();
}
void IntersectionInterface::cb_m_bRayShadow(Fl_Check_Button* o, void* v) {
  ((IntersectionInterface*)(o->parent()->user_data()))->cb_m_bRayShadow_i(o,v);
}

Fl_Double_Window* IntersectionInterface::make_window() {
  { m_intersectionWindow = new Fl_Double_Window(420, 265, "Intersection UI");
    m_intersectionWindow->user_data((void*)(this));
    { Fl_Group* o = new Fl_Group(5, 25, 145, 30);
      o->end();
    } // Fl_Group* o
    { m_iShapeType = new Fl_Choice(5, 25, 145, 30, "Object type");
      m_iShapeType->down_box(FL_BORDER_BOX);
      m_iShapeType->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      m_iShapeType->menu(menu_m_iShapeType);
    } // Fl_Choice* m_iShapeType
    { m_dXAt = new Fl_Value_Slider(5, 75, 200, 25, "At x pos");
      m_dXAt->type(5);
      m_dXAt->minimum(-1.5);
      m_dXAt->maximum(1.5);
      m_dXAt->callback((Fl_Callback*)cb_m_dXAt);
      m_dXAt->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Value_Slider* m_dXAt
    { m_dYAt = new Fl_Value_Slider(5, 115, 200, 25, "At y pos");
      m_dYAt->type(5);
      m_dYAt->minimum(-1.5);
      m_dYAt->maximum(1.5);
      m_dYAt->callback((Fl_Callback*)cb_m_dYAt);
      m_dYAt->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Value_Slider* m_dYAt
    { m_dZAt = new Fl_Value_Slider(5, 155, 200, 25, "At z pos");
      m_dZAt->type(5);
      m_dZAt->minimum(-1.5);
      m_dZAt->maximum(1.5);
      m_dZAt->callback((Fl_Callback*)cb_m_dZAt);
      m_dZAt->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Value_Slider* m_dZAt
    { m_dTheta = new Fl_Value_Slider(5, 195, 200, 25, "Vec theta");
      m_dTheta->type(5);
      m_dTheta->maximum(360);
      m_dTheta->step(1);
      m_dTheta->callback((Fl_Callback*)cb_m_dTheta);
      m_dTheta->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Value_Slider* m_dTheta
    { m_dPhi = new Fl_Value_Slider(5, 235, 200, 25, "Vec phi");
      m_dPhi->type(5);
      m_dPhi->minimum(-90);
      m_dPhi->maximum(90);
      m_dPhi->step(1);
      m_dPhi->value(45);
      m_dPhi->callback((Fl_Callback*)cb_m_dPhi);
      m_dPhi->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Value_Slider* m_dPhi
    { Fl_Button* o = new Fl_Button(330, 25, 85, 25, "Write test");
      o->callback((Fl_Callback*)cb_Write);
    } // Fl_Button* o
    { m_dXRot = new Fl_Value_Slider(215, 75, 200, 25, "View rotation");
      m_dXRot->type(5);
      m_dXRot->maximum(360);
      m_dXRot->step(1);
      m_dXRot->callback((Fl_Callback*)cb_m_dXRot);
      m_dXRot->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Value_Slider* m_dXRot
    { m_dYRot = new Fl_Value_Slider(215, 115, 200, 25, "View height");
      m_dYRot->type(5);
      m_dYRot->minimum(-90);
      m_dYRot->maximum(90);
      m_dYRot->step(1);
      m_dYRot->callback((Fl_Callback*)cb_m_dYRot);
      m_dYRot->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Value_Slider* m_dYRot
    { m_bGrid = new Fl_Check_Button(215, 155, 25, 25, "Show grid");
      m_bGrid->down_box(FL_DOWN_BOX);
      m_bGrid->value(1);
      m_bGrid->callback((Fl_Callback*)cb_m_bGrid);
    } // Fl_Check_Button* m_bGrid
    { m_bRay = new Fl_Check_Button(215, 195, 25, 25, "Show ray");
      m_bRay->down_box(FL_DOWN_BOX);
      m_bRay->value(1);
      m_bRay->callback((Fl_Callback*)cb_m_bRay);
    } // Fl_Check_Button* m_bRay
    { m_bRayShadow = new Fl_Check_Button(215, 235, 25, 25, "Show ray shadow");
      m_bRayShadow->down_box(FL_DOWN_BOX);
      m_bRayShadow->value(1);
      m_bRayShadow->callback((Fl_Callback*)cb_m_bRayShadow);
    } // Fl_Check_Button* m_bRayShadow
    { m_iSeed = new Fl_Value_Input(240, 30, 85, 20, "Seed");
    } // Fl_Value_Input* m_iSeed
    m_intersectionWindow->end();
    m_intersectionWindow->resizable(m_intersectionWindow);
  } // Fl_Double_Window* m_intersectionWindow
  return m_intersectionWindow;
}

IntersectionInterface::IntersectionInterface() {
  m_intersectionWindow = make_window();
  intersectionUI.setUI( this );
  intersectionUI.changeShape( ShapesUI::SHAPE_SPHERE );
}

UIInterface * IntersectionInterface::getUI() {
  return &intersectionUI;
}

double IntersectionInterface::getTheta() const {
  return M_PI * m_dTheta->value() / 180.0;
}

double IntersectionInterface::getPhi() const {
  return M_PI * m_dPhi->value() / 180.0;
}

double IntersectionInterface::getXRot() const {
  return M_PI * m_dXRot->value() / 180.0;
}

double IntersectionInterface::getYRot() const {
  return M_PI * m_dYRot->value() / 180.0;
}
