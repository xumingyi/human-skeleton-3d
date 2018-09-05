#include "skin.h"

void Skin::Vertex::compute(Skeleton::ArcTransformVector_t& transforms)
{
  // Clear position
  position_.set();

  // Compute position
  AttachmentList_t::iterator ali;
  for(ali = attachments_.begin(); ali != attachments_.end(); ++ali)
  {
    Attachment* a = *ali;
    Matrix44 mat = transforms[a->name_];
    position_ += (mat * a->displacement_) * a->weight_;
  }
}

void Skin::update(Skeleton* skel)
{
  // First update the transforms
  skel->getArcTransforms(transforms_);

  // Compute the vertex positions
  VertexList_t::iterator vli;
  for(vli = verts_.begin(); vli != verts_.end(); ++vli)
  {
    (*vli)->compute(transforms_);
  }
}
