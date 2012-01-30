#include "Renderer.h"
#include "../Magnet.h"

Renderer*               Renderer::RendererPtr         = NULL;

Renderer::Renderer()
{
    m_cindex        =   0;
    m_isValid       = false;
    m_shouldDraw    = true;
    m_running       = false;
    m_max_process   =    10;  //The total amount of queue memebers to process every think
    m_max_attempts  =    5;   //The amount of attempts to make to obtain a lock in think

    m_hooks         =   new Hook::Registry();

    Magnet::Hooks()->Register(Hook::Think, &Renderer::Think);
    EventHandler::AddListener(new EventListener(sf::Event::KeyPressed, &Renderer::Event_KeyPressed));
}

//Clean up all the allocated memory space
Renderer::~Renderer()
{
    delete [] RendererPtr;
    delete [] RenderWindow_ptr;
    delete [] m_hooks;
}

bool Renderer::Event_KeyPressed(sf::Event evt){
    if(evt.Key.Code == sf::Key::Escape){
        Renderer::Close(evt);
    }

    return true;
}

Hook::Registry* Renderer::Hooks(){
    return Object()->m_hooks;
}

sf::Mutex* Renderer::Mutex(){
    return &Object()->renderMutex;
}

bool Renderer::Close(sf::Event evt){
    std::cout << "Closed\n";
    Renderer::GetRenderWindow()->Close();
    Object()->renderThread_ptr->Wait();

    return true;
}

void Renderer::SetRenderWindow(sf::RenderWindow& Window){
    Object()->RenderWindow_ptr = &Window;
}

void Renderer::SetRenderThread(sf::Thread& renderThread){
    Object()->renderThread_ptr = &renderThread;
}

/*********************************************
    "Get the RenderHandler object"

    Returns a pointer to the RenderHandler

    *Uses lazy initialization
*********************************************/
Renderer* Renderer::Object(){
    if(RendererPtr == NULL){
        std::cout << "Renderer::Object()-> WARNING: Renderer not initialized! Null pointer returned\n";
    }

    return RendererPtr;
}

void Renderer::Init(sf::RenderWindow& window, sf::Thread& renderThread){
    RendererPtr = new Renderer();

    Renderer::SetRenderWindow(window);
    Renderer::SetRenderThread(renderThread);
}

/*********************************************
    "Gets a pointer to the current window"

    Returns a pointer to the sf::RenderWindow
*********************************************/
sf::RenderWindow* Renderer::GetRenderWindow(){
    return Object()->RenderWindow_ptr;
}

void Renderer::Think(){
    if(Magnet::GetState() != State::Ready) return;

    int attempts = 0;
    int process  = 0;


    while(!Object()->depth_queue.empty() && attempts < Object()->m_max_attempts && process < Object()->m_max_process){
        Object()->_SetLinkDepth(Object()->depth_queue.front());

        if(Object()->depth_queue.front().first->depth == Object()->depth_queue.front().second || !Renderer::Object()->LinkExists(Object()->depth_queue.front().first)){
            Object()->depth_queue.pop();
            process++;
        }

        attempts++;
    }

    attempts = 0;

    while(!Object()->layer_queue.empty() && attempts < Object()->m_max_attempts && process < Object()->m_max_process){
        Object()->_SetLinkLayer(Object()->layer_queue.front());

        if(Object()->layer_queue.front().first->layer == Object()->layer_queue.front().second){
            Object()->layer_queue.pop();
            process++;
        }

        attempts++;
    }

    //Process the remove link queue
    while(!Object()->delete_queue.empty() && attempts < Object()->m_max_attempts && process < Object()->m_max_process){
        Object()->_RemoveLink(Object()->delete_queue.front());

        if(!Renderer::Object()->LinkExists(Object()->delete_queue.front())){
            Object()->delete_queue.pop();
            process++;
        }

        attempts++;
    }

    attempts = 0;

    while(!Object()->newlink_queue.empty() && attempts < Object()->m_max_attempts && process < Object()->m_max_process){
        Object()->_CreateLink(Object()->newlink_queue.front());

        if(Renderer::Object()->LinkExists(Object()->newlink_queue.front())){
            Object()->newlink_queue.pop();
            process++;
        }

        attempts++;
    }

    Object()->renderThread_ptr->Launch();
}

/*********************************************
            "Draw the screen "
*********************************************/
void Renderer::Render(void* threadData){
    if(!GetRenderWindow()->IsOpened()) return;


    Renderer::Mutex()->Lock();
    Object()->m_running = true;

    Hooks()->Call(Hook::Frame);

    GetRenderWindow()->Clear(sf::Color(0, 0, 0));

    for(int i=0; i < Object()->links.size(); i++){
        GetRenderWindow()->Draw(*Object()->links[i]->object);
    }

    GetRenderWindow()->Display();

    Renderer::Mutex()->Unlock();
    Object()->m_running = false;
}

