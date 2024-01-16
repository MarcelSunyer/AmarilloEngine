#include "Move.h"


Move2::Move2() : Amarillo()
{

}

void Move2::Update()
{
    std::string pedro = "oi";
    float3 juan = { 4,2,3 };
    gameObject->transform->local_position = juan;
}

AMARILLO_FACTORY Move2* CreateMove2() {
    Move2* move = new Move2();
    return move;
}