#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "glut.h"
#include "timer.h"
#include "basicterrain.h"
#include "skeleton.h"
#include "fkframer.h"
#include "ikframer.h"
#include "humanskeleton.h"
#include "humanwalkanimation.h"
#include "humanstairanimation.h"
#include "humanwalkframer.h"
#include "humanstairframer.h"
#include "humanskin.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#define DEG_TO_RAD 0.0174532925

// Distance of camera from actor
#define CAMERA_DISTANCE 10.0

// Number of stairs to walk up
#define NUM_STAIRS 0

// Initial y translation for the human skeleton, to place bottom of feet at y = 0.0     //以躯干下部为root原点。
#define INITIAL_ELEVATION 3.5     //原始值1.98

// Initial z translation for the human skeleton
#define INITIAL_POSITION  -1.0  //-2.6原点

// Actor velocity - rate of motion along z axis as units per second; can be adjusted with rate variable from keyboard (r/R keys)
#define ACTOR_VELOCITY 0.40

// Translation increment when translating x, y, z from keyboard input
#define TRANSLATION_INCREMENT 0.25

// Translation increment when translating x, y, z from keyboard input
#define ROTATION_INCREMENT 0.5

// Initial window size, this is used when window is created and opened
GLsizei wh = 500, ww = 500;

// Translations and rotations for the scene
GLdouble x = -7.5;
GLdouble y = 0.0;
GLdouble z = 12.25;
GLdouble xrot = 2.5;
GLdouble yrot = 251.5;
GLdouble zrot = 0.0;

// Animation rate adjustment  //移动的速率，增加快速移动
double rate = 1.0;

// Timer for calculating velocities
Timer timer;

// Time of last update
double lastTime = 0.0;

// Scene controls
bool animating  = false;
bool skinning  = true;
bool wireframe = false;

// Position of light in the scene
GLfloat LightPosition[]= { 15.0f, 25.0f, 15.0f, 1.0f };

// The ground on which to walk
BasicTerrain ground;

// Forward kinematic animation with Inverse kinematic foot correction for walking over the ground
HumanWalkAnimation walkanimator(&ground);

// Forward kinematic animation for walking up stairs; switches between skeletons with root at body and feet
HumanStairAnimation stairanimator;

// The skeleton
HumanSkeleton hskel;

// The skin attached to the skeleton
HumanSkin hskin;

// The forward kinematic animation for walking
HumanWalkFramer walkframer;

// The forward kinematic for walking up stairs
HumanStairFramer stairframer;

// Select between walking up stairs and general walking
bool walking = false;

// Position of actor for tracking with camera
Vector3 bodypos;

