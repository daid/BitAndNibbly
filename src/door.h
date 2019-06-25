#ifndef DOOR_H
#define DOOR_H

#include "interactable.h"

class Door : public Interactable
{
public:
    Door(sp::P<sp::Node> parent, sp::Vector2d position, sp::Vector2d size, sp::string target);

    virtual void onUse(sp::P<Player> player) override;
private:
    sp::string target;
};

#endif//DOOR_H
