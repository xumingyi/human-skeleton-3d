#include "shapebone.h"
#include "shapecube.h"
#include "bonecluster.h"
#include "humanstairframer.h"

// Specialized skeleton with right foot as root
void HumanSkeletonRFoot::buildSkeleton()
{
  ShapeBone* bone;
  ShapeCube* empty;

  // Create right foot
  bone = new ShapeBone(ShapeBone::HORIZONTALZ, 0.3);
  bone->setTranslation(0.0, 0.0, 0.15 - ShapeBone::BONEWIDTH_2);
  SkelNode* rfoot = new SkelNode("RFoot", bone);
  SkelArc* root = new SkelArc("Root", rfoot);
  setRootArc(root);


  // Create lower right leg
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.6);
  bone->setTranslation(0.0, 0.3, 0.0);
  SkelNode* rlowerleg = new SkelNode("RLowerLeg", bone);
  SkelArc* rankle = new SkelArc("RAnkle", rlowerleg);
  rankle->setTranslation(0.0, ShapeBone::BONEWIDTH_2, 0.0);
  rankle->setRotationAxis(1.0, 0.0, 0.0);
  rfoot->addArc(rankle);


  // Create upper right leg - three arcs for 3DOF with 2 zero length nodes
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.8);
  bone->setTranslation(0.0, 0.4, 0.0);
  SkelNode* rupperlegz = new SkelNode("RUpperLegZ", bone);
  SkelArc* rknee = new SkelArc("RKnee", rupperlegz);
  rknee->setTranslation(0.0, 0.6, 0.0);
  rknee->setRotationAxis(1.0, 0.0, 0.0);
  rlowerleg->addArc(rknee);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* rupperlegy = new SkelNode("RUpperLegY", empty);
  SkelArc* rhipz = new SkelArc("RHipZ", rupperlegy);
  rhipz->setTranslation(0.0, 0.8, 0.0);
  rhipz->setRotationAxis(0.0, 0.0, 1.0);
  rupperlegz->addArc(rhipz);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* rupperlegx = new SkelNode("RUpperLegX", empty);
  SkelArc* rhipy = new SkelArc("RHipY", rupperlegx);
  rhipy->setRotationAxis(0.0, 1.0, 0.0);
  rupperlegy->addArc(rhipy);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* rupperleg = new SkelNode("RUpperLeg", empty);
  SkelArc* rhipx = new SkelArc("RHipX", rupperleg);
  rhipx->setRotationAxis(1.0, 0.0, 0.0);
  rupperlegx->addArc(rhipx);

  // Create the torso - a bone cluster
  BoneCluster* bones = new BoneCluster();
  bones->addBone(ShapeBone::HORIZONTALX, 0.6, Vector3(0.0, 0.5 - ShapeBone::BONEWIDTH_2, 0.0));
  bones->addBone(ShapeBone::VERTICAL, 1.0);
  bones->addBone(ShapeBone::HORIZONTALX, 0.3, Vector3(0.0, -0.5 + ShapeBone::BONEWIDTH_2, 0.0));

  SkelNode* torso = new SkelNode("Torso", bones);
  SkelArc* body = new SkelArc("Body", torso);
  body->setTranslation(0.15, 0.5, 0.0);
  rupperleg->addArc(body);


  // Create the neck
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.1);
  bone->setTranslation(0.0, 0.05, 0.0);                         // Put bottom of bone at origin

  SkelNode* neck = new SkelNode("Neck", bone);
  SkelArc* neckBase = new SkelArc("NeckBase", neck);
  neckBase->setTranslation(0.0, 0.5, 0.0);
  neckBase->setRotationAxis(0.0, 1.0, 0.0);
  torso->addArc(neckBase);


  // Create the head
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.4);
  bone->setTranslation(0.0, 0.2, 0.0);                          // Put bottom of bone at origin

  SkelNode* head = new SkelNode("Head", bone);
  SkelArc* headBase = new SkelArc("HeadBase", head);
  headBase->setTranslation(0.0, 0.05, 0.0);
  headBase->setRotationAxis(-1.0, 0.0, 0.0);
  neck->addArc(headBase);

  /* xmy×¢ÊÍµô¸ì²²
  // Create upper right arm - three arcs for 3DOF with 2 zero length nodes
  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* rupperarmx = new SkelNode("RupperArmX", empty);
  SkelArc* rshoulderx = new SkelArc("RShoulderX", rupperarmx);
  rshoulderx->setTranslation(-0.3, 0.5, 0.0);
  rshoulderx->setRotationAxis(-1.0, 0.0, 0.0);
  torso->addArc(rshoulderx);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* rupperarmy = new SkelNode("RupperArmY", empty);
  SkelArc* rshouldery = new SkelArc("RShoulderY", rupperarmy);
  rshouldery->setRotationAxis(0.0, -1.0, 0.0);
  rupperarmx->addArc(rshouldery);

  bone = new ShapeBone(ShapeBone::VERTICAL, 0.5);
  bone->setTranslation(0.0, -0.25, 0.0);                       // Put top of bone at origin
  SkelNode* rupperarmz = new SkelNode("RUpperArmZ", bone);
  SkelArc* rshoulderz = new SkelArc("RShoulderZ", rupperarmz);
  rshoulderz->setRotationAxis(0.0, 0.0, -1.0);
  rupperarmy->addArc(rshoulderz);


  // Create lower right arm
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.6);
  bone->setTranslation(0.0, -0.3, 0.0);                       // Put top of bone at origin
  SkelNode* rlowerarm = new SkelNode("RLowerArm", bone);
  SkelArc* relbow = new SkelArc("RElbow", rlowerarm);
  relbow->setTranslation(0.0, -0.5, 0.0);
  relbow->setRotationAxis(-1.0, 0.0, 0.0);
  rupperarmz->addArc(relbow);


  // Create upper left arm - three arcs for 3DOF with 2 zero length nodes
  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* lupperarmx = new SkelNode("LupperArmX", empty);
  SkelArc* lshoulderx = new SkelArc("LShoulderX", lupperarmx);
  lshoulderx->setTranslation(0.3, 0.5, 0.0);
  lshoulderx->setRotationAxis(-1.0, 0.0, 0.0);
  torso->addArc(lshoulderx);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* lupperarmy = new SkelNode("LupperArmY", empty);
  SkelArc* lshouldery = new SkelArc("LShoulderY", lupperarmy);
  lshouldery->setRotationAxis(0.0, -1.0, 0.0);
  lupperarmx->addArc(lshouldery);

  bone = new ShapeBone(ShapeBone::VERTICAL, 0.5);
  bone->setTranslation(0.0, -0.25, 0.0);                       // Put top of bone at origin
  SkelNode* lupperarmz = new SkelNode("LUpperArmZ", bone);
  SkelArc* lshoulderz = new SkelArc("LShoulderZ", lupperarmz);
  lshoulderz->setRotationAxis(0.0, 0.0, -1.0);
  lupperarmy->addArc(lshoulderz);


  // Create lower left arm
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.6);
  bone->setTranslation(0.0, -0.3, 0.0);                       // Put top of bone at origin
  SkelNode* llowerarm = new SkelNode("LLowerArm", bone);
  SkelArc* lelbow = new SkelArc("LElbow", llowerarm);
  lelbow->setTranslation(0.0, -0.5, 0.0);
  lelbow->setRotationAxis(-1.0, 0.0, 0.0);
  lupperarmz->addArc(lelbow);
xmy×¢ÊÍµô¸ì²²½áÊø */

  // Create upper left leg - three arcs for 3DOF with 2 zero length nodes
  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* lupperlegx = new SkelNode("LUpperLegX", empty);
  SkelArc* lhipx = new SkelArc("LHipX", lupperlegx);
  lhipx->setTranslation(0.15, -0.5, 0.0);
  lhipx->setRotationAxis(-1.0, 0.0, 0.0);
  torso->addArc(lhipx);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* lupperlegy = new SkelNode("LUpperLegY", empty);
  SkelArc* lhipy = new SkelArc("LHipY", lupperlegy);
  lhipy->setRotationAxis(0.0, -1.0, 0.0);
  lupperlegx->addArc(lhipy);

  bone = new ShapeBone(ShapeBone::VERTICAL, 0.8);
  bone->setTranslation(0.0, -0.4, 0.0);                             // Put top of bone at origin
  SkelNode* lupperlegz = new SkelNode("LUpperLegZ", bone);
  SkelArc* lhipz = new SkelArc("LHipZ", lupperlegz);
  lhipz->setRotationAxis(0.0, 0.0, -1.0);
  lupperlegy->addArc(lhipz);


  // Create lower left leg
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.6);
  bone->setTranslation(0.0, -0.3, 0.0);                             // Put top of bone at origin
  SkelNode* llowerleg = new SkelNode("LLowerLeg", bone);
  SkelArc* lknee = new SkelArc("LKnee", llowerleg);
  lknee->setTranslation(0.0, -0.8, 0.0);
  lknee->setRotationAxis(-1.0, 0.0, 0.0);
  lupperlegz->addArc(lknee);

  // Create left foot
  bone = new ShapeBone(ShapeBone::HORIZONTALZ, 0.3);
  bone->setTranslation(0.0, -ShapeBone::BONEWIDTH_2, 0.15 - ShapeBone::BONEWIDTH_2);
  SkelNode* lfoot = new SkelNode("LFoot", bone);
  SkelArc* lankle = new SkelArc("LAnkle", lfoot);
  lankle->setTranslation(0.0, -0.6, 0.0);
  lankle->setRotationAxis(-1.0, 0.0, 0.0);
  llowerleg->addArc(lankle);
}

