#include "humanstairanimation.h"

void HumanStairAnimation::addActor(HumanSkeleton* skeleton, HumanStairFramer* framer)
{
  HumanStairActor* actor = new HumanStairActor(skeleton, framer);
  actor->skelrfoot_.create();
  actor->skellfoot_.create();

  Animation::addActor(actor);
}

void HumanStairAnimation::update()
{
	printf("HumanStairAnimation update start \n");
  // Update angles and check for frame change
  actors_t::iterator iter;
  for(iter = actors_.begin(); iter != actors_.end(); ++iter)
  {
    if((*iter)->framer_)
      (*iter)->framer_->update((*iter)->angles_);

    HumanStairActor* actor = dynamic_cast<HumanStairActor*>(*iter);
    if(actor)
    {
      FKFramer* framer = dynamic_cast<FKFramer*>(actor->framer_);
      if(framer)
      {
        if(actor->currentFrame_ != framer->getCurrentFrame())
        {
          // Check frame change for transition to skeleton with different root
          actor->currentFrame_ = framer->getCurrentFrame();
          if(actor->currentFrame_ == 2)
          {
            // Get the positions
            Skeleton::ArcPositionVector_t positions;
            actor->skeleton_->getArcPositions(positions);

            // Change the skeleton
            actor->skeleton_ = &actor->skelrfoot_;
            actor->skeleton_->getRootArc()->setTranslation(positions["RAnkle"]);
          }
          else if(actor->currentFrame_ == 0 || actor->currentFrame_ == 4)
          {
            // Get the positions
            Skeleton::ArcPositionVector_t positions;
            actor->skeleton_->getArcPositions(positions);

            // Change the skeleton
            actor->skeleton_ = actor->skeltorso_;
            actor->skeleton_->getRootArc()->setTranslation(positions["Body"]);
          }
          else if(actor->currentFrame_ == 6)
          {
            // Get the positions
            Skeleton::ArcPositionVector_t positions;
            actor->skeleton_->getArcPositions(positions);

            // Change the skeleton
            actor->skeleton_ = &actor->skellfoot_;
            actor->skeleton_->getRootArc()->setTranslation(positions["LAnkle"]);
          }
        }
      }
    }

    // Traverse the skeleton
    ActorUpdater au(*iter);
    (*iter)->skeleton_->visitArcs(au);
  }
}
