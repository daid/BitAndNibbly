#ifndef PORTAL_H
#define PORTAL_H

#include "interactable.h"
#include <sp2/scene/particleEmitter.h>


class Portal : public Interactable
{
public:
    Portal(sp::P<sp::Node> parent, sp::Vector2d position, sp::string target);

    virtual void onUse(sp::P<Player> player) override;
    virtual void onFixedUpdate() override;
    virtual void onUpdate(float delta) override;
private:
    bool opening = false;
    float color_cycle = 0.0;
    sp::string target;

    float particle_frequency = 10.0;
    float particle_accumulator = 0.0;
    sp::P<sp::ParticleEmitter> particle_emitter;
};

#endif//PORTAL_H