Renderer::Link* Renderer::CreateLink(sf::Drawable* drawable_ptr, Layer layer, int depth){
    Link* newLink   =   new Link();
    newLink->object  = drawable_ptr;
    newLink->layer   = layer;
    newLink->depth   = depth;

    Object()->newlink_queue.push(newLink);
    return newLink;
}

void Renderer::_CreateLink(Renderer::Link* newLink){
    Renderer::Mutex()->Lock();

    if(Object()->links.empty()){
        Object()->links.push_back(newLink);
    }else{
        _InsertLink(newLink);
    }
    Renderer::Mutex()->Unlock();
}

void Renderer::SetLinkDepth(Link* link, int depth){
    int link_index = Object()->GetLinkIndex(link);

    if(link_index == -1) return;
    if(link->depth == depth) return;

    Object()->depth_queue.push(depth_pair_t(link, depth));
}

void Renderer::SetLinkLayer(Link* link, Layer new_layer){
    int link_index = Object()->GetLinkIndex(link);

    if(link_index == -1) return;
    if(link->layer == new_layer) return;

    Object()->layer_queue.push(layer_pair_t(link, new_layer));
}

void Renderer::_SetLinkLayer(layer_pair_t layer_pair){
    Renderer::Mutex()->Lock();

    int link_index = GetLinkIndex(layer_pair.first);

    //Make sure the link wasn't removed
    if(link_index == -1){
        return;
    }

    links.erase(links.begin()+link_index);

    layer_pair.first->layer = layer_pair.second;
    _InsertLink(layer_pair.first);


    Renderer::Mutex()->Unlock();
}

void Renderer::_SetLinkDepth(depth_pair_t depth_pair){
    Renderer::Mutex()->Lock();

    int link_index = GetLinkIndex(depth_pair.first);

    //Make sure the link wasn't removed
    if(link_index == -1){
        return;
    }

    links.erase(links.begin()+link_index);

    depth_pair.first->depth = depth_pair.second;
    _InsertLink(depth_pair.first);


    Renderer::Mutex()->Unlock();
}

void Renderer::_InsertLink(Link* newLink){
    links_iterator_t it;
    bool insertBefore = false;
    bool insertAfter = false;

    for(it = Object()->links.begin(); it != Object()->links.end(); it++){

        if((*it)->layer > newLink->layer){
            insertBefore = true;
        }else if((*it)->layer == newLink->layer){
            if((*it)->depth > newLink->depth){
                insertBefore = true;
            }else if((*it)->depth == newLink->depth){
                if((it+1) == Object()->links.end() || (*(it+1))->layer != newLink->layer || (*(it+1))->depth != newLink->depth){
                    insertAfter = true;
                }
            }
        }
        if(!insertBefore && !insertAfter){
            if((it+1) == Object()->links.end()){
                insertAfter = true;
            }
        }

        if(insertBefore){
            Object()->links.insert(it, newLink);
            break;
        }

        if(insertAfter){
            Object()->links.insert(it+1, newLink);
            break;
        }
    }
}

void Renderer::CreateLink(Link* link_ptr){
    Object()->newlink_queue.push(link_ptr);
}

Renderer::Link* Renderer::CreateLink(sf::Drawable* drawable_ptr, Layer layer){
    return Renderer::CreateLink(drawable_ptr, layer, 0);
}


Renderer::Link* Renderer::CreateLink(sf::Drawable* drawable_ptr){
    return Renderer::CreateLink(drawable_ptr, Renderer::GameLayer, 0);
}

void Renderer::RemoveLink(sf::Drawable* drawable_ptr){
    Link* link = Object()->GetLinkByDrawable(drawable_ptr);

    if(link == NULL) return;

    Object()->delete_queue.push(link);
}

void Renderer::RemoveLink(Link* link_ptr){
    if(Object()->LinkExists(link_ptr)){
        Object()->delete_queue.push(link_ptr);
    }
}

void Renderer::_RemoveLink(Link* oldLink){
    Renderer::Mutex()->Lock();
     int linkIndex = GetLinkIndex(oldLink);

    if(linkIndex == -1) return;
    delete oldLink;
    links.erase(links.begin()+linkIndex);

    Renderer::Mutex()->Unlock();
}

Renderer::Link* Renderer::GetLinkByDrawable(sf::Drawable* drawable_ptr){
    Link* link = NULL;

    for(int i = 0; i < links.size(); i++){
        if(links[i]->object == drawable_ptr){
            link = links[i];
        }
    }

    return link;
}

bool Renderer::LinkExists(Renderer::Link* link_ptr){
    if(GetLinkIndex(link_ptr) != -1){
        return true;
    }

    return false;
}

int Renderer::GetLinkIndex(Link* link_ptr){
    for(int i = 0; i < links.size(); i++){
        if(links[i] == link_ptr){
            return i;
        }
    }

    return -1;
}

void Renderer::SetLinkLayer(sf::Drawable* drawable_ptr, Layer newLayer){
    //Renderer::RemoveLink(drawable_ptr);
    //Renderer::CreateLink(drawable_ptr, newLayer);
}
