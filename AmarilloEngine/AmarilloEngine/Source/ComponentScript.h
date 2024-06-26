#ifndef COMPONENT_SCRIPT	
#define COMPONENT_SCRIPT

#include "Component.h"
#include "Functions.h"

#include "ComponentTransform.h"

#include "../AmarilloEngine/External/mono/metadata/object-forward.h"

class GameObject;

#define MAX_NAME_CHARACTERS 32

extern "C" __declspec(dllexport) class ComponentScript : public Component {
public:
	ComponentScript(GameObject* attach);
	~ComponentScript();

public:

	void Enable() override;
	void Start() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void OnStartPlay() override;


	void Reset();

	//TODO: Adapt this to the FileSystem code
	//void SaveComponent(JSONArraypack* to_save);
	//void LoadComponent(JSONArraypack* to_load);

public:

private:

	void CreateScriptInstance();

public:

	bool need_engine = false;
	std::string data_name;

	std::string script_name;
	char newName[MAX_NAME_CHARACTERS];
};


#endif // COMPONENT_SCRIPT