// Draw the scene
void mydisplay(void)
{
	//printf("mydisplay  start  \n");  实时描画
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  // Orient the scene view based on user input
  glRotated(xrot,1.0,0.0,0.0);
  glRotated(yrot,0.0,1.0,0.0);
  glRotated(zrot,0.0,0.0,1.0);

  glTranslated(x, y, z);

  // Set initial camera position to look at actor from the side
  gluLookAt(bodypos.getX(), bodypos.getY(), bodypos.getZ() + CAMERA_DISTANCE,
            bodypos.getX(), bodypos.getY(), bodypos.getZ(),
            0.0, 1.0, 0.0);

  glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

  // Draw the ground
  glColor4d(0.0, 1.0, 1.0, 1.0);
  ground.draw();

  // Draw the skeleton
  glColor3f(1.0, 1.0, 0.0);
  if(walking)
    walkanimator.draw();
  else
    stairanimator.draw();

  // Draw the skin
  if(skinning)
    hskin.draw();

  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

string intTranStr(double integer) {
	stringstream ss;
	ss << integer;
	return ss.str();
}

// Update the animation
void animate()
{
//	printf("animate  start \n");
  double t = timer.getTime();
  if(animating)
  {
    if(walking)
    {
      // Move the actor
      Vector3* pos = hskel.getRootArc()->getTranslation();
	  printf("---------------- \n");
	  printf("----------------animate  walking \n");
	  cout << "pos->getX():"+ intTranStr(pos->getX()) << endl;
	  cout << "pos->getY():" + intTranStr(pos->getY()) << endl;
	  cout << "pos->getZ():" + intTranStr(pos->getZ()) << endl;
	  printf("---------------- \n");
      if(pos->getZ() < 100.0)
      {
		printf("pos->getZ() < 10.0 \n");
        double newz = pos->getZ() + (ACTOR_VELOCITY * rate * (t - lastTime));
        double newy = INITIAL_ELEVATION + ground.getHeight(pos->getX(), newz);
        //hskel.getRootArc()->setTranslation(pos->getX(), newy, newz);
		hskel.getRootArc()->setTranslation(pos->getX(), pos->getY(), pos->getZ());

        // Update animation/correct foot elevetaion if ankle penetrates floor
        walkanimator.update();

        // Get current position for camera update
        bodypos = *hskel.getRootArc()->getTranslation();

        // Update the skin
        hskin.update(&hskel);
      }
    }
    else   //台阶行走处理
    {


      // Update stairwalk animation
      stairanimator.update();
      
      // Get current position for camera update
      Skeleton* currentSkel = (*stairanimator.begin())->skeleton_;
      Skeleton::ArcPositionVector_t positions;

      currentSkel->getArcPositions(positions);
      bodypos = positions["Body"];

	  printf("---------------- \n");
	  printf("----------------animate  stair \n");
	  cout << "bodypos.getX():" + intTranStr(bodypos.getX()) << endl;
	  cout << "bodypos.getY():" + intTranStr(bodypos.getY()) << endl;
	  cout << "bodypos.getZ():" + intTranStr(bodypos.getZ()) << endl;
	  printf("---------------- \n");

      // Update the skin
      hskin.update(currentSkel);

      // See if stair walking is complete  台阶完成条件。
      if(stairframer.getCycles() >= (NUM_STAIRS/2))
      {
		  printf("animate  walking else  -----if 111111111111111 \n");
        walking = true;
      }
    }
  }

  // Store current time for time since last frame calculation
  lastTime = t;

  glutPostRedisplay();
}

// Process keyboard input
void key(unsigned char k, int xx, int yy)
{
	printf("---key  stair \n");
  if (k=='q' || k=='Q' || k == 27)
  {
    // Quit when q or escape are pressed
    exit (0);
  }
  else if(k == '1')
  {
    // Set initial view (backward diagonal)
    x = -7.5;
    y = 0.0;
    z = 12.25;
    xrot = 2.5;
    yrot = 251.5;
    zrot = 0.0; 
  }
  else if(k == '2')
  {
    // Set view facing front of actor from a slight elevation
    x = 0.0;
    y = -2.25;
    z = 4.25;
    xrot = 25.0;
    yrot = 0.0;
    zrot = 0.0;
  }
  else if(k == '3')
  {
    // Set view looking from left side
    z = CAMERA_DISTANCE;
    x = -CAMERA_DISTANCE;
    y = 0.0;
    xrot = 0.0;
    yrot = -90.0;
    zrot = 0.0;
  }
  else if(k == '4')
  {
    // Set view looking from right side
    z = CAMERA_DISTANCE;
    x = CAMERA_DISTANCE;
    y = 0.0;
    xrot = 0.0;
    yrot = 90.0;
    zrot = 0.0;
  }
  else if(k == 'z')
    z += TRANSLATION_INCREMENT;
  else if(k == 'Z')
    z -= TRANSLATION_INCREMENT;
  else if(k == 'x')
    x += TRANSLATION_INCREMENT;
  else if(k == 'X')
    x -= TRANSLATION_INCREMENT;
  else if(k == 'y')
    y += TRANSLATION_INCREMENT;
  else if(k == 'Y')
    y -= TRANSLATION_INCREMENT;
  else if(k == 'r')
  {
    // Increase rate of animation
    rate += 0.1;
    Animation::actors_t::iterator iter;
    for(iter = walkanimator.begin(); iter != walkanimator.end(); ++iter)
    {
      double framerRate = (*iter)->framer_->getRate();
      (*iter)->framer_->setRate(framerRate + 0.1);
    }

    for(iter = stairanimator.begin(); iter != stairanimator.end(); ++iter)
    {
      double framerRate = (*iter)->framer_->getRate();
      (*iter)->framer_->setRate(framerRate + 0.1);
    }
  }
  else if(k == 'R')
  {
    // Decrease rate of animation
    rate -= 0.1;
    Animation::actors_t::iterator iter;
    for(iter = walkanimator.begin(); iter != walkanimator.end(); ++iter)
    {
      double framerRate = (*iter)->framer_->getRate();
      (*iter)->framer_->setRate(framerRate - 0.1);
    }

    for(iter = stairanimator.begin(); iter != stairanimator.end(); ++iter)
    {
      double framerRate = (*iter)->framer_->getRate();
      (*iter)->framer_->setRate(framerRate - 0.1);
    }
  }
  else if(k == 'w')
  {
    // Toggle wireframe view
    wireframe = !wireframe;
    if(wireframe)
    {
      glPolygonMode(GL_FRONT, GL_LINE);
      glPolygonMode(GL_BACK, GL_LINE);
    }
    else
    {
      glPolygonMode(GL_FRONT, GL_FILL);
      glPolygonMode(GL_BACK, GL_FILL);
    }
  }
  else if(k == 'a')
    animating = !animating;
  else if(k == 's')
    skinning = !skinning;
  else if(k == GLUT_KEY_UP || k == 'i')
    xrot -= ROTATION_INCREMENT;
  else if(k == GLUT_KEY_DOWN || k == 'k')
    xrot += ROTATION_INCREMENT;
  else if(k == GLUT_KEY_RIGHT || k == 'j')
    yrot -= ROTATION_INCREMENT;
  else if(k == GLUT_KEY_LEFT || k == 'l')
    yrot += ROTATION_INCREMENT;
  else if(k == '<' || k == ',')
    zrot -= ROTATION_INCREMENT;
  else if(k == '<' || k == '.')
    zrot += ROTATION_INCREMENT;
}


// Reshaping routine called whenever window is resized or moved
void myreshape(GLsizei w, GLsizei h)
{
	printf("myreshape  start  \n");
  glViewport(0,0,w,h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0f,(GLfloat)w/(GLfloat)h,0.1f,100000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Set global size
  ww = w;
  wh = h;
}

// Set initial OpenGL configuration
void myinit(void)
{
	printf("---myinit  stair \n");
  GLfloat LightAmbient[]= { 0.1f, 0.1f, 0.1f, 1.0f };
  GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //修改整個面板背景顏色

  glShadeModel(GL_SMOOTH);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_FILL);

  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  // Initialize time
  lastTime = timer.getTime();
}

// Print usage instructions to console
void print_usage()
{
// Print usage instructions to console
  printf("Usage\n");
  printf("\tq\t\tQuit\n");
  printf("\t1\t\tSelect diagonal view (initial view)\n");
  printf("\t2\t\tSelect view from front of actor\n");
  printf("\t3\t\tSelect view from left side of actor\n");
  printf("\t4\t\tSelect view from right side of actor\n");
  printf("\tx/X\t\tMove camera along x axis\n");
  printf("\ty/Y\t\tMove camera along y axis\n");
  printf("\tz/Z\t\tMove camera along z axis (Zoom in/out)\n");
  printf("\ti/k\t\tRotate canera around x-axis\n");
  printf("\tj/l\t\tRotate camera around y-axis\n");
  printf("\t</>\t\tRotate camera around z-axis\n");
  printf("\ta\t\tToggle animation\n");
  printf("\ts\t\tToggle skin\n");
  printf("\tw\t\tToggle wireframe\n");
  printf("\tr/R\t\tIncrease/decrease animation speed\n");
}

string toString(double d) {
	stringstream ss;
	// 默认浮点数只输出6位有效数字，这里改成小数点后6位
	ss.setf(ios::fixed);
	return ss.str();
}

int main(int argc, char** argv)
{
  // Print instructions for use to console

  print_usage();
  printf("main  start \n");
  // Initialize glut
  glutInit(&argc,argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_STENCIL | GLUT_DEPTH);
  glutInitWindowSize(ww,wh);
  glutCreateWindow("Assignment 2");

  // Initialize OpenGL settings
  myinit();

  // Initialize the skeleton
  hskel.create();

  // Set the initial position for the skeleton
  bodypos.set(0.0, INITIAL_ELEVATION - (0.39 * NUM_STAIRS), INITIAL_POSITION - (0.54 * NUM_STAIRS));
  hskel.getRootArc()->setTranslation(bodypos);

  // Initialize the skin
  hskin.create();
  hskin.update(&hskel);

  // Set the rate for the forward kinematic solver
  walkframer.setRate(rate);
  stairframer.setRate(rate);

  // Add the skeleton and forward kinematic solver to the animators
  walkanimator.addActor(&hskel, &walkframer);
  stairanimator.addActor(&hskel, &stairframer);

  // Setup glut routines
  glutReshapeFunc (myreshape);
  glutDisplayFunc(mydisplay);
  printf("main  animate  \n");
  glutIdleFunc(animate);
  glutKeyboardFunc(key);
  glutMainLoop();

  return 0;
}
