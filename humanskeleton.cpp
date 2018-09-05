//人物躯干生成

#include <map>
#include <string>
#include "names.h"
#include "shapecube.h"
#include "bonecluster.h"
#include "humanskeleton.h"

#define UP_lEG_LENGTH 1.6         //大腿的长度
#define DOWN_lEG_LENGTH 1.2       //小腿的长度
#define FOOT_LENGTH 0.3           //脚的长度

HumanSkeleton::HumanSkeleton()
: Skeleton(NULL)
{
}

HumanSkeleton::~HumanSkeleton()
{
  destroySkeleton();
}

void HumanSkeleton::create()
{
  buildSkeleton();
  initializeAngles();
}

void HumanSkeleton::initializeAngles()
{
  AngleInitializer ai;
  ai.angles.insert(std::make_pair(ROOT, SkelDOF(0.0, 0.0, 0.0)));
  ai.angles.insert(std::make_pair(NECKBASE, SkelDOF(0.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(HEADBASE, SkelDOF(0.0, -45.0, 45.0)));
  ai.angles.insert(std::make_pair(RSHOULDERZ, SkelDOF(0.0, 0.0, 90.0)));
  ai.angles.insert(std::make_pair(RSHOULDERY, SkelDOF(0.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(RSHOULDERX, SkelDOF(0.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(RELBOW, SkelDOF(0.0, 0.0, 90.0)));
  ai.angles.insert(std::make_pair(LSHOULDERZ, SkelDOF(0.0, 0.0, 90.0)));
  ai.angles.insert(std::make_pair(LSHOULDERY, SkelDOF(0.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(LSHOULDERX, SkelDOF(0.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(LELBOW, SkelDOF(0.0, 0.0, 90.0)));
  ai.angles.insert(std::make_pair(RHIPZ, SkelDOF(0.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(RHIPY, SkelDOF(0.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(RHIPX, SkelDOF(10.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(RKNEE, SkelDOF(-10.0, -120.0, -10.0)));
  ai.angles.insert(std::make_pair(RANKLE, SkelDOF(0.0, -45.0, 45.0)));
  ai.angles.insert(std::make_pair(LHIPZ, SkelDOF(0.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(LHIPY, SkelDOF(0.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(LHIPX, SkelDOF(10.0, -90.0, 90.0)));
  ai.angles.insert(std::make_pair(LKNEE, SkelDOF(-10.0, -120.0, -10.0)));
  ai.angles.insert(std::make_pair(LANKLE, SkelDOF(0.0, -45.0, 45.0)));

  visitArcs(ai);
}

void HumanSkeleton::buildSkeleton()
{
  ShapeBone* bone;
  ShapeCube* empty;

  empty = new ShapeCube();
  empty->setScale(0.0, 0.0, 0.0);
  SkelNode* rootNode = new SkelNode("Root", empty);
  SkelArc* rootArc = new SkelArc("Root", rootNode);
  setRootArc(rootArc);

  // Create the torso - a bone cluster   //躯干底部是root基点。
  BoneCluster* bones = new BoneCluster();
  bones->addBone(ShapeBone::HORIZONTALX, 0.6, Vector3(0.0, 0.5 - ShapeBone::BONEWIDTH_2, 0.0));
  bones->addBone(ShapeBone::VERTICAL, 1.0);
  bones->addBone(ShapeBone::HORIZONTALX, 0.3, Vector3(0.0, -0.5 + ShapeBone::BONEWIDTH_2, 0.0));

  SkelNode* torso = new SkelNode("Torso", bones);
  SkelArc* body = new SkelArc("Body", torso);
  rootNode->addArc(body);

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
  

/* xmy注释掉胳膊
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
*/

/* xmy注释掉胳膊
  // Create lower right arm
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.6);
  bone->setTranslation(0.0, -0.3, 0.0);                       // Put top of bone at origin
  SkelNode* rlowerarm = new SkelNode("RLowerArm", bone);
  SkelArc* relbow = new SkelArc("RElbow", rlowerarm);
  relbow->setTranslation(0.0, -0.5, 0.0);
  relbow->setRotationAxis(-1.0, 0.0, 0.0);
  rupperarmz->addArc(relbow);
*/

/* xmy注释掉胳膊
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
*/

/* xmy注释掉胳膊
  // Create lower left arm
  bone = new ShapeBone(ShapeBone::VERTICAL, 0.6);
  bone->setTranslation(0.0, -0.3, 0.0);                       // Put top of bone at origin
  SkelNode* llowerarm = new SkelNode("LLowerArm", bone);
  SkelArc* lelbow = new SkelArc("LElbow", llowerarm);
  lelbow->setTranslation(0.0, -0.5, 0.0);
  lelbow->setRotationAxis(-1.0, 0.0, 0.0);
  lupperarmz->addArc(lelbow);
xmy注释掉胳膊结束 */

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

  bone = new ShapeBone(ShapeBone::VERTICAL, UP_lEG_LENGTH);
  bone->setTranslation(0.0, -UP_lEG_LENGTH/2, 0.0);
  SkelNode* rupperlegz = new SkelNode("RUpperLegZ", bone);
  SkelArc* rhipz = new SkelArc("RHipZ", rupperlegz);
  rhipz->setRotationAxis(0.0, 0.0, -1.0);
  rupperlegy->addArc(rhipz);


  // Create lower right leg
  bone = new ShapeBone(ShapeBone::VERTICAL, DOWN_lEG_LENGTH);
  bone->setTranslation(0.0, -DOWN_lEG_LENGTH/2, 0.0);
  SkelNode* rlowerleg = new SkelNode("RLowerLeg", bone);
  SkelArc* rknee = new SkelArc("RKnee", rlowerleg);
  rknee->setTranslation(0.0, -UP_lEG_LENGTH, 0.0);
  rknee->setRotationAxis(-1.0, 0.0, 0.0);
  rupperlegz->addArc(rknee);


  // Create right foot      //脚是相对小腿去设置的位置
  bone = new ShapeBone(ShapeBone::HORIZONTALZ, FOOT_LENGTH);
  bone->setTranslation(0.0, -ShapeBone::BONEWIDTH_2, 0.15 - ShapeBone::BONEWIDTH_2);
  SkelNode* rfoot = new SkelNode("RFoot", bone);
  SkelArc* rankle = new SkelArc("RAnkle", rfoot);
  rankle->setTranslation(0.0, -DOWN_lEG_LENGTH, 0.0);
  rankle->setRotationAxis(-1.0, 0.0, 0.0);
  rlowerleg->addArc(rankle);


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

  bone = new ShapeBone(ShapeBone::VERTICAL, UP_lEG_LENGTH);
  bone->setTranslation(0.0, -UP_lEG_LENGTH/2, 0.0);                             // Put top of bone at origin
  SkelNode* lupperlegz = new SkelNode("LUpperLegZ", bone);
  SkelArc* lhipz = new SkelArc("LHipZ", lupperlegz);
  lhipz->setRotationAxis(0.0, 0.0, -1.0);
  lupperlegy->addArc(lhipz);


  // Create lower left leg
  bone = new ShapeBone(ShapeBone::VERTICAL, DOWN_lEG_LENGTH);
  bone->setTranslation(0.0, -DOWN_lEG_LENGTH/2, 0.0);                             // Put top of bone at origin
  SkelNode* llowerleg = new SkelNode("LLowerLeg", bone);
  SkelArc* lknee = new SkelArc("LKnee", llowerleg);
  lknee->setTranslation(0.0, -UP_lEG_LENGTH, 0.0);
  lknee->setRotationAxis(-1.0, 0.0, 0.0);
  lupperlegz->addArc(lknee);

  // Create left foot
  bone = new ShapeBone(ShapeBone::HORIZONTALZ, FOOT_LENGTH);
  bone->setTranslation(0.0, -ShapeBone::BONEWIDTH_2, 0.15 - ShapeBone::BONEWIDTH_2);
  SkelNode* lfoot = new SkelNode("LFoot", bone);
  SkelArc* lankle = new SkelArc("LAnkle", lfoot);
  lankle->setTranslation(0.0, -DOWN_lEG_LENGTH, 0.0);
  lankle->setRotationAxis(-1.0, 0.0, 0.0);
  llowerleg->addArc(lankle);
}

void HumanSkeleton::destroyChildren(const SkelArc* arc)
{
  SkelNode* node = arc->getNode();

  //Process the children
  skelarcs_t::const_iterator i;
  for(i = node->begin(); i != node->end(); ++i)
  {
    destroyChildren(*i);
    delete *i;
  }

  delete node->getShape();
  delete node;
}
