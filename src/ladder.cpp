#include "ladder.h"

#include <sp2/collision/2d/box.h>


Ladder::Ladder(sp::P<sp::Node> parent, sp::Vector2d position, sp::Vector2d size)
: sp::Node(parent), size(size)
{
    setPosition(position);

    sp::collision::Box2D shape(size.x / 2.0, size.y);
    shape.fixed_rotation = true;
    shape.type = sp::collision::Shape::Type::Sensor;
    setCollisionShape(shape);
}