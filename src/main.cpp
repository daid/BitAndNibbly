#include <sp2/engine.h>
#include <sp2/window.h>
#include <sp2/assert.h>
#include <sp2/logging.h>
#include <sp2/io/directoryResourceProvider.h>
#include <sp2/graphics/gui/scene.h>
#include <sp2/graphics/gui/theme.h>
#include <sp2/graphics/gui/loader.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/graphics/scene/collisionrenderpass.h>
#include <sp2/graphics/textureManager.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/spriteAnimation.h>
#include <sp2/collision/2d/circle.h>
#include <sp2/collision/2d/box.h>
#include <sp2/collision/2d/chains.h>
#include <sp2/scene/scene.h>
#include <sp2/scene/node.h>
#include <sp2/scene/camera.h>
#include <sp2/scene/tilemap.h>
#include <sp2/io/keybinding.h>
#include <sp2/io/http/server.h>
#include <sp2/tweak.h>

#include <json11/json11.hpp>

#include "areaScene.h"
#include "controls.h"
#include "player.h"
#include "ladder.h"
#include "door.h"


sp::P<sp::Window> window;

class SceneManager : public sp::Updatable
{
public:
    SceneManager(int player_count)
    {
        for(int n=0; n<player_count; n++)
        {
            render_pass[n] = new sp::BasicNodeRenderPass();
        
            sp::P<sp::SceneGraphicsLayer> scene_layer = new sp::SceneGraphicsLayer(1);
            scene_layer->addRenderPass(render_pass[n]);
#ifdef DEBUG
            scene_layer->addRenderPass(new sp::CollisionRenderPass());
#endif
            if (n == 0)
                scene_layer->setViewport(sp::Rect2d(0, 0, 0.5, 1.0));
            else
                scene_layer->setViewport(sp::Rect2d(0.5, 0, 0.5, 1.0));
            window->addLayer(scene_layer);

            scenes[n] = new AreaScene(n);
            //scenes[n]->load("home_inside");
            scenes[n]->load("houc1");

            render_pass[n]->addCamera(scenes[n]->getCamera());
        }
    }
    
    virtual void onUpdate(float delta) override
    {
    }
    
    sp::P<sp::BasicNodeRenderPass> render_pass[2];
    sp::P<AreaScene> scenes[2];
};


int main(int argc, char** argv)
{
    sp::P<sp::Engine> engine = new sp::Engine();

    //Create resource providers, so we can load things.
    new sp::io::DirectoryResourceProvider("resources");

    //Disable or enable smooth filtering by default, enabling it gives nice smooth looks, but disabling it gives a more pixel art look.
    sp::texture_manager.setDefaultSmoothFiltering(false);

    //Create a window to render on, and our engine.
    window = new sp::Window(4.0/3.0);
    window->setPosition(sp::Vector2f(0.5, 0.5));
#ifndef DEBUG
    window->setFullScreen(true);
    window->hideCursor();
#endif

    sp::gui::Theme::loadTheme("default", "gui/theme/basic.theme.txt");
    //new sp::gui::Scene(sp::Vector2d(640, 480), sp::gui::Scene::Direction::Horizontal);

    SceneManager scene_manager{2};
    
    engine->run();
    
    return 0;
}
