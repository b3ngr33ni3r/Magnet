#ifndef PANEL_H
#define PANEL_H

#include <SFML/Graphics.hpp>
#include "Component.h"

namespace mgui{
    class Panel : virtual public Component
    {
        public:
            Panel(const char* name);
            virtual ~Panel();

            virtual bool onMouseRelease(sf::Event evt);
            virtual bool onMouseMove(sf::Vector2f mouse_pos);
            virtual bool onMousePress(sf::Event evt);
        protected:
        private:
            bool            m_mouse_down;
            sf::Vector2f    m_mouse_dist;
    };
}

#endif // PANEL_H
