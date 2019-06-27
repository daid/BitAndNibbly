#include "door.h"
#include "areaScene.h"

Door::Door(sp::P<sp::Node> parent, sp::Vector2d position, sp::Vector2d size, sp::string target)
: Interactable(parent, position, size), target(target)
{
}

void Door::onUse(sp::P<Player> player)
{
    sp::P<AreaScene> scene = getScene();
    scene->load(target);
}
