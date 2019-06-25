#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <sp2/scene/node.h>

class Player;
class Interactable : public sp::Node
{
public:
    Interactable(sp::P<sp::Node> parent, sp::Vector2d position, sp::Vector2d size);
    
    virtual void onUse(sp::P<Player> player) = 0;
};

#endif//INTERACTABLE_H
