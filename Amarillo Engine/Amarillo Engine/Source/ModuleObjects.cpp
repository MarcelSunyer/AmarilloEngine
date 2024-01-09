#include "ModuleObjects.h"

#include "Globals.h"
#include "ModuleObjects.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "../External/Glew/include/glew.h"
#include "Application.h"
#include <algorithm>
#include "ComponentTransform.h"
#include "ComponentTexture.h"
//#include "ResourceScene.h"		//TODO
#include "ComponentMesh.h"
#include "Time.h"
//#include "Prefab.h"				//TODO
#include "ResourcePrefab.h"
#include "ModuleRenderer3D.h"
#include "ComponentScript.h"
#include "Amarillo.h"
#include <filesystem>
//#include "ResourceScript.h"		//TODO
//#include "mmgr/mmgr.h"			//TODO? No se muy bien que es

ModuleObjects::ModuleObjects(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	app = app;
}

ModuleObjects::~ModuleObjects()
{

}

bool ModuleObjects::Init()
{
	tags.push_back(std::string("UnTagged"));

	base_game_object = new GameObject();
	base_game_object->UID = "0";

	return true;
}

bool ModuleObjects::Start()
{
	LOG("Starting Module Objects");
	bool ret = true;

	if (App->file_system->FileExists(FILE_TAGS)) {
		JSON_Value* value = json_parse_file(FILE_TAGS);
		JSON_Object* object = json_value_get_object(value);

		if (value != nullptr && object != nullptr)
		{
			JSON_Doc* json_tags = new JSON_Doc(value, object);
			JSON_Arraypack* curr_tags = json_tags->GetArray("Tags");
			for (uint i = 0; i < curr_tags->GetArraySize(); ++i) {
				tags.push_back(curr_tags->GetString("Tag"));
				curr_tags->GetAnotherNode();
			}
			delete json_tags;
		}
	}

	//App->scene->LoadScene("Assigment3TankScene");		//TODO: Create Assigment3TankScene with prefabs & scripts


	return ret;
}

update_status ModuleObjects::PreUpdate(float dt)
{
	// delete objects
	if (need_to_delete_objects) {
		need_to_delete_objects = false;
		base_game_object->SearchToDelete();
	}

	// change parent
	if (!to_reparent.empty()) {
		std::vector< std::tuple<GameObject*, GameObject*, bool>>::iterator item = to_reparent.begin();
		for (; item != to_reparent.end(); ++item) {
			if (std::get<0>(*item) != nullptr && std::get<1>(*item) != nullptr) {
				std::get<0>(*item)->SetParent(std::get<1>(*item));
			}
		}
		to_reparent.clear();
	}

	if (!invokes.empty()) {
		std::list<InvokeInfo*> invokes_list = invokes;
		auto item = invokes_list.begin();
		for (; item != invokes_list.end(); ++item) {
			if ((*item)->time_started + (*item)->time_to_wait < App->editor->elapsed_time) {
				(*item)->function();
				if (!(*item)->is_repeating) {
					invokes.remove(*item);
					delete* item;
					*item = nullptr;
				}
				else {
					(*item)->time_started = App->editor->elapsed_time;
					(*item)->time_to_wait = (*item)->time_between;
				}
			}
		}
	}

	ScriptsPreUpdate();
	return UPDATE_CONTINUE;
}

update_status ModuleObjects::Update(float dt)
{
	ScriptsUpdate();
	return UPDATE_CONTINUE;
}

