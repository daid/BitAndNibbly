#ifndef CLIMBZONE_H
#define CLIMBZONE_H

#include <sp2/scene/node.h>


class ClimbZone : public sp::Node
{
public:
    ClimbZone(sp::P<sp::Node> parent, sp::Vector2d position, sp::Vector2d size);
};

#endif//CLIMBZONE_H
