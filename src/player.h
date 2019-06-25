#ifndef PLAYER_H
#define PLAYER_H

#include <sp2/scene/node.h>

class Controls;
class Ladder;
class ClimbZone;

class Player : public sp::Node
{
public:
    Player(sp::P<sp::Node> parent, sp::Vector2d position, Controls& controls);
    
    virtual void onFixedUpdate() override;
    virtual void onCollision(sp::CollisionInfo& info) override;
    void onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class) override;
    
    sp::script::CoroutinePtr script_coroutine;
private:
    bool anyKeyPressed();
    void die() { alive = false; }

    Controls& controls;
    
    bool alive = true;
    bool on_ground = false;
    sp::Vector2d previous_velocity;
    sp::P<Ladder> ladder;
    sp::P<ClimbZone> climb_zone;
    bool ladder_request = false;
    int death_countdown = 120;
};

#endif//PLAYER_H