update_status ModuleObjects::PostUpdate(float dt)
{
	ScriptsPostUpdate();
//	if (App->renderer3D->SetCameraToDraw(App->camera->fake_camera)) {
//		printing_scene = true;
//		// Scene Drawing
//		if (App->renderer3D->render_zbuffer) {
//			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->z_framebuffer);
//		}
//		else {
//			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->scene_frame_buffer);
//		}
//
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//		glClearStencil(0);
//
//		if (draw_ray)
//			DrawRay();
//
//		if (allow_grid)
//			App->renderer3D->RenderGrid();
//
//		if (render_octree)
//			octree.Draw();
//
//		if (base_game_object->HasChildren()) {
//			std::vector<std::pair<float, GameObject*>> to_draw;
//
//			ComponentCamera* frustum_camera = nullptr;
//
//			if (!check_culling_in_scene)
//			{
//				frustum_camera = App->camera->fake_camera;
//			}
//			else if (check_culling_in_scene && App->renderer3D->actual_game_camera != nullptr)
//			{
//				frustum_camera = App->renderer3D->actual_game_camera;
//			}
//			else {
//				frustum_camera = App->camera->fake_camera;
//			}
//
//			octree.SetStaticDrawList(&to_draw, frustum_camera);
//
//			std::vector<GameObject*>::iterator item = base_game_object->children.begin();
//			for (; item != base_game_object->children.end(); ++item) {
//				if (*item != nullptr && (*item)->IsEnabled()) {
//					(*item)->SetDrawList(&to_draw, frustum_camera);
//				}
//			}
//
//			if (prefab_scene) {
//				glEnable(GL_LIGHT0);
//				static Color ambient{ 0.5f, 0.5f, 0.5f, 1.0f };
//				static Color diffuse{ 0.75f, 0.75f, 0.75f, 1.0f };
//				glLightfv(GL_LIGHT0, GL_POSITION, float3(0, 15, -40).ptr());
//				glLightfv(GL_LIGHT0, GL_AMBIENT, &ambient);
//				glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuse);
//			}
//			std::sort(to_draw.begin(), to_draw.end(), ModuleObjects::SortGameObjectToDraw);
//			std::vector<std::pair<float, GameObject*>>::iterator it = to_draw.begin();
//			for (; it != to_draw.end(); ++it) {
//				if ((*it).second != nullptr) {
//					(*it).second->DrawScene();
//				}
//			}
//			OnDrawGizmos();
//		}
//
//		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//	}
//
//	if (App->renderer3D->SetCameraToDraw(App->renderer3D->actual_game_camera)) {
//		printing_scene = false;
//		if (App->renderer3D->render_zbuffer) {
//			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->z_framebuffer);
//		}
//		else {
//			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->game_frame_buffer);
//		}
//
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//		glClearStencil(0);
//
//		if (allow_grid)
//			App->renderer3D->RenderGrid();
//
//		if (base_game_object->HasChildren()) {
//
//			OnPreCull(App->renderer3D->actual_game_camera);
//			std::vector<std::pair<float, GameObject*>> to_draw;
//			octree.SetStaticDrawList(&to_draw, App->renderer3D->actual_game_camera);
//
//			std::vector<GameObject*>::iterator item = base_game_object->children.begin();
//			for (; item != base_game_object->children.end(); ++item) {
//				if (*item != nullptr && (*item)->IsEnabled()) {
//					(*item)->SetDrawList(&to_draw, App->renderer3D->actual_game_camera);
//				}
//			}
//
//			std::sort(to_draw.begin(), to_draw.end(), ModuleObjects::SortGameObjectToDraw);
//
//			OnPreRender(App->renderer3D->actual_game_camera);
//			std::vector<std::pair<float, GameObject*>>::iterator it = to_draw.begin();
//			for (; it != to_draw.end(); ++it) {
//				if ((*it).second != nullptr) {
//					(*it).second->DrawGame();
//				}
//			}
//
//			OnPostRender(App->renderer3D->actual_game_camera);
//		}
//		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//	}
//
//	if (App->renderer3D->selected_game_camera != nullptr && (App->objects->GetSelectedObjects().size() == 1 && App->renderer3D->actual_game_camera != App->objects->GetSelectedObjects().back()->GetComponent(ComponentType::CAMERA) && App->renderer3D->SetCameraToDraw(App->renderer3D->selected_game_camera)))
//	{
//		printing_scene = false;
//		if (App->renderer3D->render_zbuffer) {
//			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->z_framebuffer);
//		}
//		else {
//			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->sc_game_frame_buffer);
//		}
//
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//		glClearStencil(0);
//
//		if (allow_grid)
//			App->renderer3D->RenderGrid();
//
//		if (base_game_object->HasChildren()) {
//			std::vector<std::pair<float, GameObject*>> to_draw;
//
//			octree.SetStaticDrawList(&to_draw, App->renderer3D->selected_game_camera);
//
//			std::vector<GameObject*>::iterator item = base_game_object->children.begin();
//			for (; item != base_game_object->children.end(); ++item) {
//				if (*item != nullptr && (*item)->IsEnabled()) {
//					(*item)->SetDrawList(&to_draw, App->renderer3D->selected_game_camera);
//				}
//			}
//
//			std::sort(to_draw.begin(), to_draw.end(), ModuleObjects::SortGameObjectToDraw);
//			std::vector<std::pair<float, GameObject*>>::iterator it = to_draw.begin();
//			for (; it != to_draw.end(); ++it) {
//				if ((*it).second != nullptr) {
//					(*it).second->DrawGame();
//				}
//			}
//		}
//
//		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//	}

	return UPDATE_CONTINUE;
}

