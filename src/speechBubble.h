#ifndef SPEECH_BUBBLE_H
#define SPEECH_BUBBLE_H

#include <sp2/scene/node.h>

class SpeechBubble : public sp::Node
{
public:
    SpeechBubble(sp::P<sp::Node> parent, sp::string text);

    virtual void onFixedUpdate() override;
    virtual void onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class) override;
private:
    void destroy();

    sp::Vector2d size;
    sp::P<sp::Node> background_node;
};

#endif//SPEECH_BUBBLE_H
