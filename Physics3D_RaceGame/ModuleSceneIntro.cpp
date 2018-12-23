#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
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

	//Beggining
	CreateCircuit(13, 3, 60, 0, 5, 35, 0, { 0, 1, 0 });
	//U turn
	CreateCircuit(13, 3, 30, -8.7, 5, 71, -45, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -37, 5, 79.5, 90, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -72, 5, 65, 45, { 0, 1, 0 });
	//L
	CreateCircuit(13, 3, 45, -105, 5, 52.5, 90, { 0, 1, 0 });
	CreateCircuit(13, 3, 130, -134, 5, 111, 0, { 0, 1, 0 });
	//360 turn
	CreateCircuit(13, 3, 30, -142.75, 5, 182, -45, { 0, 1, 0 });
	CreateCircuit(13, 3, 30, -164, 5, 190.5, 90, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -190.25, 5, 175.5, 45, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -204.25, 5, 142.75, 0, { 0, 1, 0 });
	CreateCircuit(13, 3, 45, -190.25, 5, 109, -45, { 0, 1, 0 });
	//Ramp
	CreateCircuit(45, 3, 13, -155, 11.5, 97.5, 17.5, { 0, 0, 1 });
	//After ramp wider circuit for easier landing
	CreateCircuit(100, 3, 20, -35, 11.5, 97.5, 0, { 0, 1, 0 });
	//U turn
	CreateCircuit(50, 3, 13, 21.5, 11.5, 112.5, 90, { 0, 1, 0 });
	CreateCircuit(30, 3, 13, 30, 11.5, 144, 0, { 0, 1, 0 });
	CreateCircuit(50, 3, 13, 51.5, 11.5, 125.5, 90, { 0, 1, 0 });
	//L
	CreateCircuit(13, 3, 30, 73, 11.5, 107, 90, { 0, 1, 0 });
	CreateCircuit(13, 3, 30, 94.5, 11.5, 98.5, 0, { 0, 1, 0 });
	//Zigzag
	CreateCircuit(13, 3, 45.5, 84, 11.5, 67, 30, { 0, 1, 0 });
	CreateCircuit(13, 3, 45.5, 84, 11.5, 34.1, -30, { 0, 1, 0 });
	CreateCircuit(13, 3, 45.5, 84, 11.5, 1.2, 30, { 0, 1, 0 });
	//U turn
	CreateCircuit(13, 3, 30, 73.5, 11.5, -30.25, 0, { 0, 1, 0 });
	CreateCircuit(40, 3, 13, 60, 11.5, -51.75, 0, { 0, 1, 0 });
	CreateCircuit(60, 3, 13, 23.5, 11.5, -51.75, 0, { 0, 1, 0 });
	//Last ramp
	CreateCircuit(13, 3, 35, 0, 13, -30, -5.5, { 1, 0, 0 });

	//TESTING SENSORS ---------------------------
	Cube* First_Sens = new Cube(3,3,3);
	First_Sens->SetPos(0, 7, 20);
	First_Sens->color = Green;

	c_sensor1 = First_Sens;
	sensor1 = App->physics->AddBody(*c_sensor1, 0.0f);
	sensor1->SetAsSensor(true);
	sensor1->collision_listeners.add(this);
	//--------------------------------------------
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
	if (body1 = sensor1)
	{
		App->player->respawn = true;
	}
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


