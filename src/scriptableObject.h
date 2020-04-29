#ifndef SCRIPTABLE_OBJECT_H
#define SCRIPTABLE_OBJECT_H

#include "interactable.h"

class ScriptableObject : public Interactable
{
public:
    ScriptableObject(sp::P<sp::Node> parent, sp::Vector2d position, sp::Vector2d size);
    
    void setTile(int index);
    
    virtual void onFixedUpdate() override;
    virtual void onUse(sp::P<Player> player) override;
    virtual void onCollision(sp::CollisionInfo& info) override;
    virtual void onRegisterScriptBindings(sp::script::BindingClass& script_binding_class) override;
private:
    void setCollision(sp::Vector2d size, sp::Vector2d offset);
    void setCollisionSolid(sp::Vector2d size, sp::Vector2d offset);
    void setCollisionPlatform(float width, sp::Vector2f offset);
    void setPosition2D(sp::Vector2d position);
    void destroy();

    sp::script::Callback onUpdateCallback;
    sp::script::CoroutinePtr update_coroutine;
    sp::script::Callback onUseCallback;
    sp::script::Callback onTouchCallback;
};

#endif//SCRIPTABLE_OBJECT_H
