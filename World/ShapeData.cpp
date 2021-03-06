#include "ShapeData.h"


ShapeData::ShapeData(b2FixtureDef bodyFixture,b2BodyDef engineBodyDef)
{
    //ctor
    fdef = bodyFixture;
    bdef = engineBodyDef;

    r_grav = false;

}

ShapeData::~ShapeData()
{
    //dtor
}

void ShapeData::Create(b2World* engineWorld)
{

    world = engineWorld;
    EngineBody = world->CreateBody(&bdef);
    EngineBody->CreateFixture(&fdef);
    //now its in the world simulation
}

void ShapeData::Remove()
{
    world->DestroyBody(EngineBody);
    //now this EngineBody won't be simulated
}

void ShapeData::EnableRadialGravity(RadialGravity::Type type,float multiplier,int distance)
{
    if (type == RadialGravity::Pull)
        r_grav_direction = -1;
    else
        r_grav_direction = 1;

    r_grav_multiplier = multiplier;
    r_grav_distance = distance;
    r_grav = true;
}