void ModuleObjects::DrawRay()					//TODO: Mirar si esto es necesario con nuestro sistema de camaras
{
	/*if (App->camera->ray.IsFinite()) {
		glColor3f(ray_color.r, ray_color.g, ray_color.b);
		glLineWidth(ray_width);
		glBegin(GL_LINES);

		glVertex3f(App->camera->ray.a.x, App->camera->ray.a.y, App->camera->ray.a.z);
		glVertex3f(App->camera->ray.b.x, App->camera->ray.b.y, App->camera->ray.b.z);

		glEnd();
		glLineWidth(1);
	}*/
}

bool ModuleObjects::CleanUp()
{
	delete base_game_object;
	base_game_object = nullptr;

	return true;
}

void ModuleObjects::DeleteAllObjects()
{
	std::vector<GameObject*>::iterator item = base_game_object->children.begin();
	while (item != base_game_object->children.end()) {
		if (*item != nullptr) {
			delete* item;
			*item = nullptr;
			item = base_game_object->children.erase(item);
		}
		else
			++item;
	}
	game_objects_selected.clear();
}

void ModuleObjects::SetNewSelectedObject(GameObject* object_selected)
{
	bool exists = std::find(game_objects_selected.begin(), game_objects_selected.end(), object_selected) != game_objects_selected.end();


	auto item = game_objects_selected.begin();
	for (; item != game_objects_selected.end(); ++item) {
		if (*item != object_selected) {
			(*item)->selected = false;
		}
	}
	game_objects_selected.clear();
	game_objects_selected.push_back(object_selected);
	if (!exists) {
		object_selected->selected = true;
	}

	//App->renderer3D->selected_game_camera = (ComponentCamera*)object_selected->GetComponent(ComponentTypes::CAMERA);		//TODO: Mirar como poder acceder al GO de la main camera
}

const std::list<GameObject*>& ModuleObjects::GetSelectedObjects()
{
	return game_objects_selected;
}

void ModuleObjects::DeselectObjects()
{
	auto item = game_objects_selected.begin();
	for (; item != game_objects_selected.end(); ++item) {
		(*item)->selected = false;
	}
	game_objects_selected.clear();
	//App->renderer3D->selected_game_camera = nullptr;			//TODO: Mirar como poder acceder al GO de la main camera
}

void ModuleObjects::DeselectObject(GameObject* obj)
{
	game_objects_selected.remove(obj);
	obj->selected = false;
}

