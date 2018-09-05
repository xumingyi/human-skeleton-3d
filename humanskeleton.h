//人物躯干生成
#ifndef HUMANSKELETON_H
#define HUMANSKELETON_H

#include <map>
#include "skeleton.h"

/// Class extending the skeleton class to create a human skeleton
class HumanSkeleton : public Skeleton
{
public:
  /// Functor to initialize the angles of the skeletal joints
  class AngleInitializer : public Skeleton::ArcVisitor
  {
  public:
    /// Map of angles for each named joint
    std::map<std::string, SkelDOF> angles;
  public:
    /// Operation to apply angle to joint
    virtual void operator()(SkelArc* arc)
    {
      std::map<std::string, SkelDOF>::const_iterator iter = angles.find(arc->getName());
      if(iter != angles.end())
        arc->setRotationAngle(iter->second);
    }
  };
protected:
  /// Set initial skeletal pose
  void initializeAngles();
  /// Create the skeleton
  virtual void buildSkeleton();
  /// Recursively destroy all of the descendents of a joint
  void destroyChildren(const SkelArc* arc);
  /// Destroy the entire skeleton
  virtual void destroySkeleton() { destroyChildren(root_); delete root_; root_ = NULL; }
public:
  /// Constructor
  HumanSkeleton();

  /// Destructor
  virtual ~HumanSkeleton();

  /// Initialize the skeleton
  virtual void create();
};

#endif
