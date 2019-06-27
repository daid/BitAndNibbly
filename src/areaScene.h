#ifndef AREA_SCENE_H
#define AREA_SCENE_H

#include <sp2/scene/scene.h>
#include <sp2/script/environment.h>


class Player;
class AreaScene : public sp::Scene
{
public:
    AreaScene(int player_nr);

    void load(sp::string name);
    void reload();
    
    virtual void onFixedUpdate() override;
    
    sp::P<Player> player;
private:
    int player_nr;
    sp::string load_target;
    sp::string current_area_name;
    sp::string previous_area_name;
    sp::P<sp::script::Environment> script;
    
    void _load(sp::string name);
};

#endif//AREA_SCENE_H
