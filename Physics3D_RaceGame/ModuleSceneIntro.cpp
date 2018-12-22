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

	CreateCircuit(60, 3, 2, -10, 1.5, 40, 90, { 0, 1, 0 });
	CreateCircuit(60, 3, 2, 10, 1.5, 40, 90, { 0, 1, 0 });

	CreateCircuit(30, 3, 2, 6, 1.5, 80, 75, { 0, 1, 0 });
	CreateCircuit(15, 3, 2, -12, 1.5, 77, 75, { 0, 1, 0 });
	

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


