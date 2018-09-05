//基础生成地形类
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <math.h>
#include "shape.h"
#include "shapecube.h"
#include "shaperhomboid.h"
#include "basicterrain.h"

#define RAD_TO_DEG 57.2957795

#define NUM_STAIRS 5

BasicTerrain::BasicTerrain()
{
  ShapeCube* flat;
  ShapeRhomboid* incline;

  // Stairs
  for(int i = 0; i <= NUM_STAIRS; i++)
  {
    flat = new ShapeCube();
    flat->setScale(4.0, 0.5, 0.54);
    flat->setTranslation(0.0, -0.25 + (-0.39 * i), -2.23 + (-0.54 * i));
    shapes_.push_back(flat);
    path_.push_back(new Line2D(-2.5 + (-0.54 * i), (-0.39 * i), -1.96 + (-0.54 * i), (-0.39 * i)));
  }

  // First flat segment
  flat = new ShapeCube();
  flat->setScale(4.0, 0.1, 5.0 - 0.54);
  flat->setTranslation(0.0, -0.05, 0.27);
  shapes_.push_back(flat);
  path_.push_back(new Line2D(-1.96, 0.0, 2.5, 0.0));

  // Incline segment
  incline = new ShapeRhomboid();
  incline->setScale(4.0, 0.1, 3.0);
  incline->setTranslation(0.0, 0.0, 2.5 + 1.5);
  shapes_.push_back(incline);
  path_.push_back(new Line2D(2.5, 0.0, 5.5, 1.0));

  // Second flat segment
  flat = new ShapeCube();
  flat->setScale(4.0, 0.1, 5.0);
  flat->setTranslation(0.0, 0.95, 2.5 + 3.0 + 2.5);
  shapes_.push_back(flat);
  path_.push_back(new Line2D(5.5, 1.0, 10.5, 1.0));
}

BasicTerrain::~BasicTerrain()
{
  ShapeList_t::iterator sli;
  for(sli = shapes_.begin(); sli != shapes_.end(); ++sli)
    delete *sli;

  LineList_t::iterator pli;
  for(pli = path_.begin(); pli != path_.end(); ++pli)
    delete *pli;
}

double BasicTerrain::getHeight(double x, double z)
{
  // Find line segment that contains current z position
  LineList_t::iterator pli;
  for(pli = path_.begin(); pli != path_.end(); ++pli)
  {
    if(z <= (*pli)->x2_)
      break;
  }

  if(pli != path_.end())
  {
    // Get the slope
    Line2D* l = *pli;
    double m = (l->y2_ - l->y1_) / (l->x2_ - l->x1_);
    return (m * (z - l->x1_)) + l->y1_;
  }

  return 0.0;
}

double BasicTerrain::getAngle(double x, double z)
{
  // Find line segment that contains current z position
  LineList_t::iterator pli;
  for(pli = path_.begin(); pli != path_.end(); ++pli)
  {
    if(z <= (*pli)->x2_)
      break;
  }

  if(pli != path_.end())
  {
    // Get the angle
    Line2D* l = *pli;
    double xx = (l->x2_ - l->x1_);
    double yy = (l->y2_ - l->y1_);
    double zz = sqrt((xx * xx) + (yy * yy));
    return acos((xx < 0.0) ? -xx : xx / zz) * RAD_TO_DEG;
  }

  return 0.0;
}

void BasicTerrain::draw()
{
  glPushMatrix();

  glTranslated(trans_.getX(), trans_.getY(), trans_.getZ());
  glRotated(rotAngle_, rotAxis_.getX(), rotAxis_.getY(), rotAxis_.getZ());
  glScaled(scale_.getX(), scale_.getY(), scale_.getZ());

  ShapeList_t::iterator sli;
  for(sli = shapes_.begin(); sli != shapes_.end(); ++sli)
    (*sli)->draw();

  glPopMatrix();
}
