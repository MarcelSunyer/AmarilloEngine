#include "Move.h"


Move2::Move2() : Amarillo()
{

}

void Move2::Start()
{

}

void Move2::Update()
{

}

AMARILLO_FACTORY Move2* CreateMove2() {
    Move2* move = new Move2();
    return move;
}