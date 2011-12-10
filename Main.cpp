#include <iostream>
#include "Gun/GunMaker.h"
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "ImageHandler.h"
#include "State/GameState.h"

int main()
{
    /*************************************************
    **********=>    This is the debug
    **********=>      code.
    **************************************************/
    std::cout<<"This is OurGame\n";
    srand ( time(NULL) );//random seeder for guns
    GunMaker Gun(.3f);

    GunMaker Gun2(.3f);

    GunMaker Gun3(.3f);

    GunMaker Gun4(.3f);
    std::cout<<"GunsMade";

    Gun.SetPosition(0,0);
    Gun2.SetPosition(0,130);
    Gun3.SetPosition(0,250);
    Gun4.SetPosition(0,380);


    /*************************************************
    **********=>    End debug code
    **************************************************/

     //sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "OurGame");

//    GameState currentState();

    while (Renderer::Window()->IsOpened())
    {
        /***Load Handlers **/
        /*switch(currentState.GetState()){
            case GameState::NULL:
                //What to do here?
                break;
            case GameState::LOADING:

                //Compress to ResourceHandler
                ImageHandler::Init();
                //FontHandler::Init();


                RenderHandler::Init();


                break;
            case GameState::READY:
                //On
                break;
        }*/


        sf::Event Event;
        while (Renderer::Window()->GetEvent(Event))
        {
            // Close window : exit
            if ((Event.Type == sf::Event::Closed)||((Event.Type == sf::Event::KeyReleased)&&(Event.Key.Code == sf::Key::Escape))){
                ImageHandler::PrintAvailableImages();
                Renderer::Window()->Close();
            }
        }

        // Clear the screen (fill it with black color)
        Renderer::Render();

        /*To draw these guns you need to:
            Renderer::Link(sf::Drawable*);  //sf::Drawable being the sprite that is the gun
        */
    }
    return 0;
}