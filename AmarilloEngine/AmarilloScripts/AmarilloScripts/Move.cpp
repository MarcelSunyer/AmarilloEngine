#include "Move.h"


Move2::Move2() : Amarillo()
{

}

bool Move2::Test()
{

    return true;
}

extern "C" __declspec(dllexport) Move2* CreateMove2() {
    Move2* move = new Move2();
    return move;
}