void ModuleObjects::InitScriptsOnPlay() const
{
	// scripts awake
	std::list<Amarillo*>::const_iterator item = current_scripts.cbegin();
	for (; item != current_scripts.cend(); ++item) {
		if (*item != nullptr && (*item)->game_object != nullptr && (*item)->game_object->parent->active && (*item)->game_object->active && (*item)->IsScriptEnabled()) {
			try {
				(*item)->Awake();
			}
			catch (...)
			{
				try {
					LOG("CODE ERROR IN THE AWAKE OF THE SCRIPT: %s", (*item)->data_name);
				}
				catch (...) {
					LOG("UNKNOWN ERROR IN SCRIPTS AWAKE");
				}
			}
		}
	}
	// scripts start
	item = current_scripts.cbegin();
	for (; item != current_scripts.cend(); ++item) {
		if (*item != nullptr && (*item)->game_object != nullptr && (*item)->game_object->parent->active && (*item)->game_object->active && (*item)->IsScriptEnabled()) {
			try {
				(*item)->Start();
			}
			catch (...)
			{
				try {
					LOG("CODE ERROR IN THE START OF THE SCRIPT: %s", (*item)->data_name);
				}
				catch (...) {
					LOG("UNKNOWN ERROR IN SCRIPTS START");
				}
			}
		}
	}
}

void ModuleObjects::ScriptsPreUpdate() const
{
	if (App->editor->timerState == Timer_State::RUNNING && !current_scripts.empty()) {
		std::list<Amarillo*>::const_iterator item = current_scripts.cbegin();
		for (; item != current_scripts.cend(); ++item) {
			if (*item != nullptr && (*item)->game_object != nullptr && (*item)->game_object->parent->active && (*item)->game_object->active && (*item)->IsScriptEnabled()) {
				try {
					(*item)->PreUpdate();
				}
				catch (...)
				{
					try {
						LOG("CODE ERROR IN THE PREUPDATE OF THE SCRIPT: %s", (*item)->data_name);
					}
					catch (...) {
						LOG("UNKNOWN ERROR IN SCRIPTS PREUPDATE");
					}
				}
			}
		}
	}
}

void ModuleObjects::ScriptsUpdate() const
{
	if (App->editor->timerState == Timer_State::RUNNING && !current_scripts.empty()) {
		std::list<Amarillo*>::const_iterator item = current_scripts.cbegin();
		for (; item != current_scripts.cend(); ++item) {
			if (*item != nullptr && (*item)->game_object != nullptr && (*item)->game_object->parent->active && (*item)->game_object->active && (*item)->IsScriptEnabled()) {
				try {
					(*item)->Update();
				}
				catch (...)
				{
					try {
						LOG("CODE ERROR IN THE UPDATE OF THE SCRIPT: %s", (*item)->data_name);
					}
					catch (...) {
						LOG("UNKNOWN ERROR IN SCRIPTS UPDATE");
					}
				}
			}
		}
	}
}

void ModuleObjects::ScriptsPostUpdate() const
{
	if (App->editor->timerState == Timer_State::RUNNING && !current_scripts.empty()) {
		std::list<Amarillo*>::const_iterator item = current_scripts.cbegin();
		for (; item != current_scripts.cend(); ++item) {
			if (*item != nullptr && (*item)->game_object != nullptr && (*item)->game_object->parent->active && (*item)->game_object->active && (*item)->IsScriptEnabled()) {
				try {
					(*item)->PostUpdate();
				}
				catch (...)
				{
					try {
						LOG("CODE ERROR IN THE POSTUPDATE OF THE SCRIPT: %s", (*item)->data_name);
					}
					catch (...) {
						LOG("UNKNOWN ERROR IN SCRIPTS POSTUPDATE");
					}
				}
			}
		}
	}
}

void ModuleObjects::CleanUpScriptsOnStop() const
{
	std::list<Amarillo*>::const_iterator item = current_scripts.cbegin();
	for (; item != current_scripts.cend(); ++item) {
		if (*item != nullptr && (*item)->game_object != nullptr) {
			try {
				(*item)->CleanUp();
			}
			catch (...)
			{
				try {
					LOG("CODE ERROR IN THE CLEANUP OF THE SCRIPT: %s", (*item)->data_name);
				}
				catch (...) {
					LOG("UNKNOWN ERROR IN SCRIPTS CLEANUP");
				}
			}
		}
	}
}

