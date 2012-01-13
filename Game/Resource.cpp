#include "Resource.h"

Resource*   Resource::_resource_ptr =   NULL;

Resource::Resource(sf::Thread* loadThread, std::string resourceDir)
    :   m_load_state(State::Null),
        ResourceDir(resourceDir),
        ConfigDir(ResourceDir+"config/"),
        ImageDir(ResourceDir+"image/"),
        FontDir(ResourceDir+"font/"),
        ErrorImage("error.png")
{
    m_debug = true;

    if(m_debug)
        std::cout << "\n[Resource] Initializing...\n\tSetting vars...\n";
    m_loadSize  =   0;
    m_loadLeft  =   0;
    m_loading   =   false;

    if(m_debug)
        std::cout << "\tInitializing directories...\n";

    if(!FileAction::FindDir(ResourceDir)){
        FileAction::MakeDir(ResourceDir);

        if(m_debug)
            std::cout << "\tCreated\t" << ResourceDir << "\n";
    }

    if(!FileAction::FindDir(ConfigDir)){
        FileAction::MakeDir(ConfigDir);

        if(m_debug)
            std::cout << "\tCreated\t" << ConfigDir << "\n";
    }

    if(!FileAction::FindDir(ImageDir)){
        FileAction::MakeDir(ImageDir);

        if(m_debug)
            std::cout << "\tCreated\t" << ImageDir << "\n";
    }

    if(!FileAction::FindDir(FontDir)){
        FileAction::MakeDir(FontDir);

        if(m_debug)
            std::cout << "\tCreated\t" << FontDir << "\n";
    }

    m_loadThread_ptr    =   loadThread;

    if(m_debug)
        std::cout << "\tSetting hooks...\n";

    Magnet::Hooks("Resource::Constructor")->Register(Hook::Load, &Resource::Hook_Load);

    if(m_debug)
        std::cout << "[Resource] Initialized.\n\n";
}

Resource::~Resource()
{
    delete [] _resource_ptr;
}

Resource* Resource::Object(){
    if(_resource_ptr == NULL){
        if(Object()->m_debug)
            std::cout << "Resource::Object()-> WARNING: Resource as not been initialized! Null pointer returned!\n";
    }
    return _resource_ptr;
}

void Resource::Init(sf::Thread* loadThread, std::string resourceDir){
    _resource_ptr   =   new Resource(loadThread, resourceDir);
}

void Resource::Add(std::string file){
    if(Resource::Loading()) return;

    if(Object()->m_load_state.get() == State::Ready){
        Object()->m_load_state.set(State::Null);
    }

    Object()->m_load_queue.push(file);
    Object()->m_loadSize = Object()->m_load_queue.size();
    Object()->m_loadLeft = Object()->m_load_queue.size();

    if(Object()->m_debug)
        std::cout << "[Resource] Added \"" << file << "\" to the load queue\n";
}

bool Resource::NeedLoad(){
    if(Object()->m_load_state.get() == State::Null && Object()->m_loadSize > 0){
        return true;
    }

    return false;

}

void Resource::Hook_Load(){
    if(Resource::NeedLoad()){
        Object()->m_load_state.set(State::Loading);

        if(Object()->m_debug)
            std::cout << "[Resource][Load] Loading...\n";
        Object()->m_loadThread_ptr->Launch();
    }else{
        Object()->m_load_state.set(State::Ready);
    }
}

void Resource::Load(void* data){
    //Process load queue
    while(!Object()->m_load_queue.empty()){
        std::string file = Object()->m_load_queue.front();

        if(!Object()->m_resource_vect.count(file)){
            ResourcePointer* resource = new ResourcePointer(file);

            if(resource->isValid()){
                Object()->m_resource_vect[resource->file()] = resource;
            }else{
                std::cout << "*[Resource] [Load] Warning: Couldn't load resource \"" << file << "\"\n";
            }
        }

        Object()->m_load_queue.pop();
        Object()->m_loadLeft = Object()->m_load_queue.size();
    }


    if(Object()->m_debug)
        std::cout << "[Resource][Load] Done loading\n";

    Object()->m_load_state.set(State::Ready);
}

sf::Image& Resource::GetImage(std::string file){
    if(Object()->m_resource_vect.count(file)){
        return Object()->m_resource_vect[file]->getImage();
    }else{
        return Object()->m_resource_vect[Object()->ErrorImage]->getImage();
    }
}
