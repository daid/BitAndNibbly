#include "scriptableParticleEmitter.h"

#include <sp2/random.h>
#include <sp2/tweak.h>


ScriptableParticleEmitter::ScriptableParticleEmitter(sp::P<sp::Node> parent)
: sp::ParticleEmitter(parent)
{
    render_data.order = parent->render_data.order + 1;
}

void ScriptableParticleEmitter::onUpdate(float delta)
{
    accumulator += delta * frequency;

    while(accumulator >= 1.0f)
    {
        accumulator -= 1.0f;
        
        sp::ParticleEmitter::Parameters parameters;

        parameters.position.x = sp::random(-area.x, area.x);
        parameters.position.y = sp::random(-area.y, area.y);
        //parameters.velocity;
        //parameters.acceleration.y = 2.0;
        parameters.size = 0.1;
        parameters.color = color;
        parameters.lifetime = 1.0;

        emit(parameters);
    }
    sp::ParticleEmitter::onUpdate(delta);
}

void ScriptableParticleEmitter::destroy()
{
    delete this;
}

void ScriptableParticleEmitter::onRegisterScriptBindings(sp::script::BindingClass& script_binding_class)
{
    sp::ParticleEmitter::onRegisterScriptBindings(script_binding_class);
    
    script_binding_class.bind("frequency", &ScriptableParticleEmitter::setFrequency);
    script_binding_class.bind("area", &ScriptableParticleEmitter::setArea);
    script_binding_class.bind("color", &ScriptableParticleEmitter::setColor);
    script_binding_class.bind("destroy", &ScriptableParticleEmitter::destroy);
}
