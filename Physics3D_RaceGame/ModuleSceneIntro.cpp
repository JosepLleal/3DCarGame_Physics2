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

	CreateCircuit(13, 3, 60, 0, 5, 35, 0, { 0, 1, 0 });
	CreateCircuit(13, 3, 30, -8.7, 5, 71, -45, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -37, 5, 79.5, 90, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -72, 5, 65, 45, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -105, 5, 52.5, 90, { 0, 1, 0 });
	CreateCircuit(13, 3, 130, -134, 5, 68.5 + 30 + 12.5, 0, { 0, 1, 0 });
	CreateCircuit(13, 3, 30, -142.75, 5, 97 + 55 + 30, -45, { 0, 1, 0 });
	CreateCircuit(13, 3, 30, -164, 5, 105.5 + 55 + 30, 90, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -190.25, 5, 91.5 + 55 + 30, 45, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -204.25, 5, 57.75 + 55 + 30, 0, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -190.25, 5, 24 + 55 + 30, -45, { 0, 1, 0 });
	CreateCircuit(45, 3, 13, -155, 11.5, 97.5, 17.5, { 0, 0, 1 });

	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (p2List_item<Cube*>* circuit_item = circuitList.getFirst(); circuit_item; circuit_item = circuit_item->next)
	{
		delete circuit_item->data;
		circuit_item->data = nullptr;
	}
	circuitList.clear();
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	Render_Circuit();

	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		if (App->player->enabled)
		{
			App->player->CleanUp();
			App->player->enabled = false;

			App->player2->enabled = true;
			App->player2->Start();
		}
		else if (App->player2->enabled)
		{
			App->player2->CleanUp();
			App->player2->enabled = false;

			App->player->enabled = true;
			App->player->Start();
		}

	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateCircuit(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, float angle, vec3 rotation, Color color)
{
	Cube* circuit = new Cube (sizeX, sizeY, sizeZ);
	circuit->SetPos(posX, posY, posZ);
	circuit->SetRotation(angle, rotation);
	circuit->color = color;
	App->physics->AddBody(*circuit, 0.0f);
	circuitList.add(circuit);

}

void ModuleSceneIntro::Render_Circuit()
{
	for (p2List_item<Cube*>* circuit_item = circuitList.getFirst(); circuit_item; circuit_item = circuit_item->next)
	{
		circuit_item->data->Render();
	}
}


