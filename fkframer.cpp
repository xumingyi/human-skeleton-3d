/*
	Ö¡Çý¶¯Æ÷
*/
#include "fkframer.h"

FKFramer::FKFramer()
: currentFrame_(0),
  totalFrames_(0),
  frameStart_(-1.0)
{
}

void FKFramer::update(Animation::angles_t& angles)
{
	printf("FKFramer update start \n");
  if(!frames_.empty())
  {
    // Has the timer been initialized?
    if(frameStart_ < 0.0)
      frameStart_ = timer_.getTime();

    // Get time elapsed since start of frame
    double t = timer_.getTime() - frameStart_;

    t *= rate_;

    if(t > frames_[currentFrame_].getDuration())
    {
      // Proceed to next frame
      if(++currentFrame_ == totalFrames_)
      {
        cycles_++;
        currentFrame_ = 0;
      }

      frameStart_ = timer_.getTime();
      t = 0.0;
    }

    double a = t / frames_[currentFrame_].getDuration();

    FKFrame::frameAngles_t::const_iterator iter;
    for(iter = frames_[currentFrame_].begin(); iter != frames_[currentFrame_].end(); ++iter)
    {
      angles[iter->first].setValue(iter->second + ((frames_[currentFrame_ + 1][iter->first] - iter->second) * a));
    }
  }
}
