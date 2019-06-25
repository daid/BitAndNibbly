#ifndef LADDER_H
#define LADDER_H

#include <sp2/scene/node.h>


class Ladder : public sp::Node
{
public:
    Ladder(sp::P<sp::Node> parent, sp::Vector2d position, sp::Vector2d size);
    
private:
    sp::Vector2d size;
};

#endif//LADDER_H
