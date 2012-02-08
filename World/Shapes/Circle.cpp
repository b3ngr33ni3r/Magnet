#include "Circle.h"

Circle::Circle(int radius,sf::Vector2f pos,Material* mat,float degangle)
{
    //ctor
    Set_Radius(radius);
    Set_Position(sf::Vector2f(pos.x,pos.y));
    Set_Mat(mat);
    Set_Angle(degangle);
    Set_Static(false);
        Set_Radial_Gravity_Distance(0);
    Set_Radial_Gravity(b2Vec2(0,0));
    Set_CreateWithForce(false);
    startanim(false);
}

Circle::Circle(int radius,b2Vec2 tForce,sf::Vector2f pos,Material* mat,float degangle)
{
    //ctor
    Set_Radius(radius);
    Set_Position(sf::Vector2f(pos.x,pos.y));
    Set_Mat(mat);
    Set_Angle(degangle);
    Set_Static(false);

    Set_CreateWithForce(true);
    Set_CreateWithForce_Force(tForce);

    Set_Radial_Gravity_Distance(0);
    Set_Radial_Gravity(b2Vec2(0,0));
    startanim(false);
}


Circle::Circle(int radius,bool staticc,sf::Vector2f pos,Material* mat,float degangle)
{
    //ctor
    Set_Radius(radius);
    Set_Position(sf::Vector2f(pos.x,pos.y));
    Set_Mat(mat);
    Set_Angle(degangle);
    Set_Static(staticc);
    Set_Radial_Gravity_Distance(0);
    Set_Radial_Gravity(b2Vec2(0,0));
    Set_CreateWithForce(false);
    startanim(false);
}



Circle::Circle(int radius,b2Vec2 radialgrav,int radialdist,bool staticc,sf::Vector2f pos,Material* mat,float degangle)
{
    //ctor
    Set_Radius(radius);
    Set_Position(sf::Vector2f(pos.x,pos.y));
    Set_Mat(mat);
    Set_Angle(degangle);
    Set_Static(staticc);
    Set_Radial_Gravity(radialgrav);
    Set_Radial_Gravity_Distance(radialdist);
    pullorpush_val_set(radialgrav.y);
    Set_CreateWithForce(false);
    startanim(false);
}

Circle::~Circle()
{
    //dtor
    Destroy(Get_C_World());
}

void Circle::Create(b2World* p_world)
{
    //
    Set_C_World(p_world);
    b2BodyDef bodyDef;
    if (!Get_Static()){
	bodyDef.type = b2_dynamicBody;
    }
    bodyDef.allowSleep = true;
	bodyDef.awake = true;

    //OKAY, SO THIS WORKS ALL COMMENTED OUT.
    //if (Get_Static())
        bodyDef.position.Set((Get_Position().x/*+Get_Radius()*/)*WorldStandards::ppm, (Get_Position().y/*+Get_Radius()*/)*WorldStandards::ppm);
    //else
    //  bodyDef.position.Set(((Get_Position().x+Get_Radius())/2)*WorldStandards::ppm, ((Get_Position().y+Get_Radius())/2)*WorldStandards::ppm);

    bodyDef.angle = (((-1)*Get_Angle())*WorldStandards::degtorad);

	Set_Body(p_world->CreateBody(&bodyDef));

        if (Get_Mat()->UsesText()){//Text size of rect. disregard other size

        sf::String* temp = new sf::String();
        temp->SetText(Get_Mat()->GetText());

        if (temp->GetRect().GetWidth() > temp->GetRect().GetHeight())
        Set_Radius(temp->GetRect().GetWidth());

        if (temp->GetRect().GetWidth() < temp->GetRect().GetHeight())
        Set_Radius(temp->GetRect().GetHeight());

        delete temp;
        }

	b2CircleShape circle;

	circle.m_radius = Get_Radius()*WorldStandards::ppm;

	b2FixtureDef fixtureDef;

	fixtureDef.shape = &circle;

    if (!Get_Static()){
    /////add material class in the futurec.
    fixtureDef.density = Get_Mat()->GetDensity();
    fixtureDef.friction = Get_Mat()->GetFriction();
    fixtureDef.restitution = Get_Mat()->GetRestitution();
	//////
    }

	Get_Body()->CreateFixture(&fixtureDef);

	if (Get_CreateWithForce()){
	    std::cout<<"ApplyForce\n";
        ApplyForce(Get_CreateWithForce_Force());
	}

   if (WorldStandards::debug)
        std::cout << "[Box2D] Added Circle.\n";

    //do sfml

    if (Get_Mat()->UsesText())
    {//should really avoid this tho, looks quite odd.
         sf::String* temp = new sf::String();
        temp->SetText(Get_Mat()->GetText());

        temp->SetFont(*Get_Mat()->GetFont());
        temp->SetPosition(sf::Vector2f(Get_Position().x, Get_Position().y)); //Get_Position()
        temp->Rotate(Get_Angle());

        temp->SetCenter((Get_Radius()*(1/temp->GetScale().x))/2,(Get_Radius()*(1/temp->GetScale().y))/2);

        Set_Shape(temp);
    }
    else if (Get_Mat()->UsesImage())
    {
        sf::Sprite* temp = new sf::Sprite();
        temp->SetImage(*Get_Mat()->GetImage());
        temp->SetPosition(Get_Position());
        temp->Rotate(Get_Angle());
        temp->Resize(Get_Radius()*2, Get_Radius()*2);
        temp->SetCenter(Get_Radius()*(1/temp->GetScale().x),Get_Radius()*(1/temp->GetScale().y));
        Set_Shape(temp);
    }
    else
    {
        Set_Shape(new sf::Shape(sf::Shape::Circle(0,0,Get_Radius(),Get_Mat()->GetColor())));
        Get_Shape()->SetPosition(Get_Position());
        Get_Shape()->Rotate(Get_Angle());
    }


    Renderer::CreateLink(Get_Shape());

     if (WorldStandards::debug)
        std::cout << "[SFML] Added Circle.\n";

}
