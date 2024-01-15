//TODO: Write a script that do things, like moves a GameObject
#ifndef _MOVE_H_
#define _MOVE_H_

#include "../../Amarillo Engine/Source/Amarillo.h"
#include "AmarilloScripts.h"

class AMARILLO_ENGINE_API Move2 : public Amarillo {

public:

	Move2();

	virtual ~Move2();


	void Start();
	void Update();

	void Test() override;

	int input_int_test = 0;
	int drag_int_test = 1;
	float input_float_test = 0.34F;
	float drag_float_test = 10.4F;
	int slider_int_test = 10;
	float slider_float_test = 10.5F;
	bool jump = false;
	GameObject* bullet;
	GameObject* test_object = nullptr;
	Uint32 time2 = 0;
};

AMARILLO_FACTORY Move2* CreateMove2() {
	Move2* move = new Move2();
	//SHOW_IN_INSPECTOR_AS_INPUT_INT(move->input_int_test);
	//SHOW_IN_INSPECTOR_AS_DRAGABLE_INT(move->drag_int_test);
	//SHOW_IN_INSPECTOR_AS_INPUT_FLOAT(move->input_float_test);
	//SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(move->drag_float_test);
	//SHOW_IN_INSPECTOR_AS_SLIDER_INT(move->slider_int_test, 0, 20);
	//SHOW_IN_INSPECTOR_AS_SLIDER_FLOAT(move->slider_float_test, 1.5F, 13.5F);
	//SHOW_IN_INSPECTOR_AS_CHECKBOX_BOOL(move->jump);
	//SHOW_IN_INSPECTOR_AS_GAMEOBJECT(move->test_object);
	return move;
}

#endif // !_MOVE_H_


