#include "warp.h"
#include "player.h"
#include "areaScene.h"

#include <sp2/collision/2d/box.h>


Warp::Warp(sp::P<sp::Node> parent, sp::Vector2d position, sp::Vector2d size, sp::string target)
: sp::Node(parent), target(target)
{
    setPosition(position);

    sp::collision::Box2D shape(size.x / 2.0, size.y / 2.0);
    shape.fixed_rotation = true;
    shape.type = sp::collision::Shape::Type::Sensor;
    setCollisionShape(shape);
}

void Warp::onCollision(sp::CollisionInfo& info)
{
    sp::P<Player> player = info.other;
    if (!player)
        return;
    
    sp::P<AreaScene> scene = sp::Scene::get("AREA");
    scene->load(target);
}