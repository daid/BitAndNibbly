#include "player.h"
#include "controls.h"
#include "ladder.h"
#include "climbZone.h"
#include "interactable.h"
#include "areaScene.h"

#include <sp2/graphics/spriteAnimation.h>
#include <sp2/collision/2d/box.h>
#include <sp2/scene/scene.h>
#include <sp2/engine.h>


Player::Player(sp::P<sp::Node> parent, sp::Vector2d position, Controls& controls)
: sp::Node(parent), controls(controls)
{
    setAnimation(sp::SpriteAnimation::load("player.txt"));
    
    sp::collision::Box2D shape(0.6, 0.6, 0, -0.2);
    shape.fixed_rotation = true;
    setCollisionShape(shape);
    
    setPosition(position);

    getScene()->queryCollision(getPosition2D() - sp::Vector2d(0, 0.4), [this](sp::P<sp::Node> node)
    {
        ladder = node;
        return !ladder;
    });

    if (ladder)
        animationPlay("Climb");
    else
        animationPlay("Stand");
    
    render_data.order = 5;
}

void Player::onFixedUpdate()
{
    sp::Vector2d movement_request = controls.getMovement();
    if (script_coroutine)
    {
        movement_request = sp::Vector2d(0, 0);
        if (!script_coroutine->resume())
        {
            script_coroutine = nullptr;
            return;
        }
    }

    sp::Vector2d velocity = getLinearVelocity2D();
        
    if (ladder && !ladder->testCollision(getPosition2D() - sp::Vector2d(0, 0.4)))
        ladder = nullptr;
    if (climb_zone && !climb_zone->testCollision(getPosition2D() - sp::Vector2d(0, 0.4)))
        climb_zone = nullptr;

    if (alive && (controls.up.getDown() || controls.down.getDown()))
        ladder_request = true;
    if (movement_request.y == 0.0 || ladder)
        ladder_request = false;
    if (ladder_request && !ladder)
    {
        getScene()->queryCollision(getPosition2D() - sp::Vector2d(0, 0.4), [this](sp::P<sp::Node> node)
        {
            ladder = node;
            return !ladder;
        });
    }

    if (alive && (controls.up.getDown() || controls.use.getDown()) && on_ground && !script_coroutine)
    {
        getScene()->queryCollision(getPosition2D() - sp::Vector2d(0, 0.4), [this](sp::P<sp::Node> node)
        {
            sp::P<Interactable> interactable = node;
            if (interactable)
            {
                interactable->onUse(this);
                return false;
            }
            return true;
        });
    }
    
    on_ground = ladder || (velocity.y > previous_velocity.y + 0.1);
    if (climb_zone && on_ground)
        climb_zone = nullptr;
    if (!alive)
    {
        ladder = nullptr;
        climb_zone = nullptr;
        movement_request = sp::Vector2d(0, 0);
        if (death_countdown)
        {
            death_countdown--;
        }
        else
        {
            sp::P<AreaScene> scene = getScene();
            scene->reload();
        }
    }
    
    
    if (ladder)
    {
        velocity.y = movement_request.y;
        if (!script_coroutine && !controls.down.getDown() && !ladder->testCollision(getPosition2D() - sp::Vector2d(0, 0.4) + sp::Vector2d(0.0, velocity.y * sp::Engine::fixed_update_delta)))
            velocity.y = 0.0;
        velocity.x = (ladder->getPosition2D().x - getPosition2D().x) / sp::Engine::fixed_update_delta;
    }
    else if (climb_zone)
    {
        velocity.y = -0.5;
        velocity.x = (climb_zone->getPosition2D().x - getPosition2D().x);
    }
    else
    {
        velocity.y -= 0.32;
        velocity.x = velocity.x * 0.8 + (movement_request.x * 3.0) * 0.2;
    }
    if (alive && (on_ground || climb_zone) && !script_coroutine && controls.jump.getDown())
    {
        if (climb_zone)
        {
            velocity.x = std::copysign(5.5, getPosition2D().x - climb_zone->getPosition2D().x);
            velocity.y += 6.0;
        }
        else
        {
            velocity.y += 7.2;
        }
        ladder = nullptr;
        climb_zone = nullptr;
    }
    velocity.y = std::max(-10.0, velocity.y);
        
    
    previous_velocity = velocity;
    setLinearVelocity(velocity);

    if (movement_request.x > 0)
        animationSetFlags(0);
    if (movement_request.x < 0)
        animationSetFlags(sp::SpriteAnimation::FlipFlag);
    
    if (!alive)
    {
        animationPlay("Death");
    }
    else if (script_coroutine)
    {
        animationPlay("Use");
    }
    else if (climb_zone)
    {
        if (climb_zone->getPosition2D().x > getPosition2D().x)
            animationSetFlags(0);
        else
            animationSetFlags(sp::SpriteAnimation::FlipFlag);
        animationPlay("Climb");
    }
    else if (ladder)
    {
        if (velocity.y == 0.0)
            animationPlay("Ladder", 0.0);
        else
            animationPlay("Ladder");
    }
    else if (on_ground)
    {
        if (movement_request.x != 0)
            animationPlay("Walk");
        else
            animationPlay("Stand");
    }
    else
    {
        if (velocity.y > -7.0)
            animationPlay("Jump");
        else
            animationPlay("Fall");
    }
}

void Player::onCollision(sp::CollisionInfo& info)
{
    if (alive && !on_ground && !ladder && info.other && info.other->isSolid())
    {
        if (std::abs(info.normal.x) > 0.9 && std::signbit(info.normal.x) == std::signbit(controls.getMovement().x))
        {
            getScene()->queryCollision(getPosition2D() - sp::Vector2d(0, 0.4), [this](sp::P<sp::Node> node)
            {
                climb_zone = node;
                return !climb_zone;
            });
        }
    }
}

bool Player::anyKeyPressed()
{
    return controls.jump.getDown();
}

void Player::onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class)
{
    sp::Node::onRegisterScriptBindings(script_binding_class);
    
    script_binding_class.bindProperty("position", &sp::Node::getPosition2D, &sp::Node::setPosition);
    script_binding_class.bind("anyKeyPressed", &Player::anyKeyPressed);
    script_binding_class.bind("die", &Player::die);
}
