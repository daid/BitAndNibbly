#include "portal.h"
#include "areaScene.h"
#include "player.h"

#include <sp2/graphics/spriteAnimation.h>
#include <sp2/tween.h>
#include <sp2/random.h>


Portal::Portal(sp::P<sp::Node> parent, sp::Vector2d position, sp::string target)
: Interactable(parent, position, sp::Vector2d(1, 1)), target(target)
{
    setAnimation(sp::SpriteAnimation::load("portal.txt"));
    animationPlay("Closed");

    particle_emitter = new sp::ParticleEmitter(this);
    
    onFixedUpdate();
}

void Portal::onFixedUpdate()
{
    sp::P<AreaScene> scene = sp::Scene::get("AREA");
    
    render_data.color = sp::HsvColor(color_cycle, 70.0, 100.0);
    
    double distance = 20.0;
    if (scene->player)
        distance = (scene->player->getPosition2D() - getPosition2D()).length();
    render_data.color.a = sp::Tween<double>::easeInQuad(distance, 4.5, 1, 0.1, 0.9);
    particle_frequency = sp::Tween<double>::easeInQuad(distance, 4.5, 1, 1.0, 20.0);
    
    color_cycle += 1.0;
    if (color_cycle > 360.0)
        color_cycle -= 360.0;
    
    if (opening && animationIsFinished())
    {
        scene->load(target);
        opening = false;
    }
}

void Portal::onUpdate(float delta)
{
    particle_accumulator += delta * particle_frequency;

    while(particle_accumulator >= 1.0f)
    {
        particle_accumulator -= 1.0f;
    
        sp::ParticleEmitter::Parameters parameters;

        parameters.position.x = sp::random(-0.4, 0.4);
        parameters.position.y = -parameters.position.x;
        parameters.acceleration = sp::Vector3f(1, 1, 0);
        if (sp::random(0, 100) < 50)
            parameters.acceleration = -parameters.acceleration;
        parameters.velocity = parameters.acceleration * 0.1f;
        parameters.start_size = 0.1;
        parameters.end_size = 0.0;
        parameters.start_color = render_data.color;
        parameters.end_color = parameters.start_color;
        parameters.end_color.a = 0.0;
        parameters.lifetime = 1.0;

        particle_emitter->emit(parameters);
    }
}

void Portal::onUse(sp::P<Player> player)
{
    if (!opening)
    {
        animationPlay("Opening");
        opening = true;
    }
}
