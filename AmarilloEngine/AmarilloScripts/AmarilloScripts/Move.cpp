#include "Move.h"


Move2::Move2() : Amarillo()
{

}

void Move2::Update()
{
    std::string pedro = "oi";
    unsigned int josete = sizeof(GameObject);
    gameObject->mName = josete;
}

AMARILLO_FACTORY Move2* CreateMove2() {
    Move2* move = new Move2();
    return move;
}