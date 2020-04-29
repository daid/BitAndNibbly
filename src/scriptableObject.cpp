#include "scriptableObject.h"
#include "player.h"

#include <sp2/random.h>
#include <sp2/graphics/textureManager.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/collision/2d/box.h>
#include <sp2/collision/2d/chains.h>


ScriptableObject::ScriptableObject(sp::P<sp::Node> parent, sp::Vector2d position, sp::Vector2d size)
: Interactable(parent, position, size)
{
}

void ScriptableObject::setTile(int index)
{
    float f = 17.0 / 543.0;
    float u = 16.0 / 543.0;

    int xidx = index % 32;
    int yidx = index / 32;
    sp::Vector2f uv0(f * xidx, f * yidx);
    sp::Vector2f uv1(uv0 + sp::Vector2f(u, u));

    render_data.type = sp::RenderData::Type::Normal;
    render_data.texture = sp::texture_manager.get("tiles.png");
    render_data.shader = sp::Shader::get("internal:basic.shader");
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1, 1), uv0, uv1);
    render_data.order = -1;
}

void ScriptableObject::setCollision(sp::Vector2d size, sp::Vector2d offset)
{
    sp::collision::Box2D shape(size.x, size.y, offset.x, offset.y);
    shape.fixed_rotation = true;
    shape.type = sp::collision::Shape::Type::Sensor;
    setCollisionShape(shape);
}

void ScriptableObject::setCollisionSolid(sp::Vector2d size, sp::Vector2d offset)
{
    sp::collision::Box2D shape(size.x, size.y, offset.x, offset.y);
    shape.type = sp::collision::Shape::Type::Static;
    setCollisionShape(shape);
}

void ScriptableObject::setCollisionPlatform(float width, sp::Vector2f offset)
{
    sp::collision::Chains2D shape;
    shape.chains = {{ {offset.x-width/2.0f, offset.y}, {offset.x+width/2.0f, offset.y} }};
    setCollisionShape(shape);
}

void ScriptableObject::onFixedUpdate()
{
    if (!update_coroutine || !update_coroutine->resume())
    {
        update_coroutine = onUpdateCallback.callCoroutine();
    }
}

void ScriptableObject::onUse(sp::P<Player> player)
{
    player->script_coroutine = onUseCallback.callCoroutine(player);
}

void ScriptableObject::onCollision(sp::CollisionInfo& info)
{
    sp::P<Player> player = info.other;
    if (player && !player->script_coroutine)
    {
        player->script_coroutine = onTouchCallback.callCoroutine(player);
    }
}

void ScriptableObject::destroy()
{
    delete this;
}

void ScriptableObject::onRegisterScriptBindings(sp::script::BindingClass& script_binding_class)
{
    Interactable::onRegisterScriptBindings(script_binding_class);
    
    script_binding_class.bindProperty("position", &sp::Node::getPosition2D, &sp::Node::setPosition);
    script_binding_class.bind("setTile", &ScriptableObject::setTile);
    script_binding_class.bind("setCollision", &ScriptableObject::setCollision);
    script_binding_class.bind("setCollisionSolid", &ScriptableObject::setCollisionSolid);
    script_binding_class.bind("setCollisionPlatform", &ScriptableObject::setCollisionPlatform);
    script_binding_class.bind("onUpdate", onUpdateCallback);
    script_binding_class.bind("onUse", onUseCallback);
    script_binding_class.bind("onTouch", onTouchCallback);
    script_binding_class.bind("destroy", &ScriptableObject::destroy);
}
