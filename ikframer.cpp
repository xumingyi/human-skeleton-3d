/*
	Ö¡Çý¶¯Æ÷½Ó¿Ú
*/
#include "shape.h"
#include "ikframer.h"

#ifndef M_PI
#define M_PI     3.1415926535897932384626433832795
#endif
#ifndef M_PI_2
#define M_PI_2   1.5707963267948966192313216916398
#endif
#ifndef DEG2RAD
#define DEG2RAD  0.017453292519943295769236907684886
#endif

#define EPSILON  0.01

IKSolver::InfoGetter::InfoGetter()
: total_(0)
{ 
  G_.SetIdentity();
}

void IKSolver::InfoGetter::reset()
  {
    total_ = 0;
    G_.SetIdentity();
    positions_.clear();
    names_.clear();
    angles_.clear();
    axes_.clear();
  }

void IKSolver::InfoGetter::operator()(SkelArc* arc)
{
  Vector3 p, a;
  Matrix44 t, r;

  // Update the global matrix
  t.SetTranslation(arc->getTranslation()->getX(), arc->getTranslation()->getY(), arc->getTranslation()->getZ());
  r.SetRotation(arc->getRotationAngle()->getValue(), Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));

  t *= r;
  G_ *= t;

  // Calculate joint position
  p = G_ * p;

  positions_.push_back(p);

  // Transform axis of rotation
  Matrix hg(4,1);
  hg[0][0] = arc->getRotationAxis()->getX(); 
  hg[1][0] = arc->getRotationAxis()->getY(); 
  hg[2][0] = arc->getRotationAxis()->getZ(); 
  hg[3][0] = 0.0; 

  Matrix hh;
  hh.Mult(G_, hg);

  a.set(hg[0][0], hg[1][0], hg[2][0]);
  a.Normalize();
  axes_.push_back(a);

  angles_.push_back(*arc->getRotationAngle());
  names_.push_back(arc->getName());

  total_++;
}

class AngleSetter : public Skeleton::ArcVisitor
{
protected:
  Animation::angles_t * angles_;
public:
  AngleSetter(Animation::angles_t* angles) : angles_(angles) { }
  virtual void operator()(SkelArc* arc)
  {
    Animation::angles_t::const_iterator iter = angles_->find(arc->getName());
    if(iter != angles_->end())
      arc->setRotationAngle(iter->second);
  }
};

IKSolver::IKSolver(const Skeleton* skel, const std::string& root, const std::string& endeffector)
: stepScale_(0.1)
{
  // Find the "root" of our skeleton
  SkelArc* oldArc = skel->findArc(root);
  if(oldArc)
  {
    SkelNode* node = new SkelNode(oldArc->getNode()->getName(), NULL);

    // The new root will be at the origin, so the translations will not be copied
    SkelArc* arc = new SkelArc(oldArc->getName(), node);
    arc->setRotationAngle(*oldArc->getRotationAngle());
    arc->setRotationAxis(*oldArc->getRotationAxis());
//    arc->setTranslation(*oldArc->getTranslation());
    skel_.setRootArc(arc);

    SkelArc* parent = oldArc;
    SkelArc* lastArc = arc;

    skelarcs_t::iterator iter = parent->getNode()->begin();
    while(iter != parent->getNode()->end() && parent->getName() != endeffector)
    {
      SkelArc* oldArc = *iter;
      node = new SkelNode(oldArc->getNode()->getName(), NULL);
      arc = new SkelArc(oldArc->getName(), node);
      arc->setRotationAngle(*oldArc->getRotationAngle());
      arc->setRotationAxis(*oldArc->getRotationAxis());
      arc->setTranslation(*oldArc->getTranslation());

      lastArc->getNode()->addArc(arc);

      parent = oldArc;
      lastArc = arc;

      iter = parent->getNode()->begin();
    }
  }
}

void IKSolver::computeJacobian(const InfoGetter& ig)
{
  // The endeffector is at the last position in the InfoGetter's arc vector
  unsigned int i, endeffector = ig.total_ - 1;
  std::vector<Vector3> vs;

  // Set the values for v in v = Jt
  std::vector<Vector3>::const_iterator citer;
  for(i = 0; i < endeffector; i++)
    vs.push_back(ig.positions_[endeffector] - ig.positions_[i]);

  // Get longest distance from origin
  std::vector<Vector3>::iterator iter = vs.begin();
  double length = (*iter).Mag();
  iter++;
  for(; iter != vs.end(); ++iter)
  {
    double l = (*iter).Mag();
    if(l > length)
      length = l;
  }

  // Scale all of the vectors to (0..1) range
  for(iter = vs.begin(); iter != vs.end(); ++iter)
    (*iter) /= length;

  Vector3 temp;
  J_.Resize(3, endeffector);
  for(i = 0; i < endeffector; i++)
  {
    temp.Cross(ig.axes_[i], vs[i]);
    J_[0][i] = temp.getX();
    J_[1][i] = temp.getY();
    J_[2][i] = temp.getZ();
  }

  JT_.Copy(J_);
  JT_.Transpose();

  Matrix JJTI;
  JJTI.Mult(J_, JT_);
  JJTI.Invert();

  JTJJTI_.Mult(JT_, JJTI);
}

void IKSolver::solve(Animation::angles_t& angles)
{
  unsigned int i, j, endeffector;
  Vector3 v;

  InfoGetter ig;

  // Update the angles of the chain that was copied from the initial skeleton
  AngleSetter update(&angles);
  skel_.visitArcs(update);

  for(i = 0; i < 50; i++)
  {
    // Get joint angles and positions
    skel_.visitArcs(ig);

    // Endeffector is the last item in the InfoGetter's arc vector
    endeffector = ig.total_ - 1;

    // Calculate the end effector
    v = goal_ - ig.positions_[endeffector];

    if(v.Mag() < EPSILON)
      break;

    computeJacobian(ig);

    v *= stepScale_;
    v.Normalize();

    Matrix V(3, 1);
    V[0][0] = v.getX();
    V[1][0] = v.getY();
    V[2][0] = v.getZ();

    Matrix ThetaDot;
    ThetaDot.Mult(JTJJTI_, V);

    double temp = ThetaDot[2][0];
    for(j = 0; j < ig.total_ - 1; j++)
    {
      angles[ig.names_[j]].setValue(ig.angles_[j].getValue() + ThetaDot[j][0]);
    }

    // Apply the angles and recalculate end effector position
    AngleSetter as(&angles);
    skel_.visitArcs(as);

    ig.reset();
  }
}

void IKFramer::addChain(const std::string& name, const Skeleton* skel, const std::string& root, const std::string& endeffector)
{
  iksolvers_.insert(std::make_pair(name, IKSolver(skel, root, endeffector)));
}

void IKFramer::setGoal(const std::string& name, double x, double y, double z)
{
  iksolvers_t::iterator iter = iksolvers_.find(name);
  if(iter != iksolvers_.end())
    iter->second.setGoal(x, y, z);
}

void IKFramer::update(Animation::angles_t& angles)
{
  iksolvers_t::iterator iter;
  for(iter = iksolvers_.begin(); iter != iksolvers_.end(); ++iter)
  {
    iter->second.solve(angles);
  }
}
