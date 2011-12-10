#include "Renderer.h"

#include <queue>
#include <iterator>
#include <typeinfo>

Renderer*               Renderer::RendererPtr = NULL;

Renderer::Renderer()
{
    //Move to loader
    WindowPtr = new sf::RenderWindow(sf::VideoMode::GetMode(3), "Chess - Adam Zielinski");
   // m_isValid = false;
    m_cindex    =   0;

    m_isValid = false;

    //AddLayer(Layer newLayer);
}

//Clean up all the allocated memory space
Renderer::~Renderer()
{
    delete [] WindowPtr;
    delete [] RendererPtr;
}
/*
void Renderer::Link(sf::Drawable* linkedObj, Layer layer){
    GetObject()->Map(linkedObj, layer, 0);
}

void Renderer::Link(sf::Drawable* linkedObj, Layer layer, int depth){
    GetObject()->Map(linkedObj, layer, depth);
}
*/
/*
template<typename _T>
static void Renderer::Link(RenderObject<_T> &linkedObj){
    RenderHandler::GetObject()->Add(linkedObj);
}
*/
/*********************************************
    "Get the RenderHandler object"

    Returns a pointer to the RenderHandler

    *Uses lazy initialization
*********************************************/
Renderer* Renderer::GetObject(){
    if(RendererPtr == NULL){
        RendererPtr = new Renderer();
    }

    return RendererPtr;
}

/*********************************************
    "Gets a pointer to the current window"

    Returns a pointer to the sf::RenderWindow
*********************************************/
sf::RenderWindow* Renderer::Window(){
    return GetObject()->WindowPtr;
}
/*********************************************
            "Draw the screen "
*********************************************/
void Renderer::Render(){
    if(GetObject()->isValid()) return;

    Window()->Clear(sf::Color(0, 0, 0));

    for(int i=0; i<GetObject()->m_cindex; i++){
        Window()->Draw(*GetObject()->drawable_map[i]) ;
    }

    Window()->Display();

    GetObject()->validate();
}

void Renderer::CreateLink(sf::Drawable* drawable_ptr){
    GetObject()->drawable_map[GetObject()->m_cindex] = &*drawable_ptr;

    drawable_ptr = &*GetObject()->drawable_map[GetObject()->m_cindex];
    GetObject()->m_cindex++;
}

/*********************************************
    "Check if the screen is valid"

    returns TRUE if the screen is valid
            FALSE if the screen is invalid, aka doesn't represent the current data
*********************************************/
bool Renderer::isValid(){ return m_isValid; }

/*********************************************
    "Force the screen to redraw"
*********************************************/
void Renderer::invalidate(){ GetObject()->m_isValid = false; }

/*********************************************
    "sets m_isValid to true"
*********************************************/
void Renderer::validate(){ m_isValid = true; }