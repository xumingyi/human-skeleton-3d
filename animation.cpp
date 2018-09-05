#include "framer.h"
#include "animation.h"

//人物角色定义：骨架+帧驱动器
void Animation::addActor(Actor* actor)
{
  actors_.push_back(actor);

  ActorInitializer ao(actors_.back());
  actor->skeleton_->visitArcs(ao);
}

// update the frame
void Animation::update()
{
  actors_t::iterator iter;
  for(iter = actors_.begin(); iter != actors_.end(); ++iter)
  {
    if((*iter)->framer_)
      (*iter)->framer_->update((*iter)->angles_);

    // Traverse the skeleton
    ActorUpdater au(*iter);
    (*iter)->skeleton_->visitArcs(au);
  }
}

// draw the current frame
void Animation::draw()
{
  actors_t::iterator iter;
  for(iter = actors_.begin(); iter != actors_.end(); ++iter)
  {
    (*iter)->skeleton_->evaluate();
  }
}

Animation::~Animation()
{
  actors_t::iterator iter;
  for(iter = actors_.begin(); iter != actors_.end(); ++iter)
  {
    delete (*iter);
  }
}
