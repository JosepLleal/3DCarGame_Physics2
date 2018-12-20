#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	CreateCircuit(20, 20, 20, 0, 0, 100, 30, { 0, 20, 0 });
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateCircuit(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, 
									 float angle, vec3 rotation, Color color)
{
	Cube circuit(sizeX, sizeY, sizeZ);
	circuit.SetPos(posX, posY, posZ);
	circuit.SetRotation(angle, rotation);
	circuit.color = color;
	App->physics->AddBody(circuit, 0.0f);
	circuitList.add(circuit);

}