void HumanSkeletonRFoot::arcTransforms(const SkelArc* arc, const Matrix44& G, ArcTransformVector_t& transforms)
{
  SkelNode* node = arc->getNode();
  Vector3 p;
  Matrix44 t, r;
  Matrix44 L = G;

  t.SetTranslation(arc->getTranslation()->getX(), arc->getTranslation()->getY(), arc->getTranslation()->getZ());

  // The knees need a zero value for the angle - the skin should probably use the ankle angle for foot as root
  std::string name = arc->getName();
  if(name == "RKnee")
  {
    r.SetRotation(0, Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));
    t *= r;

    Matrix44 M = G;
    M *= t;

    transforms[arc->getName()] = M;

    t.SetTranslation(arc->getTranslation()->getX(), arc->getTranslation()->getY(), arc->getTranslation()->getZ());
    r.SetRotation(arc->getRotationAngle()->getValue(), Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));

    t *= r;
    L *= t;
  }
  else
  {
    r.SetRotation(arc->getRotationAngle()->getValue(), Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));

    t *= r;
    L *= t;

    transforms[arc->getName()] = L;
  }

  //Process the children
  skelarcs_t::const_iterator i;
  for(i = node->begin(); i != node->end(); ++i)
  {
    arcTransforms(*i, L, transforms);
  }
}

