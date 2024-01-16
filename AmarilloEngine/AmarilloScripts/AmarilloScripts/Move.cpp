#include "Move.h"


Move2::Move2() : Amarillo()
{

}

void Move2::Start()
{

}

void Move2::Update()
{
    float3 juan = { 4,2,3 };
    gameObject->transform->SetLocalPosition(juan);
}

AMARILLO_FACTORY Move2* CreateMove2() {
    Move2* move = new Move2();
    return move;
}