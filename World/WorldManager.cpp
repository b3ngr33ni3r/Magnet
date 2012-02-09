#include "WorldManager.h"

WorldManager* WorldManager::m_ptr = NULL;

WorldManager::WorldManager()
{
    //ctor
    m_curuuid=0;
    defaultconstraint=500;

    //achievs
    has_clicked=false;//player hasn't clicked originally


    //the hooked hooks
    Renderer::Hooks()->Register(Hook::Frame,&WorldManager::Renderer_Frame_Hook);
    Magnet::Hooks()->Register(Hook::Setup,&WorldManager::Magnet_Load_Hook);

    //all our events
    EventHandler::AddListener(new EventListener(sf::Event::MouseButtonReleased, &WorldManager::Event_MouseButtonReleased));
    EventHandler::AddListener(new EventListener(sf::Event::MouseButtonPressed, &WorldManager::Event_MouseButtonPressed));
    EventHandler::AddListener(new EventListener(sf::Event::MouseWheelMoved, &WorldManager::Event_MouseWheelMoved));
    EventHandler::AddListener(new EventListener(sf::Event::KeyReleased,&WorldManager::Event_KeyReleased));
    EventHandler::AddListener(new EventListener(sf::Event::MouseMoved, &WorldManager::Event_MouseMoved));
    EventHandler::AddListener(new EventListener(sf::Event::KeyPressed,&WorldManager::Event_KeyPresed));

    //Magnet::Achieves()->Register(new Achievement("World_Mouse_Click","You learned to click the screen!",&WorldManager::Achievement_Completion),new EventListener(sf::Event::MouseButtonReleased,&WorldManager::Achievement_Conditions));

    //init mouseposes
    Released_Mouse = b2Vec2(0,0);
    Init_Mouse = b2Vec2(0,0);

    //lol_logo
    lol_logo_counter=0;
    lol_sprite = new sf::Sprite();


}


void WorldManager::SelectWorld(World* in)
{
    std::list<World*>::iterator it;
    for (it=m_WorldList.begin();it != m_WorldList.end();it++)
    {

        if ((*it)->uuid == in->uuid)
            m_curWorld = (*it);

    }

}

World* WorldManager::GetWorld(World* in)
{
    std::list<World*>::iterator it;
        for (it=m_WorldList.begin();it != m_WorldList.end();it++)
        {

            if ((*it)->uuid == in->uuid)
                return (*it);

        }
}

void WorldManager::Remove(World* in)
{
    std::list<World*>::iterator it;
    for (it=m_WorldList.begin();it != m_WorldList.end();it++)
    {

        if ((*it)->uuid == in->uuid){
            (*it)->Unload();
            m_WorldList.erase(it);
        }

    }
}


WorldManager::~WorldManager()
{
    //dtor
}

void WorldManager::Init()
{
    m_ptr = new WorldManager();
}

WorldManager* WorldManager::Access()
{
    if (!m_ptr)
        std::cout<<"[WorldManager][Access] Returning NULL PTR.\n";

    return m_ptr;
}

World* WorldManager::CurrentWorld()
{
  if (!m_curWorld)
    std::cout<<"[WorldManager][CurrentWorld] Returning NULL PTR.\n";

  return m_curWorld;
}

//**HOOKS BELOW**/

void WorldManager::Renderer_Frame_Hook()
{
    if (Access()->lol_logo_counter==0)
    {

   /* Access()->lol_sprite->SetImage(Resource::GetImage("image/boot-logo.png"));
    Access()->lol_sprite->SetCenter(Resource::GetImage("image/boot-logo.png").GetWidth()/2,Resource::GetImage("image/boot-logo.png").GetHeight()/2);
    Access()->lol_sprite->SetPosition(Renderer::GetRenderWindow()->GetWidth()/2,Renderer::GetRenderWindow()->GetHeight()/2);
    Renderer::CreateLink(Access()->lol_sprite,Renderer::MenuLayer);*/

    }

    if (Access()->lol_logo_counter>100)
    {
        Renderer::RemoveLink(Access()->lol_sprite);
    }


    Access()->lol_logo_counter++;

    if (Access()->CurrentWorld())
    Access()->CurrentWorld()->Step();
}

void WorldManager::Magnet_Load_Hook()
{

    //create a world on startup
    Access()->SelectWorld(Access()->AddUndefinedWorld());//add a new undefined world, and select it

}