// Specialized skeleton with left foot as root
void HumanSkeletonLFoot::buildSkeleton()
{
  ShapeBone* bone;
  ShapeCube* empty;

  // Create left foot
  bone = new ShapeBone(ShapeBone::HORIZONTALZ, 0.3);
  bone->setTranslation(0.0, 0.0, 0.15 - ShapeBone::BONEWIDTH_2);
  SkelNode* lfoot = new SkelNode("LFoot", bone);
  SkelArc* root = new SkelArc("Root", lfoot);
  setRootArc(root);


  // Create lower right leg
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.6);
  bone->setTranslation(0.0, 0.3, 0.0);
  SkelNode* llowerleg = new SkelNode("LLowerLeg", bone);
  SkelArc* lankle = new SkelArc("LAnkle", llowerleg);
  lankle->setTranslation(0.0, ShapeBone::BONEWIDTH_2, 0.0);
  lankle->setRotationAxis(1.0, 0.0, 0.0);
  lfoot->addArc(lankle);


  // Create upper right leg - three arcs for 3DOF with 2 zero length nodes
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.8);
  bone->setTranslation(0.0, 0.4, 0.0);
  SkelNode* lupperlegz = new SkelNode("LUpperLegZ", bone);
  SkelArc* lknee = new SkelArc("LKnee", lupperlegz);
  lknee->setTranslation(0.0, 0.6, 0.0);
  lknee->setRotationAxis(1.0, 0.0, 0.0);
  llowerleg->addArc(lknee);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* lupperlegy = new SkelNode("LUpperLegY", empty);
  SkelArc* lhipz = new SkelArc("LHipZ", lupperlegy);
  lhipz->setTranslation(0.0, 0.8, 0.0);
  lhipz->setRotationAxis(0.0, 0.0, 1.0);
  lupperlegz->addArc(lhipz);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* lupperlegx = new SkelNode("LUpperLegX", empty);
  SkelArc* lhipy = new SkelArc("LHipY", lupperlegx);
  lhipy->setRotationAxis(0.0, 1.0, 0.0);
  lupperlegy->addArc(lhipy);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* lupperleg = new SkelNode("LUpperLeg", empty);
  SkelArc* lhipx = new SkelArc("LHipX", lupperleg);
  lhipx->setRotationAxis(1.0, 0.0, 0.0);
  lupperlegx->addArc(lhipx);

  // Create the torso - a bone cluster
  BoneCluster* bones = new BoneCluster();
  bones->addBone(ShapeBone::HORIZONTALX, 0.6, Vector3(0.0, 0.5 - ShapeBone::BONEWIDTH_2, 0.0));
  bones->addBone(ShapeBone::VERTICAL, 1.0);
  bones->addBone(ShapeBone::HORIZONTALX, 0.3, Vector3(0.0, -0.5 + ShapeBone::BONEWIDTH_2, 0.0));

  SkelNode* torso = new SkelNode("Torso", bones);
  SkelArc* body = new SkelArc("Body", torso);
  body->setTranslation(-0.15, 0.5, 0.0);
  lupperleg->addArc(body);


  // Create the neck
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.1);
  bone->setTranslation(0.0, 0.05, 0.0);                         // Put bottom of bone at origin

  SkelNode* neck = new SkelNode("Neck", bone);
  SkelArc* neckBase = new SkelArc("NeckBase", neck);
  neckBase->setTranslation(0.0, 0.5, 0.0);
  neckBase->setRotationAxis(0.0, 1.0, 0.0);
  torso->addArc(neckBase);


  // Create the head
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.4);
  bone->setTranslation(0.0, 0.2, 0.0);                          // Put bottom of bone at origin

  SkelNode* head = new SkelNode("Head", bone);
  SkelArc* headBase = new SkelArc("HeadBase", head);
  headBase->setTranslation(0.0, 0.05, 0.0);
  headBase->setRotationAxis(-1.0, 0.0, 0.0);
  neck->addArc(headBase);

  /* xmy×¢ÊÍµô¸ì²²
  // Create upper right arm - three arcs for 3DOF with 2 zero length nodes
  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* rupperarmx = new SkelNode("RupperArmX", empty);
  SkelArc* rshoulderx = new SkelArc("RShoulderX", rupperarmx);
  rshoulderx->setTranslation(-0.3, 0.5, 0.0);
  rshoulderx->setRotationAxis(-1.0, 0.0, 0.0);
  torso->addArc(rshoulderx);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* rupperarmy = new SkelNode("RupperArmY", empty);
  SkelArc* rshouldery = new SkelArc("RShoulderY", rupperarmy);
  rshouldery->setRotationAxis(0.0, -1.0, 0.0);
  rupperarmx->addArc(rshouldery);

  bone = new ShapeBone(ShapeBone::VERTICAL, 0.5);
  bone->setTranslation(0.0, -0.25, 0.0);                       // Put top of bone at origin
  SkelNode* rupperarmz = new SkelNode("RUpperArmZ", bone);
  SkelArc* rshoulderz = new SkelArc("RShoulderZ", rupperarmz);
  rshoulderz->setRotationAxis(0.0, 0.0, -1.0);
  rupperarmy->addArc(rshoulderz);


  // Create lower right arm
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.6);
  bone->setTranslation(0.0, -0.3, 0.0);                       // Put top of bone at origin
  SkelNode* rlowerarm = new SkelNode("RLowerArm", bone);
  SkelArc* relbow = new SkelArc("RElbow", rlowerarm);
  relbow->setTranslation(0.0, -0.5, 0.0);
  relbow->setRotationAxis(-1.0, 0.0, 0.0);
  rupperarmz->addArc(relbow);


  // Create upper left arm - three arcs for 3DOF with 2 zero length nodes
  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* lupperarmx = new SkelNode("LupperArmX", empty);
  SkelArc* lshoulderx = new SkelArc("LShoulderX", lupperarmx);
  lshoulderx->setTranslation(0.3, 0.5, 0.0);
  lshoulderx->setRotationAxis(-1.0, 0.0, 0.0);
  torso->addArc(lshoulderx);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* lupperarmy = new SkelNode("LupperArmY", empty);
  SkelArc* lshouldery = new SkelArc("LShoulderY", lupperarmy);
  lshouldery->setRotationAxis(0.0, -1.0, 0.0);
  lupperarmx->addArc(lshouldery);

  bone = new ShapeBone(ShapeBone::VERTICAL, 0.5);
  bone->setTranslation(0.0, -0.25, 0.0);                       // Put top of bone at origin
  SkelNode* lupperarmz = new SkelNode("LUpperArmZ", bone);
  SkelArc* lshoulderz = new SkelArc("LShoulderZ", lupperarmz);
  lshoulderz->setRotationAxis(0.0, 0.0, -1.0);
  lupperarmy->addArc(lshoulderz);


  // Create lower left arm
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.6);
  bone->setTranslation(0.0, -0.3, 0.0);                       // Put top of bone at origin
  SkelNode* llowerarm = new SkelNode("LLowerArm", bone);
  SkelArc* lelbow = new SkelArc("LElbow", llowerarm);
  lelbow->setTranslation(0.0, -0.5, 0.0);
  lelbow->setRotationAxis(-1.0, 0.0, 0.0);
  lupperarmz->addArc(lelbow);
xmy×¢ÊÍµô¸ì²²½áÊø */

  // Create upper right leg - three arcs for 3DOF with 2 zero length nodes
  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* rupperlegx = new SkelNode("RUpperLegX", empty);
  SkelArc* rhipx = new SkelArc("RHipX", rupperlegx);
  rhipx->setTranslation(-0.15, -0.5, 0.0);
  rhipx->setRotationAxis(-1.0, 0.0, 0.0);
  torso->addArc(rhipx);

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* rupperlegy = new SkelNode("RUpperLegY", empty);
  SkelArc* rhipy = new SkelArc("RHipY", rupperlegy);
  rhipy->setRotationAxis(0.0, -1.0, 0.0);
  rupperlegx->addArc(rhipy);

  bone = new ShapeBone(ShapeBone::VERTICAL, 0.8);
  bone->setTranslation(0.0, -0.4, 0.0);
  SkelNode* rupperlegz = new SkelNode("RUpperLegZ", bone);
  SkelArc* rhipz = new SkelArc("RHipZ", rupperlegz);
  rhipz->setRotationAxis(0.0, 0.0, -1.0);
  rupperlegy->addArc(rhipz);


  // Create lower right leg
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.6);
  bone->setTranslation(0.0, -0.3, 0.0);
  SkelNode* rlowerleg = new SkelNode("RLowerLeg", bone);
  SkelArc* rknee = new SkelArc("RKnee", rlowerleg);
  rknee->setTranslation(0.0, -0.8, 0.0);
  rknee->setRotationAxis(-1.0, 0.0, 0.0);
  rupperlegz->addArc(rknee);


  // Create right foot
  bone = new ShapeBone(ShapeBone::HORIZONTALZ, 0.3);
  bone->setTranslation(0.0, -ShapeBone::BONEWIDTH_2, 0.15 - ShapeBone::BONEWIDTH_2);
  SkelNode* rfoot = new SkelNode("RFoot", bone);
  SkelArc* rankle = new SkelArc("RAnkle", rfoot);
  rankle->setTranslation(0.0, -0.6, 0.0);
  rankle->setRotationAxis(-1.0, 0.0, 0.0);
  rlowerleg->addArc(rankle);
}