//void ModuleObjects::OnPreCull(ComponentCamera* camera) const					//TODO: Mirar si esto hace falta
//{
//	if (camera != nullptr && camera->game_object_attached != nullptr) {
//		std::vector<ComponentScript*> script_cameras = camera->game_object_attached->GetComponents<ComponentScript>();
//		auto item = script_cameras.begin();
//		for (; item != script_cameras.end(); ++item) {
//			if (*item != nullptr && (*item)->need_engine && (*item)->data_ptr != nullptr) {
//				Amarillo* amarillo = (Amarillo*)(*item)->data_ptr;
//				if (amarillo != nullptr) {
//					try {
//						amarillo->OnPreCull();
//					}
//					catch (...)
//					{
//						try {
//							LOG("CODE ERROR IN THE ONPRECULL OF THE SCRIPT: %s", amarillo->data_name);
//						}
//						catch (...) {
//							LOG("UNKNOWN ERROR IN SCRIPTS ONPRECULL");
//						}
//					}
//				}
//			}
//		}
//	}
//}

//void ModuleObjects::OnPreRender(ComponentCamera* camera) const					//TODO: Mirar si esto hace falta
//{
//	if (camera != nullptr && camera->game_object_attached != nullptr) {
//		std::vector<ComponentScript*> script_cameras = camera->game_object_attached->GetComponents<ComponentScript>();
//		auto item = script_cameras.begin();
//		for (; item != script_cameras.end(); ++item) {
//			if (*item != nullptr && (*item)->need_engine && (*item)->data_ptr != nullptr) {
//				Amarillo* amarillo = (Amarillo*)(*item)->data_ptr;
//				if (amarillo != nullptr) {
//					try {
//						amarillo->OnPreRender();
//					}
//					catch (...)
//					{
//						try {
//							LOG("CODE ERROR IN THE ONPRERENDER OF THE SCRIPT: %s", amarillo->data_name);
//						}
//						catch (...) {
//							LOG("UNKNOWN ERROR IN SCRIPTS ONPRERENDER");
//						}
//					}
//				}
//			}
//		}
//	}
//}

//void ModuleObjects::OnPostRender(ComponentCamera* camera) const						//TODO: Mirar si esto hace falta
//{
//	if (camera != nullptr && camera->game_object_attached != nullptr) {
//		std::vector<ComponentScript*> script_cameras = camera->game_object_attached->GetComponents<ComponentScript>();
//		auto item = script_cameras.begin();
//		for (; item != script_cameras.end(); ++item) {
//			if (*item != nullptr && (*item)->need_engine && (*item)->data_ptr != nullptr) {
//				Amarillo* amarillo = (Amarillo*)(*item)->data_ptr;
//				if (amarillo != nullptr) {
//					try {
//						amarillo->OnPostRender();
//					}
//					catch (...)
//					{
//						try {
//							LOG("CODE ERROR IN THE ONPOSTRENDER OF THE SCRIPT: %s", amarillo->data_name);
//						}
//						catch (...) {
//							LOG("UNKNOWN ERROR IN SCRIPTS ONPOSTRENDER");
//						}
//					}
//				}
//			}
//		}
//	}
//}

GameObject* ModuleObjects::CreateEmptyGameObject(GameObject* parent, bool set_selected)
{
	GameObject* object = nullptr;

	if (parent != nullptr) {
		object = new GameObject();
		object->SetParent(parent);
		object->mName ="Empty Child";
		object->UID = App->resourceManager->NewGuid().toString();
	}
	else {
		object = new GameObject();
		object->SetParent(GetRoot(false));
		object->mName = "Empty GameObject";
		object->UID = App->resourceManager->NewGuid().toString();
	}

	if (set_selected)
		SetNewSelectedObject(object);

	return object;
}

GameObject* ModuleObjects::GetGameObjectByID(const std::string& id)
{
	return base_game_object->GetGameObjectByID(id);
}


void ModuleObjects::CreateEmptyScene(ResourceScene* scene)
{
	if (scene != nullptr) {
		delete base_game_object;
		game_objects_selected.clear();
		base_game_object = new GameObject();
		base_game_object->UID = "0";

		current_scene = scene;
	}
}


