#include "Animation.h"
#include "Resource.h"

Animation::Animation(std::string imageDir,int eachVisibleFor)
{
    //ctor
    m_mainPath = imageDir;
    m_Fps = eachVisibleFor;
    m_curframe=0;

    //for now. later, Resource::AddDir will populate this.
    m_files.push_back("poof/poof-1.png");
    m_files.push_back("poof/poof-2.png");
    m_files.push_back("poof/poof-3.png");
    m_files.push_back("poof/poof-4.png");
    m_files.push_back("poof/poof-5.png");

    //you cant do this here...
    //so fuckin gay. Resource shit cant be done here.


    Sprite->SetImage(Resource::GetImage(m_files[m_curframe]));

}

Animation::~Animation()
{
    //dtor
    delete Sprite;
}

void Animation::Cycle()
{

    m_cyclecounter++;

    if ( m_Fps == m_cyclecounter ){

    std::cout<<"Cycles match "<<m_cyclecounter<<" .\n";

    m_curframe++; m_cyclecounter=0; Sprite->SetImage(Resource::GetImage(m_files[m_curframe]));

                                }

}

