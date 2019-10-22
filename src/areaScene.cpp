#include "areaScene.h"
#include "ladder.h"
#include "climbZone.h"
#include "player.h"
#include "controls.h"
#include "door.h"
#include "warp.h"
#include "scriptableObject.h"
#include "scriptableParticleEmitter.h"
#include "speechBubble.h"
#include "portal.h"

#include <sp2/graphics/textureManager.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/scene/camera.h>
#include <sp2/scene/tilemap.h>
#include <sp2/collision/2d/chains.h>
#include <sp2/random.h>
#include <sp2/engine.h>
#include <sp2/tweak.h>

#include <json11/json11.hpp>


static int luaYield(lua_State* lua)
{
    return lua_yield(lua, 0);
}

static sp::P<SpeechBubble> luaShowSpeechBubble(sp::P<sp::Node> target, sp::string text)
{
    if (!target) return nullptr;
    
    return new SpeechBubble(target, text);
}

static int luaAddObject(lua_State* lua)
{
    lua_getfield(lua, lua_upvalueindex(1), "player");
    lua_getmetatable(lua, -1);
    lua_getfield(lua, -1, "object_ptr");
    sp::Node* player = dynamic_cast<sp::Node*>(static_cast<sp::ScriptBindingObject*>(lua_touserdata(lua, -1)));
    lua_pop(lua, 2);

    sp::Vector2d position = sp::script::convertFromLua(lua, sp::script::typeIdentifier<sp::Vector2d>{}, 1);
    sp::P<ScriptableObject> obj = new ScriptableObject(player->getParent(), position, sp::Vector2d(1, 1));
    return sp::script::pushToLua(lua, obj);
}

static sp::P<ScriptableParticleEmitter> luaAddParticleEmitter(sp::P<sp::Node> target)
{
    if (!target) return nullptr;
    
    return new ScriptableParticleEmitter(target);
}

static std::map<sp::string, int> lua_global_values;
static void luaSetGlobal(sp::string name, int value)
{
    lua_global_values[name] = value;
}

static int luaGetGlobal(sp::string name)
{
    return lua_global_values[name];
}

AreaScene::AreaScene(int player_nr)
: sp::Scene("AREA" + sp::string(player_nr)), player_nr(player_nr)
{
    sp::Camera* camera = new sp::Camera(getRoot());
    setDefaultCamera(camera);
    camera->setOrtographic(8.0);
    camera->setPosition(sp::Vector2d(4, 4));
}

void AreaScene::load(sp::string name)
{
    load_target = name;
}

void AreaScene::reload()
{
    load_target = current_area_name;
}

void AreaScene::onFixedUpdate()
{
    if (load_target.length() > 0)
    {
        _load(load_target);
        load_target = "";
    }
    if (Controls::quit.getDown())
        sp::Engine::getInstance()->shutdown();
}

void AreaScene::_load(sp::string name)
{
    auto stream = sp::io::ResourceProvider::get("areas/" + name + ".json");
    if (!stream)
    {
        LOG(Error, "Failed to find area:", name);
        return;
    }
    std::string err;
    json11::Json json = json11::Json::parse(stream->readAll(), err);

    if (current_area_name != name)
    {
        previous_area_name = current_area_name;
        current_area_name = name;
    }
    
    script.destroy();
    for(sp::P<sp::Node> node : getRoot()->getChildren())
    {
        if (node != getCamera())
            node.destroy();
    }

    script = new sp::script::Environment();

    sp::Node* background = new sp::Node(getRoot());
    background->render_data.type = sp::RenderData::Type::Normal;
    background->render_data.texture = sp::texture_manager.get("background.png");
    background->render_data.shader = sp::Shader::get("internal:basic.shader");
    background->render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(16, 16));
    background->render_data.order = 10;
    background->setPosition(sp::Vector2d(4, 4));

    sp::P<sp::Tilemap> tilemap = new sp::Tilemap(getRoot(), "tiles.png", 1.0, 1.0, 32, 32);
    tilemap->setTilemapSpacingMargin(1.0f / 16.0f + 0.01, 0.0f);
    tilemap->render_data.order = -5;

    sp::Vector2d start_position(4, 8.0);
    
    for(auto& layer : json["layers"].array_items())
    {
        sp::Tilemap::Collision collision = sp::Tilemap::Collision::Open;
        if (layer["name"].string_value() == "Solids")
            collision = sp::Tilemap::Collision::Solid;
        if (layer["name"].string_value() == "Platforms")
            collision = sp::Tilemap::Collision::Platform;
        auto& data = layer["data"];
        int w=layer["width"].int_value();
        int h=layer["height"].int_value();
        for(int y=0;y<h;y++)
        {
            for(int x=0;x<w;x++)
            {
                long long idx = data[x + y * w].number_value();
                int tile = (idx & 0xffff) - 1;
                if (idx & 0x80000000)
                    tile |= sp::Tilemap::flip_horizontal;
                if (idx & 0x40000000)
                    tile |= sp::Tilemap::flip_vertical;
                if (tile > -1)
                    tilemap->setTile(x, h-1-y, tile, collision);
            }
        }
        
        for(auto& object : layer["objects"].array_items())
        {
            sp::string name = object["name"].string_value();
            double rotation = -object["rotation"].number_value();
            sp::Vector2d position(object["x"].number_value() / 16.0, 8.0 - object["y"].number_value() / 16.0);
            sp::Vector2d size(object["width"].number_value() / 16.0, object["height"].number_value() / 16.0);
            position += (size / 2.0).rotate(rotation);
            
            if (name == "=LADDER")
            {
                new Ladder(getRoot(), position, size);
            }
            else if (name == "=CLIMB")
            {
                new ClimbZone(getRoot(), position, size);
            }
            else if (name == "=DOOR")
            {
                new Door(getRoot(), position, size, object["type"].string_value());
                if (previous_area_name == object["type"].string_value())
                    start_position = position;
            }
            else if (name == "=PORTAL")
            {
                new Portal(getRoot(), position, object["type"].string_value());
                if (previous_area_name == object["type"].string_value())
                    start_position = position;
            }
            else if (name == "=WARP")
            {
                new Warp(getRoot(), position, size, object["type"].string_value());
            }
            else if (name == "=ENTRY")
            {
                if (previous_area_name == object["type"].string_value())
                    start_position = position;
            }
            else
            {
                sp::P<ScriptableObject> so = new ScriptableObject(getRoot(), position, size);
                so->setRotation(rotation);
                so->setTile(object["gid"].int_value() - 1);
                script->setGlobal(name, so);
            }
        }
    }
    tilemap->onFixedUpdate();
    
    sp::P<sp::Node> edge = new sp::Node(getRoot());
    sp::collision::Chains2D shape;
    shape.loops = {{ {0, 0}, {8, 0}, {8, 10}, {0, 10} }};
    edge->setCollisionShape(shape);
    
    player = new Player(getRoot(), start_position, player_nr, player_controllers[player_nr]);

    script->setGlobal("player", player);
    
    script->setGlobal("yield", luaYield);
    script->setGlobal("random", sp::random);
    script->setGlobal("irandom", sp::irandom);
    
    script->setGlobal("setGlobal", luaSetGlobal);
    script->setGlobal("getGlobal", luaGetGlobal);
    
    script->setGlobal("showSpeechBubble", luaShowSpeechBubble);
    script->setGlobal("addObject", luaAddObject);
    script->setGlobal("addParticleEmitter", luaAddParticleEmitter);

    script->load("utils.lua");
    script->load("areas/" + name + ".lua");
}
