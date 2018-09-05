#ifndef HUMANSTAIRANIMATION_H
#define HUMANSTAIRANIMATION_H

#include <map>
#include "skeleton.h"
#include "humanskeleton.h"
#include "humanstairframer.h"
#include "animation.h"

/// Class to animate a human character walking across terrain
class HumanStairAnimation : public Animation
{
public:
  /// Class containing the actor to be animated
  class HumanStairActor : public Actor
  {
  public:
    /// The number of animatino cylces that have executed
    unsigned int cycles_;
    /// The current frame - for selecting which skeleton to use: root as torso, left foot, or right foot
    unsigned int currentFrame_;
    /// The torso rooted skeleton
    HumanSkeleton* skeltorso_;
    /// The right foot rooted skeleton
    HumanSkeletonRFoot skelrfoot_;
    /// The left foot rooted skeleton
    HumanSkeletonLFoot skellfoot_;
  public:
    /// Constructor
    HumanStairActor(HumanSkeleton* skeleton, HumanStairFramer* framer) : cycles_(0), currentFrame_(0), skeltorso_(skeleton), Actor(skeleton, framer) { }

    /// Destructor
    virtual ~HumanStairActor() { }
  };
public:
  /// Constructor
  HumanStairAnimation() { }

  /// Destructor
  ~HumanStairAnimation() { }

  /// New add actor which will create skeletons rooted at the feet
  void addActor(HumanSkeleton* skeleton, HumanStairFramer* framer);

  /// update the frame
  virtual void update();
};

#endif
