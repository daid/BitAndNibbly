#ifndef WARP_H
#define WARP_H

#include <sp2/scene/node.h>

class Warp : public sp::Node
{
public:
    Warp(sp::P<sp::Node> parent, sp::Vector2d position, sp::Vector2d size, sp::string target);
    
    virtual void onCollision(sp::CollisionInfo& info) override;
private:
    sp::string target;
};

#endif//WARP_H
