#include "Move.h"


Move2::Move2() : Amarillo()
{

}

void Move2::Start()
{

}

void Move2::Update()
{
    //El scripting funciona pero se caga encima con las librerias
    //gameObject->transform->SetLocalPosition({ 6,9,8 });
}


AMARILLO_FACTORY Move2* CreateMove2() {
    Move2* move = new Move2();
    return move;
}