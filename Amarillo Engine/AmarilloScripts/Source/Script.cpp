//TODO: Write a script that do things, like moves a GameObject
#include "Script.h"

Move2::Move2() : Amarillo()
{

}

Move2::~Move2()
{

}

void Move2::Start()
{
	//time2 = ModuleEditor::elapsed_time;		//TODO: Arreglar el contador de tiempo

	if (test_object != nullptr) {
		LOG(test_object->mName.c_str());
	}
}

void Move2::Update()
{
	GameObject** untags = nullptr;
	GameObject** players = nullptr;
	GameObject** enemies = nullptr;
	uint s_untags = GameObject::FindGameObjectsWithTag("UnTagged", &untags);
	uint count = 0;
	for (uint i = 0; i < s_untags; ++i) {
		if (untags[i] != nullptr && untags[i]->active) {
			count++;
			LOG(untags[i]->mName.c_str());
		}
	}
	LOG("%i", count);
	GameObject::FreeArrayMemory((void***)&untags);

}

