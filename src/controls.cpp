#include "controls.h"

Controls player_controllers[2]{{0}, {1}};
sp::io::Keybinding Controls::quit("quit", {"escape", "1", "2"});


Controls::Controls(int index)
: index(index)
, up("up" + sp::string(index))
, down("down" + sp::string(index))
, left("left" + sp::string(index))
, right("right" + sp::string(index))
, jump("jump_" + sp::string(index))
, use("use_" + sp::string(index))
, fire3("fire3_" + sp::string(index))
, unknown3("unknown3_" + sp::string(index))
, unknown4("unknown4_" + sp::string(index))
, unknown5("unknown5_" + sp::string(index))
, start("start" + sp::string(index))
{
    if (index == 0)
    {
        up.setKeys({"up", "keypad 8"});
        down.setKeys({"down", "keypad 2"});
        left.setKeys({"left", "keypad 4"});
        right.setKeys({"right", "keypad 6"});
        
        jump.setKey("space");
        use.setKey("z");
        fire3.setKey("x");
        unknown3.setKey("c");
        unknown4.setKey("v");
        unknown5.setKey("b");
        
        start.setKey("1");
    }

    if (index == 1)
    {
        up.setKey("w");
        down.setKey("s");
        left.setKey("a");
        right.setKey("d");
        
        jump.setKey("q");
        use.setKey("e");
        fire3.setKey("r");
        unknown3.setKey("f");
        unknown4.setKey("t");
        unknown5.setKey("g");
        
        start.setKey("2");
    }
}

sp::Vector2d Controls::getMovement()
{
    return sp::Vector2d(right.getValue() - left.getValue(), up.getValue() - down.getValue());
}
