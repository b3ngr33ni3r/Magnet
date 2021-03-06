/*

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

#include "WorldStandards.h"
#include "EntityDimensions.h"
#include "ShapeTransform.h"
#include "ShapeDraw.h"
#include "ShapeData.h"
#include "Material/Material.h"

namespace EntityInfo{
	enum Type{
		Circle,
		Rect,
		Triangle,
		Bullet
		};

    //should change to something less confusing (confused with opengl context)
    enum Context{
        Static,
        Dynamic
		};

};

class Entity
{
    public:
        /// Default constructor
        Entity(EntityInfo::Type type,EntityDimensions* dims,Material* mat= new Material(MatType::Default),EntityInfo::Context context = EntityInfo::Dynamic);
        Entity(ShapeTransform* trans,ShapeDraw* draw,ShapeData* data,EntityInfo::Context context = EntityInfo::Dynamic);

        //the Transformable,Drawable,Data of this Entity
        ShapeTransform* Transform;
        ShapeDraw*	   Draw;
        ShapeData*	   Data;

        /// Default destructor
        ~Entity();

        //static or dynamic?
        EntityInfo::Context Context;
    protected:
    private:

        //if EntityInfo::Type constructor is used, these config stuff
        void CraftCircle(EntityInfo::Context context,EntityDimensions* dims,Material* mat);
        void CraftRect(EntityInfo::Context context,EntityDimensions* dims,Material* mat);
        void CraftTriangle(EntityInfo::Context context,EntityDimensions* dims,Material* mat);
        void CraftBullet(EntityInfo::Context context,EntityDimensions* dims,Material* mat);
};




#endif // ENTITY_H

*/
