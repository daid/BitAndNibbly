#ifndef CONTROLS_H
#define CONTROLS_H

#include <sp2/io/keybinding.h>
#include <sp2/math/vector2.h>

class Controls
{
public:
    Controls(int index);

    sp::Vector2d getMovement();
    
    int index;
    
    sp::io::Keybinding up;
    sp::io::Keybinding down;
    sp::io::Keybinding left;
    sp::io::Keybinding right;

    sp::io::Keybinding jump;
    sp::io::Keybinding use;
    sp::io::Keybinding fire3;
    sp::io::Keybinding unknown3;
    sp::io::Keybinding unknown4;
    sp::io::Keybinding unknown5;
    
    sp::io::Keybinding start;
    
    static sp::io::Keybinding quit;
};

extern Controls player_controllers[2];

#endif//CONTROLS_H
