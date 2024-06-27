#pragma once
#include "Module.h"

#include "../External/mono/utils/mono-publib.h"
#include "../External/MathGeoLib/include/Math/float3.h"

#include "../External/mono/metadata/object-forward.h"
#include "../External/mono/metadata/object.h"
#include "../External/mono/metadata/blob.h"
#include "../External/mono/metadata/threads.h"
#include <vector>

typedef struct _MonoDomain MonoDomain;
typedef struct _MonoAssembly MonoAssembly;
typedef struct _MonoClassField MonoClassField;

class GameObject;
class CScript;

#define USER_SCRIPTS_NAMESPACE ""
#define AMARILLO_SCRIPTS_NAMESPACE "AmarilloEngine"

union FieldValue
{
	int iValue;
	float fValue;
	bool bValue;
	char strValue[50];
	GameObject* goValue;
};
struct SerializedField
{
	SerializedField();
	SerializedField(MonoClassField* _field, MonoObject* _object, CScript* parent);

	MonoClassField* field;
	MonoTypeEnum type = MonoTypeEnum::MONO_TYPE_END;
	FieldValue fiValue;
	CScript* parentSC;
	std::string displayName;
	uint goUID;
};

class ModuleScripting : public Module
{
public:
	ModuleScripting(Application* app, bool start_enabled = true);
	virtual ~ModuleScripting();

	bool Init() override;
	void OnGUI();
	bool CleanUp() override;

	static Quat UnboxQuat(MonoObject* _obj);
	void DebugAllFields(const char* className, std::vector<SerializedField>& _data, MonoObject* obj, CScript* script, const char* namespace_name);
	static float3 UnboxVector(MonoObject* _obj);

	static void LoadFieldData(SerializedField& _field, MonoObject* _object);
	static void DebugAllMethods(const char* nsName, const char* className, std::vector<std::string>& _data);


	void CreateAssetsScript(const char* localPath);
	void AddScriptToSLN(const char* scriptLocalPath);
	void RemoveScriptFromSLN(const char* scriptLocalPath);

	GameObject* GameObject_From_CSGO(MonoObject* goObj);

	MonoObject* QuatToCS(Quat& inVec) const;
	MonoObject* Float3ToCS(float3& inVec) const;
	MonoObject* GoToCSGO(GameObject* inGo) const;

	void ReCompileCS();

private:
	void InitMono();

public:
	MonoDomain* domain;
	MonoDomain* jitDomain;
	MonoAssembly* assembly;
	MonoImage* image;
	MonoThread* domainThread;

	std::vector<MonoClass*> userScripts;

};