void HumanSkeletonLFoot::arcTransforms(const SkelArc* arc, const Matrix44& G, ArcTransformVector_t& transforms)
{
  SkelNode* node = arc->getNode();
  Vector3 p;
  Matrix44 t, r;
  Matrix44 L = G;

  t.SetTranslation(arc->getTranslation()->getX(), arc->getTranslation()->getY(), arc->getTranslation()->getZ());

  // The knees need a zero value for the angle - the skin should probably use the ankle angle for foot as root
  std::string name = arc->getName();
  if(name == "LKnee")
  {
    r.SetRotation(0, Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));
    t *= r;

    Matrix44 M = G;
    M *= t;

    transforms[arc->getName()] = M;

    t.SetTranslation(arc->getTranslation()->getX(), arc->getTranslation()->getY(), arc->getTranslation()->getZ());
    r.SetRotation(arc->getRotationAngle()->getValue(), Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));

    t *= r;
    L *= t;
  }
  else
  {
    r.SetRotation(arc->getRotationAngle()->getValue(), Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));

    t *= r;
    L *= t;

    transforms[arc->getName()] = L;
  }

  //Process the children
  skelarcs_t::const_iterator i;
  for(i = node->begin(); i != node->end(); ++i)
  {
    arcTransforms(*i, L, transforms);
  }
}

