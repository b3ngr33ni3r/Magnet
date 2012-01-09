#ifndef WORLDSTANDARDS_H
#define WORLDSTANDARDS_H

#include <Box2D/Box2D.h>

namespace WorldStandards{


static const float ratio = 30.0f;
static const float ppm = ratio;
static const float mpp = (1.0F/ratio);
static const float unratio = mpp;
static const float degtorad = (b2_pi / 180.0);
static const float radtodeg = (180.0 / b2_pi);


static const bool debug = true; //set to false to supress couts
static const bool debug_step = true; //set to false to supress couts in stepping


};



#endif // WORLD_H