GameObject* ModuleObjects::GetRoot(bool ignore_prefab)
{
	if (prefab_scene && !ignore_prefab) {
		if (base_game_object->children.size() == 0)
			return base_game_object;
		else
			return base_game_object->children.back();
	}
	else
		return base_game_object;
}

void ModuleObjects::CreateRoot()
{
	delete base_game_object;
	game_objects_selected.clear();
	base_game_object = new GameObject();
	base_game_object->UID = "0";
}


void ModuleObjects::HotReload()
{
	JSON_Value* value = json_value_init_object();
	JSON_Object* json_object = json_value_get_object(value);
	json_serialize_to_file_pretty(value, "../Library/ScriptsTEMP.amarillo");

	if (value != nullptr && json_object != nullptr) {
		JSON_Doc* to_save = new JSON_Doc(value, json_object);		//TODO: Falta asignarle una ruta donde guardarlo IDK como
		to_save->StartSave();
		if (current_scripts.empty()) {
			to_save->SetBool("AreScripts", false);
		}
		else {
			to_save->SetBool("AreScripts", true);
			JSON_Arraypack* scripts = to_save->InitNewArray("Arr.Scripts");
			//CreateJsonScript(base_game_object, scripts);	//TODO: Descomentar cuando esté creado
		}
		to_save->FinishSave();
		delete to_save;
	}
	current_scripts.clear();
	if (FreeLibrary(App->scripts_dll)) {
		LOG("Dll correctly unloaded");
		if (remove(DLL_WORKING_PATH) == 0) {
			LOG("Dll correctly removed");
			App->resourceManager->ReloadScripts();
			// if this change is done without waiting, we cant move the file because the creating dll process hasn't finished
			// so I just wait until MoveFileA succed. 
			// If someone knows how to know when an extern process finishes with a file, tell me please contrasnya@gmail.com
			while (MoveFileA(DLL_CREATION_PATH, DLL_WORKING_PATH) == FALSE) {}
			LOG("New Dll correctly moved");
			App->scripts_dll = nullptr;
			App->scripts_dll = LoadLibrary(App->dll.data());
			if (App->scripts_dll != nullptr) {
				JSON_Value* value_load = json_parse_file("../Library/ScriptsTEMP.amarillo");
				JSON_Object* json_object_load = json_value_get_object(value_load);

				if (value_load != nullptr && json_object_load != nullptr) {
					JSON_Doc* to_load = new JSON_Doc("Library/ScriptsTEMP.amarillo", json_object_load, value_load);
					if (to_load->GetBool("AreScripts")) {
						JSON_Arraypack* scripts_to_load = to_load->GetArray("Arr.Scripts");
						//ReAssignScripts(scripts_to_load);	//TODO: Descomentar cuando esté creado
						if (App->editor->timerState == Timer_State::RUNNING) {
							auto item = current_scripts.begin();
							for (; item != current_scripts.end(); ++item) {
								if (*item != nullptr) {
									(*item)->Awake();
									(*item)->Start();
								}
							}
						}
					}
					remove("../Library/ScriptsTEMP.amarillo");

					delete to_load;
				}
			}
		}
	}
}


bool ModuleObjects::SortGameObjectToDraw(std::pair<float, GameObject*> first, std::pair<float, GameObject*> last)
{
	return first.first > last.first;
}

void ModuleObjects::AddScriptObject(const u64& ID, GameObject** object)
{
	to_add.push_back({ ID, object });
}

void ModuleObjects::AddInvoke(std::function<void()> void_no_params_function, const float& second, Amarillo* amarillo)
{
	InvokeInfo* info = new InvokeInfo();
	info->function = void_no_params_function;
	info->time_to_wait = second;
	info->amarillo = amarillo;
	info->is_repeating = false;
	info->time_started = App->editor->elapsed_time;
	info->ID = App->resourceManager->NewGuid().toString();

	invokes.push_back(info);
}

