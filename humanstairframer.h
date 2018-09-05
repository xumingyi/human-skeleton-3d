#ifndef HUMANSTAIRFRAMER_H
#define HUMANSTAIRFRAMER_H

#include "humanskeleton.h"
#include "fkframer.h"

// Specialized skeleton with right foot as root
class HumanSkeletonRFoot : public HumanSkeleton
{
protected:
  /// Create the skeleton
  virtual void buildSkeleton();
  /// Specialized version of recursive function to retrieve world space transformations for arcs
  virtual void arcTransforms(const SkelArc* arc, const Matrix44& G, ArcTransformVector_t& transforms);
public:
  /// Constructor
  HumanSkeletonRFoot() { }
  /// Destructor
  virtual ~HumanSkeletonRFoot() { }
};

// Specialized skeleton with left foot as root
class HumanSkeletonLFoot : public HumanSkeleton
{
protected:
  /// Create the skeleton
  virtual void buildSkeleton();
  /// Specialized version of recursive function to retrieve world space transformations for arcs
  virtual void arcTransforms(const SkelArc* arc, const Matrix44& G, ArcTransformVector_t& transforms);
public:
  /// Constructor
  HumanSkeletonLFoot() { }
  /// Destructor
  virtual ~HumanSkeletonLFoot() { }
};

/// Class extending FKFramer to create a walking up stairs animation
class HumanStairFramer : public FKFramer
{
protected:
  /// Create key positions for skeletal animation
  void setupFrames();
public:
  /// Constructor
  HumanStairFramer();
};

#endif
