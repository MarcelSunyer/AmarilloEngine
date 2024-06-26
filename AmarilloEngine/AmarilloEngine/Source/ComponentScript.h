#pragma once

#include "Component.h"
#include "Functions.h"

#include<vector>
#include<string>

#include "ComponentTransform.h"

#include "../External/mono/metadata/object-forward.h"
#include "ModuleScripting.h"
#include "../External/mono/metadata/class.h"

class GameObject;

#define MAX_NAME_CHARACTERS 32

class CScript : public Component {
public:

	CScript(GameObject* _gm, const char* scriptName);
	virtual ~CScript();

	void Update() override;

	void ReloadComponent();

	void OnRecursiveUIDChange(std::map<uint, GameObject*> gameObjects);

	void OnEditor() override;

	//void SaveData(JSON_Object* nObj);
	//void LoadData(DEConfig& nObj);

	void SetField(MonoClassField* field, GameObject* value);
	void DropField(SerializedField& fieldName, const char* dropType);

	void LoadScriptData(const char*);

public:

	std::vector<std::string> methods;
	std::vector<SerializedField> fields;

	MonoMethod* updateMethod;
	uint32_t noGCobject;
	std::string name;

	static CScript* runningScript;
};