void ModuleObjects::AddInvokeRepeating(std::function<void()> void_no_params_function, const float& second, const float& seconds_between_each_call, Amarillo* amarillo)
{
	InvokeInfo* info = new InvokeInfo();
	info->function = void_no_params_function;
	info->time_to_wait = second;
	info->amarillo = amarillo;
	info->is_repeating = true;
	info->time_between = seconds_between_each_call;
	info->time_started = App->editor->elapsed_time;
	info->ID = App->resourceManager->NewGuid().toString();

	invokes.push_back(info);
}

void ModuleObjects::CancelInvokes(Amarillo* amarillo)
{
	auto invokes_list = invokes;
	auto item = invokes_list.begin();
	for (; item != invokes_list.end(); ++item) {
		if ((*item)->amarillo == amarillo) {
			invokes.remove(*item);
			delete* item;
			*item = nullptr;
		}
	}
}

//bool ModuleObjects::IsInvoking(std::function<void()> void_no_params_function)
//{
//	auto item = invokes.begin();
//	for (; item != invokes.end(); ++item) {
//		if ((*item). == CompareFunction<void>(void_no_params_function)) {
//			return true;
//		}
//	}
//	return false;
//}

//TODO: Arreglar esto de abajo

//void ModuleObjects::CreateJsonScript(GameObject* obj, JSONArraypack* to_save)
//{
//	if (obj->HasChildren()) {
//		std::vector<GameObject*>::iterator item = obj->children.begin();
//		for (; item != obj->children.end(); ++item) {
//			if (*item != nullptr) {
//				std::vector<ComponentScript*> scripts = (*item)->GetComponents<ComponentScript>();
//				if (!scripts.empty()) {
//					std::vector<ComponentScript*>::iterator script = scripts.begin();
//					for (; script != scripts.end(); ++script) {
//						if (*script != nullptr) {
//							to_save->SetAnotherNode();
//							to_save->SetString("GameObjectID", std::to_string((*item)->ID));
//							to_save->SetString("ResourceScriptID", std::to_string((*script)->resourceID));
//							to_save->SetString("DataName", (*script)->data_name);
//							to_save->SetString("CompScriptID", std::to_string((*script)->ID));
//							if ((*script)->inspector_variables.empty()) {
//								to_save->SetBoolean("HasInspector", false);
//							}
//							else {
//								to_save->SetBoolean("HasInspector", true);
//								JSONArraypack* inspector = to_save->InitNewArray("Inspector");
//								for (uint i = 0; i < (*script)->inspector_variables.size(); ++i) {
//									inspector->SetAnotherNode();
//									if ((*script)->inspector_variables[i].ptr == nullptr) {
//										inspector->SetBoolean("IsNull", true);
//										continue;
//									}
//									inspector->SetBoolean("IsNull", false);
//									inspector->SetString("Name", (*script)->inspector_variables[i].variable_name);
//									inspector->SetNumber("Type", (*script)->inspector_variables[i].variable_type);
//									switch ((*script)->inspector_variables[i].variable_type)
//									{
//									case InspectorScriptData::DataType::INT: {
//										inspector->SetNumber("int", (*(int*)((*script)->inspector_variables[i].ptr)));
//										break; }
//									case InspectorScriptData::DataType::FLOAT: {
//										inspector->SetNumber("float", (*(float*)((*script)->inspector_variables[i].ptr)));
//										break; }
//									case InspectorScriptData::DataType::BOOL: {
//										inspector->SetNumber("bool", (*(bool*)((*script)->inspector_variables[i].ptr)));
//										break; }
//									case InspectorScriptData::DataType::PREFAB: {
//										Prefab* prefab = ((Prefab*)((*script)->inspector_variables[i].ptr));
//										inspector->SetString("prefab", std::to_string(prefab->prefabID));
//										break; }
//									case InspectorScriptData::DataType::GAMEOBJECT: {
//										GameObject** obj = ((GameObject**)((*script)->inspector_variables[i].obj));
//										if (obj != nullptr && *obj != nullptr) {
//											inspector->SetString("gameobject", std::to_string((*obj)->prefabID));
//										}
//										else {
//											inspector->SetString("gameobject", "0");
//										}
//										break; }
//									default:
//										break;
//									}
//								}
//							}
//							(*item)->RemoveComponent(*script);	//Todo: RemoveComponent()
//						}
//					}
//				}
//				CreateJsonScript((*item), to_save);
//			}
//		}
//	}
//}
//
//void ModuleObjects::ReAssignScripts(JSONArraypack* to_load)
//{
//	for (uint i = 0; i < to_load->GetArraySize(); ++i) {
//		GameObject* obj = GetGameObjectByID(std::stoull(to_load->GetString("GameObjectID")));
//		if (obj == nullptr) {
//			to_load->GetAnotherNode();
//			continue;
//		}
//		ResourceScript* r_script = (ResourceScript*)App->resources->GetResourceWithID(std::stoull(to_load->GetString("ResourceScriptID")));
//		if (r_script == nullptr) {
//			to_load->GetAnotherNode();
//			continue;
//		}
//		std::string data_name = to_load->GetString("DataName");
//		for (uint k = 0; k < r_script->data_structures.size(); ++k) {
//			if (App->StringCmp(data_name.data(), r_script->data_structures[k].first.data())) {
//				ComponentScript* script = new ComponentScript(obj);
//				script->ID = std::stoull(to_load->GetString("CompScriptID"));
//				script->resourceID = r_script->GetID();
//				script->LoadData(data_name.data(), r_script->data_structures[k].second);
//				if (to_load->GetBoolean("HasInspector")) {
//					JSONArraypack* inspector = to_load->GetArray("Inspector");
//					for (uint j = 0; j < inspector->GetArraySize(); ++j) {
//						if (inspector->GetBoolean("IsNull")) {
//							inspector->GetAnotherNode();
//							continue;
//						}
//						std::vector<InspectorScriptData>::iterator item = script->inspector_variables.begin();
//						std::string var_name = inspector->GetString("Name");
//						InspectorScriptData::DataType type = (InspectorScriptData::DataType)(uint)inspector->GetNumber("Type");
//						for (; item != script->inspector_variables.end(); ++item) {
//							if (App->StringCmp((*item).variable_name.data(), var_name.data())) {
//								if (type == (*item).variable_type) {
//									switch (type) {
//									case InspectorScriptData::DataType::INT: {
//										*(int*)(*item).ptr = inspector->GetNumber("int");
//										break; }
//									case InspectorScriptData::DataType::FLOAT: {
//										*(float*)(*item).ptr = inspector->GetNumber("float");
//										break; }
//									case InspectorScriptData::DataType::BOOL: {
//										*(bool*)(*item).ptr = inspector->GetNumber("bool");
//										break; }
//									case InspectorScriptData::DataType::PREFAB: {
//										u64 id = std::stoull(inspector->GetString("prefab"));
//										if (id != 0) {
//											ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(id);
//											if (prefab != nullptr) {
//												Prefab* ins_prefab = (Prefab*)(*item).ptr;
//												ins_prefab->prefabID = prefab->GetID();
//												ins_prefab->prefab_name = std::string(prefab->GetName());
//												prefab->prefab_references.push_back(ins_prefab);
//											}
//										}
//										break; }
//									case InspectorScriptData::DataType::GAMEOBJECT: {
//										u64 id = std::stoull(inspector->GetString("gameobject"));
//										if (id != 0) {
//											*(*item).obj = GetGameObjectByID(id);
//										}
//										break; }
//									default:
//										break;
//									}
//								}
//							}
//						}
//						inspector->GetAnotherNode();
//					}
//				}
//			}
//		}
//		to_load->GetAnotherNode();
//	}
//}


GameObject* ModuleObjects::GetGameObjectByName(const std::string& targetName) {
	for (size_t i = 0; i < applic->scene->game_objects.size(); ++i) {
		GameObject* gameObject = applic->scene->game_objects[i];
		if (gameObject->mName == targetName) {
			return gameObject;
		}
	}

	return nullptr;
}