HumanStairFramer::HumanStairFramer()
{
  setupFrames();
}

#define FRAMEDURATION 1.0
void HumanStairFramer::setupFrames()
{
  // This animation has 4 evenly spaced frames to lift the leg, step to stair, and then step up
  FKFrame frame1;
  frame1.setDuration(2.0*FRAMEDURATION/3.0);
  frame1.addAngle("RHipX", 10.0);
  frame1.addAngle("RKnee", -10.0);
  frame1.addAngle("RAnkle", 0.0);
  frame1.addAngle("LHipX", 10.0);
  frame1.addAngle("LKnee", -10.0);
  frame1.addAngle("LAnkle", 0.0);
  frame1.addAngle("RShoulderX", 0.0);
  frame1.addAngle("RElbow", 0.0);
  frame1.addAngle("LShoulderX", 0.0);
  frame1.addAngle("LElbow", 0.0);
  frames_.push_back(frame1);

  /*
  FKFrame frame2;
  frame2.setDuration(2.0*FRAMEDURATION/3.0);
  frame2.addAngle("RHipX", 45.0);
  frame2.addAngle("RKnee", -100.0);
  frame2.addAngle("RAnkle", 45.0);

  frame2.addAngle("LHipX", 10.0);
  frame2.addAngle("LKnee", -10.0);
  frame2.addAngle("LAnkle", 0.0);

  frame2.addAngle("RShoulderX", 10.0);
  frame2.addAngle("RElbow", 15.0);

  frame2.addAngle("LShoulderX", -10.0);
  frame2.addAngle("LElbow", 0.0);
  frames_.push_back(frame2);

  FKFrame frame3;
  frame3.setDuration(FRAMEDURATION);
  frame3.addAngle("RHipX", 60.0);
  frame3.addAngle("RKnee", -60.0);
  frame3.addAngle("RAnkle", 1.0);

  frame3.addAngle("LHipX", 10.0);
  frame3.addAngle("LKnee", -10.0);
  frame3.addAngle("LAnkle", 0.0);

  frame3.addAngle("RShoulderX", 20.0);
  frame3.addAngle("RElbow", 30.0);

  frame3.addAngle("LShoulderX", -15.0);
  frame3.addAngle("LElbow", 15.0);
  frames_.push_back(frame3);

  FKFrame frame4;
  frame4.setDuration(FRAMEDURATION/3.0);
  frame4.addAngle("RHipX", 10.0);
  frame4.addAngle("RKnee", -10.0);
  frame4.addAngle("RAnkle", 0.0);
  frame4.addAngle("LHipX", 10.0);
  frame4.addAngle("LKnee", -45.0);
  frame4.addAngle("LAnkle", 30.0);
  frame4.addAngle("RShoulderX", 0.0);
  frame4.addAngle("RElbow", 0.0);
  frame4.addAngle("LShoulderX", 0.0);
  frame4.addAngle("LElbow", 0.0);
  frames_.push_back(frame4);

  // Adding frame 1 to end for cyclic animation
  frames_.push_back(frame1);

  FKFrame frame6;
  frame6.setDuration(2.0*FRAMEDURATION/3.0);
  frame6.addAngle("LHipX", 45.0);
  frame6.addAngle("LKnee", -100.0);
  frame6.addAngle("LAnkle", 45.0);

  frame6.addAngle("RHipX", 10.0);
  frame6.addAngle("RKnee", -10.0);
  frame6.addAngle("RAnkle", 0.0);

  frame6.addAngle("LShoulderX", 10.0);
  frame6.addAngle("LElbow", 15.0);

  frame6.addAngle("RShoulderX", -10.0);
  frame6.addAngle("RElbow", 0.0);
  frames_.push_back(frame6);

  FKFrame frame7;
  frame7.setDuration(FRAMEDURATION);
  frame7.addAngle("LHipX", 60.0);
  frame7.addAngle("LKnee", -60.0);
  frame7.addAngle("LAnkle", 1.0);

  frame7.addAngle("RHipX", 10.0);
  frame7.addAngle("RKnee", -10.0);
  frame7.addAngle("RAnkle", 0.0);

  frame7.addAngle("LShoulderX", 20.0);
  frame7.addAngle("LElbow", 30.0);

  frame7.addAngle("RShoulderX", -15.0);
  frame7.addAngle("RElbow", 15.0);
  frames_.push_back(frame7);

  FKFrame frame8;
  frame8.setDuration(FRAMEDURATION/3.0);
  frame8.addAngle("RHipX", 10.0);
  frame8.addAngle("RKnee", -45.0);
  frame8.addAngle("RAnkle", 30.0);
  frame8.addAngle("LHipX", 10.0);
  frame8.addAngle("LKnee", -10.0);
  frame8.addAngle("LAnkle", 0.0);
  frame8.addAngle("RShoulderX", 0.0);
  frame8.addAngle("RElbow", 0.0);
  frame8.addAngle("LShoulderX", 0.0);
  frame8.addAngle("LElbow", 0.0);
  frames_.push_back(frame8);

  // Adding frame 1 to end for cyclic animation
  */
  frames_.push_back(frame1);
  
  totalFrames_ = 1;
}