//**HOOKS ABOVE**/
//**EVENTS BELOW**/
bool WorldManager::Event_KeyReleased(sf::Event evt){

     if (evt.Key.Code == sf::Key::LControl)
     {
         const sf::Input& inpt = Renderer::GetRenderWindow()->GetInput();

         Access()->CurrentWorld()->AddShape(new Projectile(sf::Vector2f(inpt.GetMouseX(),inpt.GetMouseY()),b2Vec2(0,50)));
     }




    if (evt.Key.Code == sf::Key::Num1)
    {
        Access()->CurrentWorld()->SetCurrentMaterial(new Material(MatType::Default));
    }else if (evt.Key.Code == sf::Key::Num2)
    {
        Access()->CurrentWorld()->SetCurrentMaterial(new Material(MatType::Heavy));
    }else if (evt.Key.Code == sf::Key::Num3)
    {
        Access()->CurrentWorld()->SetCurrentMaterial(new Material(MatType::Light));
    }else if (evt.Key.Code == sf::Key::Num4)
    {
        Access()->CurrentWorld()->SetCurrentMaterial(new Material(MatType::Rubber));
    }else if (evt.Key.Code == sf::Key::Num5)
    {
        Access()->CurrentWorld()->SetCurrentMaterial(new Material(2,0.5f,0.3f,"image/tire.png"));
    }


    if (evt.Key.Code == sf::Key::Num0)
    {
        World* temp = Access()->CurrentWorld();
        Access()->HideWorld(temp);
        Access()->SelectWorld(Access()->AddUndefinedWorld());//autoselects it too...
    }

    if (evt.Key.Code == sf::Key::Num6)
    {
        Access()->CurrentWorld()->AddShape(new Circle(50,b2Vec2(0.5,1),500,false,sf::Vector2f(300,300), new Material(2,0.00002f,0.6f,"image/earth.png")));
    }


    if (evt.Key.Code == sf::Key::Num9)
    {
        Access()->CurrentWorld()->AddShape(new Rect(0,0,sf::Vector2f(200,100),new Material(.5,0.002f,0.3f,"Magnet","font/tahoma.ttf")));
    }

    if (evt.Key.Code == sf::Key::Num7)
    {
        Access()->CurrentWorld()->AddShape(new Circle(40,true,sf::Vector2f(288,214),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(60,true,sf::Vector2f(967,349),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(10,true,sf::Vector2f(548,640),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(10,true,sf::Vector2f(1187,150),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(25,true,sf::Vector2f(218,626),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(15,true,sf::Vector2f(1252,627),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(15,true,sf::Vector2f(1252,627),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(15,true,sf::Vector2f(122,57),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(15,true,sf::Vector2f(177,227),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(15,true,sf::Vector2f(135,527),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(15,true,sf::Vector2f(152,427),new Material(MatType::Floor)));

        Access()->CurrentWorld()->AddShape(new Rect(5,10,true,sf::Vector2f(1022,57),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Rect(87,110,true,sf::Vector2f(1477,627),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Rect(7,10,true,sf::Vector2f(735,227),new Material(MatType::Floor)));
        Access()->CurrentWorld()->AddShape(new Circle(34,true,sf::Vector2f(652,827),new Material(MatType::Floor)));


    }

    if (evt.Key.Code == sf::Key::Slash)
    {
        Access()->CurrentWorld()->SetTimestep(1.0f / 40.0f);
    }


    if (evt.Key.Code == sf::Key::End)
    {
        Access()->CurrentWorld()->AwakenAll();
        Access()->CurrentWorld()->SetGravity(b2Vec2(0.0f,0.0f));
    }

    if (evt.Key.Code == sf::Key::Num8)
    {
        Access()->CurrentWorld()->AddShape(new Line(0,0,2000,0));
        Access()->CurrentWorld()->AddShape(new Line(0,0,0,2000));
        Access()->CurrentWorld()->AddShape(new Line(0,873,2000,873));
        Access()->CurrentWorld()->AddShape(new Line(1430,0,1430,2000));
    }

    if (evt.Key.Code == sf::Key::Up)
    {
        Access()->CurrentWorld()->AwakenAll();
        Access()->CurrentWorld()->SetGravity(b2Vec2(0.0f,-10.0f));
    }

    if (evt.Key.Code == sf::Key::Down)
    {
        Access()->CurrentWorld()->AwakenAll();
        Access()->CurrentWorld()->SetGravity(b2Vec2(0.0f,10.0f));
    }

    if (evt.Key.Code == sf::Key::Left)
    {
        Access()->CurrentWorld()->AwakenAll();
        Access()->CurrentWorld()->SetGravity(b2Vec2(-10.0f,0.0f));
    }

    if (evt.Key.Code == sf::Key::Right)
    {
        Access()->CurrentWorld()->AwakenAll();
        Access()->CurrentWorld()->SetGravity(b2Vec2(10.0f,0.0f));
    }


    return true;
}

bool WorldManager::Event_KeyPresed(sf::Event evt){

  if (evt.Key.Code == sf::Key::Comma)
    {
        if(Access()->CurrentWorld()->GetTimestep() > WorldStandards::minSpeed){
        Access()->CurrentWorld()->SetTimestep(Access()->CurrentWorld()->GetTimestep()-(1.0f / 1000.0f));
        }
    }

    if (evt.Key.Code == sf::Key::Period)
    {

        if(Access()->CurrentWorld()->GetTimestep() < WorldStandards::maxSpeed)
        Access()->CurrentWorld()->SetTimestep(Access()->CurrentWorld()->GetTimestep()+(1.0f / 1000.0f));
    }


    return true;
}


bool WorldManager::Event_MouseButtonPressed(sf::Event evt)
{

    Access()->Init_Mouse = b2Vec2(evt.MouseButton.X,evt.MouseButton.Y);
    return true;
}

bool WorldManager::Event_MouseButtonReleased(sf::Event evt)
{
    int radius = 5;
    int i=0; //if for is commented out, just do this for now.
    int w = radius*2;
     int h = w;
     int tHeight=w;

    Access()->Released_Mouse = b2Vec2(evt.MouseButton.X,evt.MouseButton.Y);

    if (!Access()->has_clicked)
    Access()->has_clicked=true;//now player has cliked (achieves)





    b2Vec2 force = b2Vec2(WorldStandards::rgrav_forceConst*(Access()->Released_Mouse.x-Access()->Init_Mouse.x), WorldStandards::rgrav_forceConst*(Access()->Released_Mouse.y-Access()->Init_Mouse.y));

                    if(force.x < 0){
                        if(force.x < -30){
                            force = b2Vec2(-30, force.y);
                        }
                    }else{
                        if(force.x > 30){
                            force = b2Vec2(30, force.y);
                        }
                    }

                    if(force.y < 0){
                        if(force.y < -30){
                            force = b2Vec2(force.x, -30);
                        }
                    }else{
                        if(force.y > 30){
                            force = b2Vec2(force.x, 30);
                        }
                    }

                    //std::cout << "X: " << force.x << " Y: " << force.y << std::endl;




         if (evt.MouseButton.Button == sf::Mouse::Left){
                //for(int i=0; i<100; i++) //difference released-init

                    Access()->CurrentWorld()->AddShape(new Circle(radius,force,sf::Vector2f(evt.MouseButton.X-radius+i*radius,evt.MouseButton.Y-radius),Access()->CurrentWorld()->CurrentMaterial()));
         }
         if (evt.MouseButton.Button == sf::Mouse::Right)
            //for(int i=0; i<100; i++)
                Access()->CurrentWorld()->AddShape(new Rect(w,h,force,sf::Vector2f(evt.MouseButton.X-w+i*w,evt.MouseButton.Y-h),Access()->CurrentWorld()->CurrentMaterial()));



    return true;

}

bool WorldManager::Event_MouseWheelMoved(sf::Event evt)
{

        int radius = 10;
    int i=0; //if for is commented out, just do this for now.
    int w = radius*2;
     int h = w;
     int tHeight=w;


     const sf::Input& inpt = Renderer::GetRenderWindow()->GetInput();
        for(int i=0; i<5; i++)
        Access()->CurrentWorld()->AddShape(new Circle(radius,sf::Vector2f(inpt.GetMouseX()-tHeight+i*tHeight,inpt.GetMouseY()-tHeight),new Material(1,0.002f,0.3f,"image/moon.png")));

    return true;
}

bool WorldManager::Event_MouseMoved(sf::Event evt)
{
    return true;
}


//**EVENTS ABOVE**/
//**ACHEIVES BELOW**/
bool WorldManager::Achievement_Conditions(sf::Event evt)
{
    if (evt.Type == sf::Event::MouseButtonReleased)
        {
            if (Access()->has_clicked)
                std::cout<<"Clicked via achievs";
            else
                return false;
        }
}

void WorldManager::Achievement_Completion(std::string name)
{
    std::cout<<"[WorldManager][Achievement_Completion] Congrats! You completed "<<name<<"!\n";
}
