#include "speechBubble.h"

#include <sp2/graphics/fontManager.h>
#include <sp2/graphics/textureManager.h>


SpeechBubble::SpeechBubble(sp::P<sp::Node> parent, sp::string text)
: sp::Node(parent)
{
    render_data.type = sp::RenderData::Type::Normal;
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1, 1));
    render_data.shader = sp::Shader::get("internal:basic.shader");
    render_data.texture = sp::texture_manager.get("speech.png");
    render_data.color = sp::Color(0.9, 0.9, 0.9);
    render_data.order = 9;

    sp::Font* font = sp::font_manager.get("gui/theme/Boxy-Bold.ttf");
    sp::Font::PreparedFontString prepared = font->prepare(text, 32, 0.4, sp::Vector2d(0, 0), sp::Alignment::BottomLeft);
    size = sp::Vector2d(prepared.getUsedAreaSize());

    background_node = new sp::Node(this);
    
    sp::P<sp::Node> text_node = new sp::Node(background_node);
    text_node->render_data.type = sp::RenderData::Type::Normal;
    text_node->render_data.mesh = prepared.create();
    text_node->render_data.texture = font->getTexture(32);
    text_node->render_data.shader = sp::Shader::get("internal:basic.shader");
    text_node->render_data.color = sp::Color(0, 0, 0);
    text_node->render_data.order = 15;
    text_node->setPosition(sp::Vector2d(0, 0.14));

    sp::MeshData::Vertices vertices;
    sp::MeshData::Indices indices{0,1,2,2,1,3};
    vertices.reserve(4);
    
    vertices.emplace_back(sp::Vector3f(-0.2, 0, 0.0f), sp::Vector2f(0, 1));
    vertices.emplace_back(sp::Vector3f(size.x+0.2, 0, 0.0f), sp::Vector2f(1, 1));
    vertices.emplace_back(sp::Vector3f(-0.2, size.y, 0.0f), sp::Vector2f(0, 0));
    vertices.emplace_back(sp::Vector3f(size.x+0.2, size.y, 0.0f), sp::Vector2f(1, 0));

    background_node->render_data.type = sp::RenderData::Type::Normal;
    background_node->render_data.mesh = sp::MeshData::create(std::move(vertices), std::move(indices));
    background_node->render_data.shader = sp::Shader::get("internal:color.shader");
    background_node->render_data.color = sp::Color(0.9, 0.9, 0.9);
    background_node->render_data.order = 14;
    
    onFixedUpdate();
}

void SpeechBubble::onFixedUpdate()
{
    //Try to position the box with text towards the center
    sp::Vector2d position = getGlobalPosition2D();
    sp::Vector2d offset(4.0 - position.x - size.x / 2.0, 0.5);
    
    //Limit so the box is above the speech arrow
    offset.x = std::min(-0.5, offset.x);
    offset.x = std::max(-size.x+0.5, offset.x);
    
    //Limit so the box is not outside the view space
    offset.x = std::max(offset.x, 0.2 - position.x);
    offset.x = std::min(offset.x, 7.8 - position.x - size.x);

    if (position.y + size.y > 8.0 - 0.5)
    {
        offset.x = -offset.x;
        offset.y += size.y;
        setRotation(180);
        background_node->setRotation(-180);
    }
    else
    {
        setRotation(0);
        background_node->setRotation(0);
    }
    
    background_node->setPosition(offset);
}

void SpeechBubble::destroy()
{
    delete this;
}

void SpeechBubble::onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class)
{
    sp::Node::onRegisterScriptBindings(script_binding_class);
    
    script_binding_class.bind("destroy", &SpeechBubble::destroy);
}
