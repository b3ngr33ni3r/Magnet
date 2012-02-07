#include "Projectile.h"
#include "../WorldManager.h"

Projectile::Projectile(sf::Vector2f pos,b2Vec2 tForce,Material* mat,float degangle)
{
    //ctor
    Set_Width(5);
    Set_Height(5);
    Set_Position(pos);
    Set_Mat(mat);
    Set_Static(false);//default staticicity
    Set_Angle(degangle);
    Set_ShapeType(WorldShapes::Rect);
    Set_Force(tForce);
    Set_Bullet(true);
        Set_Radial_Gravity_Distance(0);
        Set_CreateWithForce(false);//just..bullets do their own force stuff so this is no
    Set_Radial_Gravity(b2Vec2(0,0));
    startanim(false);
}

Projectile::~Projectile()
{
    //dtor
    Destroy(Get_C_World());
}

void Projectile::Create(b2World* p_world)
{
    Set_C_World(p_world);
    //do box2d first..
    b2BodyDef bodyDef;
	if (!Get_Static()){
	bodyDef.type = b2_dynamicBody;
	}
    bodyDef.allowSleep = true;
	bodyDef.awake = true;
	bodyDef.bullet = true;

    //NO FUCKING CLUE...THIS IS DUMB. but it works...
    if (!Get_Static())
        bodyDef.position.Set(/*(*/(Get_Position().x+Get_Width())/*/2)*/*WorldStandards::ppm, /*(*/(Get_Position().y+Get_Height())/*/2)*/*WorldStandards::ppm);
    else
        bodyDef.position.Set(((Get_Position().x+Get_Width())/2)*WorldStandards::ppm, ((Get_Position().y+Get_Height())/2)*WorldStandards::ppm);

    bodyDef.angle = (((-1)*Get_Angle())*WorldStandards::degtorad);

	Set_Body(p_world->CreateBody(&bodyDef));

	b2PolygonShape dynamicBox;

	dynamicBox.SetAsBox((Get_Width()/2)*WorldStandards::ppm, (Get_Height()/2)*WorldStandards::ppm);

	//fixture stuff
	b2FixtureDef fixtureDef;

	fixtureDef.shape = &dynamicBox;

    if (!Get_Static()){
    /////add material class in the futurec.
    fixtureDef.density = Get_Mat()->GetDensity();
    fixtureDef.friction = Get_Mat()->GetFriction();
    fixtureDef.restitution = Get_Mat()->GetRestitution();
	//////
	}

	Get_Body()->CreateFixture(&fixtureDef);

    if (WorldStandards::debug)
        std::cout << "[Box2D] Added Projectile.\n";


    float b2posx = Get_Body()->GetPosition().x;
    float b2posy = Get_Body()->GetPosition().y;
    float b2rot  = Get_Body()->GetAngle();

    float sfposx = b2posx*WorldStandards::mpp;
    float sfposy = b2posy*WorldStandards::mpp;

    //do sfml
    Set_Shape(new sf::Shape(sf::Shape::Rectangle(0, 0,Get_Width(),Get_Height(),Get_Mat()->GetColor())));
    Get_Shape()->SetPosition(sf::Vector2f(sfposx, sfposy)); //Get_Position()
    Get_Shape()->SetCenter(sf::Vector2f((Get_Width()/2), (Get_Height()/2)));
    Get_Shape()->Rotate(Get_Angle());

    Renderer::CreateLink(Get_Shape());

    if (Get_Force().IsValid())
        ApplyForce(Get_Force());

    if (WorldStandards::debug)
        std::cout << "[SFML] Added Projectile.\n";


}
