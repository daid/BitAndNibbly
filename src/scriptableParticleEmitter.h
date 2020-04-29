#ifndef SCRIPTABLE_PARTICLE_EMITTER_H
#define SCRIPTABLE_PARTICLE_EMITTER_H

#include <sp2/scene/particleEmitter.h>

class ScriptableParticleEmitter : public sp::ParticleEmitter
{
public:
    ScriptableParticleEmitter(sp::P<sp::Node> parent);
    
    virtual void onUpdate(float delta) override;
    virtual void onRegisterScriptBindings(sp::script::BindingClass& script_binding_class) override;
    
private:
    float frequency = 3.0;
    float accumulator = 0.0;

    sp::Vector2f area{0.4, 0.3};
    sp::Color color{1, 1, 1};

    void setArea(float x, float y) { area.x = x / 2.0f; area.y = y / 2.0f; }
    void setColor(float r, float g, float b, float a) { color = sp::Color(r, g, b, a); }
    void setFrequency(float f) { frequency = f; }
    void destroy();
};

#endif//SCRIPTABLE_PARTICLE_EMITTER_H
