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

	App->audio->SetMusicVolume();
	App->audio->SetSfxVolume();

	//FX
	music = App->audio->PlayMusic("audio/map_music.ogg");
	
	car_accelerate = App->audio->LoadFx("audio/car_accelerate.wav");
	car_brake = App->audio->LoadFx("audio/car_brakes.wav");
	car_fall = App->audio->LoadFx("audio/car_fall.wav");
	win = App->audio->LoadFx("audio/win.wav");

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

	CreateCircuit(10, 4, 10, -100, 2, -80, 0, { 1, 0, 0 });
	CreateCircuit(10, 7, 10, -110, 3.5, -80, 0, { 1, 0, 0 });

	CreateObstacles();

	//CHECK POINTS ---------------------------
	Cube* First_Sens = new Cube(13,10,1);
	First_Sens->SetPos(0, 10, 20);
	First_Sens->color = Green;

	c_sensor1 = First_Sens;
	sensor1 = App->physics->AddBody(*c_sensor1, 0.0f);
	sensor1->SetAsSensor(true);
	sensor1->collision_listeners.add(this);
	//---
	Cube* Second_Sens = new Cube(13, 5, 1);
	Second_Sens->SetPos(-134, 10, 111);
	Second_Sens->color = Green;

	c_sensor2 = Second_Sens;
	sensor2 = App->physics->AddBody(*c_sensor2, 0.0f);
	sensor2->SetAsSensor(true);
	sensor2->collision_listeners.add(this);
	//--
	Cube* Third_Sens = new Cube(1, 5, 16);
	Third_Sens->SetPos(-35, 15.5, 97.5);
	Third_Sens->color = Green;

	c_sensor3 = Third_Sens;
	sensor3 = App->physics->AddBody(*c_sensor3, 0.0f);
	sensor3->SetAsSensor(true);
	sensor3->collision_listeners.add(this);
	//--
	Cube* Fourth_Sens = new Cube(13, 5, 1);
	Fourth_Sens->SetPos(73.5, 15.5, -30.25);
	Fourth_Sens->color = Green;

	c_sensor4 = Fourth_Sens;
	sensor4 = App->physics->AddBody(*c_sensor4, 0.0f);
	sensor4->SetAsSensor(true);
	sensor4->collision_listeners.add(this);
	//--------------------------------------------
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	//Track clean up
	for (p2List_item<Cube*>* circuit_item = circuitList.getFirst(); circuit_item; circuit_item = circuit_item->next)
	{
		delete circuit_item->data;
		circuit_item->data = nullptr;
	}
	circuitList.clear();
	//Obstacle clean up
	for (p2List_item<Cube*>* obstacle_item = obstacleList.getFirst(); obstacle_item; obstacle_item = obstacle_item->next)
	{
		delete obstacle_item->data;
		obstacle_item->data = nullptr;
	}
	obstacleList.clear();
	

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	if (App->physics->debug)
	{
		c_sensor1->Render();
		c_sensor2->Render();
		c_sensor3->Render();
		c_sensor4->Render();
	}

	Render_Circuit();
	Render_Obstacles(body_physbody, obstacleBody);
	Render_Obstacles(body_physbody1, obstacleBody1);
	Render_Obstacles(body_physbody2, obstacleBody2);
	Render_Obstacles(body_physbody3, obstacleBody3);



	LOG("Player 1 = %i", chrono_player1);
	LOG("Player 2 = %i", chrono_player2);
	
	TimeSet(chrono.Read());
	
	if (ended)
	{
		if (winner == 1)
		{
			/*App->audio->PlayFx(App->scene_intro->win);*/
			TimeSet(chrono_player1);
		}
		if (winner == 2)
		{
			/*App->audio->PlayFx(App->scene_intro->win);*/
			TimeSet(chrono_player2);
		}
		
	}

	
	/*if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		ended = !ended;
	}*/
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1 == sensor1)
	{
		if (App->player->active)
		{
			if (App->player->reached_CP4 == false)
			{
				App->player->actual_checkpoint = CheckPoint::First_CP;
				
			}
			else
			{
				if (ended == false)
				{
					startTimer = false;
					chrono_player1 = chrono.Read();
					
					App->player->CleanUp();
					App->player->enabled = false;

					App->player2->enabled = true;
					App->player2->Start();
				}
				
			}
			
			
		}
		else
		{
			if (App->player2->reached_CP4 == false)
			{
				App->player2->actual_checkpoint = CheckPoint::First_CP;
			}
			else
			{
				if (ended == false)
				{
					chrono_player2 = chrono.Read();
					chrono.Stop();
				}

				if (chrono_player1 < chrono_player2)
				{
					winner = 1;
				}
				else if (chrono_player1 > chrono_player2)
				{
					winner = 2;
				}

				ended = true;

				App->player->enabled = true;
				App->player->Start();
				App->player2->Start();

				
			}
			
		}
		
	}
	else if (body1 == sensor2)
	{
		if (App->player->active)
		{
			App->player->actual_checkpoint = CheckPoint::Second_CP;
		}
		else
		{
			App->player2->actual_checkpoint = CheckPoint::Second_CP;
		}
	}
	else if (body1 == sensor3)
	{
		if (App->player->active)
		{
			App->player->actual_checkpoint = CheckPoint::Third_CP;
		}
		else
		{
			App->player2->actual_checkpoint = CheckPoint::Third_CP;
		}
	}
	else if (body1 == sensor4)
	{
		if (App->player->active)
		{
			App->player->actual_checkpoint = CheckPoint::Fourth_CP;
			App->player->reached_CP4 = true;
		}
		else
		{
			App->player2->actual_checkpoint = CheckPoint::Fourth_CP;
			App->player2->reached_CP4 = true;
		}
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
void ModuleSceneIntro::CreateObstacles()
{
	//first obstacle
	obstacleAnchor = new Cube(1, 1, 1);
	obstacleAnchor->SetPos(-134, 7, 131);
	anchor_physbody = App->physics->AddBody(*obstacleAnchor, 0.0f);

	obstacleBody = new  Cube(1, 1, 1);
	obstacleBody->SetPos(-134, 7, 135);
	obstacleBody->color = Red;
	obstacleList.add(obstacleBody);
	body_physbody = App->physics->AddBody(*obstacleBody, 10.0f);
	App->physics->StartHingeMotion(anchor_physbody, body_physbody);
	
	//second obstacle
	obstacleAnchor1 = new Cube(1, 1, 1);
	obstacleAnchor1->SetPos(-204.25, 7, 142.75);
	anchor_physbody1 = App->physics->AddBody(*obstacleAnchor1, 0.0f);

	obstacleBody1 = new  Cube(1, 1, 1);
	obstacleBody1->SetPos(-204.25, 7, 146.75);
	obstacleBody1->color = Red;
	obstacleList.add(obstacleBody1);
	body_physbody1 = App->physics->AddBody(*obstacleBody1, 10.0f);

	App->physics->StartHingeMotion(anchor_physbody1, body_physbody1);

	//third obstacle
	obstacleAnchor2 = new Cube(1, 1, 1);
	obstacleAnchor2->SetPos(94.5, 13.5, 98.5);
	anchor_physbody2 = App->physics->AddBody(*obstacleAnchor2, 0.0f);

	obstacleBody2 = new  Cube(1, 1, 1);
	obstacleBody2->SetPos(94.5, 13.5, 102.5);
	obstacleBody2->color = Red;
	obstacleList.add(obstacleBody2);
	body_physbody2 = App->physics->AddBody(*obstacleBody2, 10.0f);

	App->physics->StartHingeMotion(anchor_physbody2, body_physbody2);

	//fourth obstacle
	obstacleAnchor3 = new Cube(1, 1, 1);
	obstacleAnchor3->SetPos(84, 13.5, 34.1);
	anchor_physbody3 = App->physics->AddBody(*obstacleAnchor3, 0.0f);

	obstacleBody3 = new  Cube(1, 1, 1);
	obstacleBody3->SetPos(84, 13.5, 38.1);
	obstacleBody3->color = Red;
	obstacleList.add(obstacleBody3);
	body_physbody3 = App->physics->AddBody(*obstacleBody3, 10.0f);

	App->physics->StartHingeMotion(anchor_physbody3, body_physbody3);

}

void ModuleSceneIntro::TimeSet(uint timer)
{
	if (!startTimer)
		chrono.Start();

	char title[80];
	

	int sec = timer / 1000.0f; //from milisecond to sec
	int min = sec / 60.0f; //from seconds to minutes 
	int hour = min / 60.0f; //from minutes to hours


	int sec_print = sec;

	if (min > 0)
		sec_print -= min * 60;

	if (!ended)
	{
		if (hour >= 10 && min >= 10 && sec_print >= 10) {
			sprintf_s(title, "%i : %i : %i", hour, min, sec_print);
		}
		else if (hour < 10 && min >= 10 && sec_print >= 10) {
			sprintf_s(title, "0%i : %i : %i", hour, min, sec_print);
		}
		else if (hour >= 10 && min < 10 && sec_print >= 10) {
			sprintf_s(title, "%i : 0%i : %i", hour, min, sec_print);
		}
		else if (hour >= 10 && min >= 10 && sec_print < 10) {
			sprintf_s(title, "%i : %i : 0%i", hour, min, sec_print);
		}
		else if (hour >= 10 && min < 10 && sec_print < 10) {
			sprintf_s(title, "%i : 0%i : 0%i", hour, min, sec_print);
		}
		else if (hour < 10 && min >= 10 && sec_print < 10) {
			sprintf_s(title, "0%i : %i : 0%i", hour, min, sec_print);
		}
		else if (hour < 10 && min < 10 && sec_print >= 10) {
			sprintf_s(title, "0%i : 0%i : %i", hour, min, sec_print);
		}
		else {
			sprintf_s(title, "0%i : 0%i : 0%i", hour, min, sec_print);
		}

		if (startTimer)
			App->window->SetTitle(title);

	}
	else 
	{
		if (winner == 2)
		{
			if (hour >= 10 && min >= 10 && sec_print >= 10) {
				sprintf_s(title, "The winner is Player 2 with a time of %i : %i : %i", hour, min, sec_print);
			}
			else if (hour < 10 && min >= 10 && sec_print >= 10) {
				sprintf_s(title, "The winner is Player 2 with a time of 0%i : %i : %i", hour, min, sec_print);
			}
			else if (hour >= 10 && min < 10 && sec_print >= 10) {
				sprintf_s(title, "The winner is Player 2 with a time of %i : 0%i : %i", hour, min, sec_print);
			}
			else if (hour >= 10 && min >= 10 && sec_print < 10) {
				sprintf_s(title, "The winner is Player 2 with a time of %i : %i : 0%i", hour, min, sec_print);
			}
			else if (hour >= 10 && min < 10 && sec_print < 10) {
				sprintf_s(title, "The winner is Player 2 with a time of %i : 0%i : 0%i", hour, min, sec_print);
			}
			else if (hour < 10 && min >= 10 && sec_print < 10) {
				sprintf_s(title, "The winner is Player 2 with a time of 0%i : %i : 0%i", hour, min, sec_print);
			}
			else if (hour < 10 && min < 10 && sec_print >= 10) {
				sprintf_s(title, "The winner is Player 2 with a time of 0%i : 0%i : %i", hour, min, sec_print);
			}
			else {
				sprintf_s(title, "The winner is Player 2 with a time of 0%i : 0%i : 0%i", hour, min, sec_print);
			}
			App->window->SetTitle(title);
		}
		else if (winner == 1)
		{
			if (hour >= 10 && min >= 10 && sec_print >= 10) {
				sprintf_s(title, "The winner is Player 1 with a time of %i : %i : %i", hour, min, sec_print);
			}
			else if (hour < 10 && min >= 10 && sec_print >= 10) {
				sprintf_s(title, "The winner is Player 1 with a time of 0%i : %i : %i", hour, min, sec_print);
			}
			else if (hour >= 10 && min < 10 && sec_print >= 10) {
				sprintf_s(title, "The winner is Player 1 with a time of %i : 0%i : %i", hour, min, sec_print);
			}
			else if (hour >= 10 && min >= 10 && sec_print < 10) {
				sprintf_s(title, "The winner is Player 1 with a time of %i : %i : 0%i", hour, min, sec_print);
			}
			else if (hour >= 10 && min < 10 && sec_print < 10) {
				sprintf_s(title, "The winner is Player 1 with a time of %i : 0%i : 0%i", hour, min, sec_print);
			}
			else if (hour < 10 && min >= 10 && sec_print < 10) {
				sprintf_s(title, "The winner is Player 1 with a time of 0%i : %i : 0%i", hour, min, sec_print);
			}
			else if (hour < 10 && min < 10 && sec_print >= 10) {
				sprintf_s(title, "The winner is Player 1 with a time of 0%i : 0%i : %i", hour, min, sec_print);
			}
			else {
				sprintf_s(title, "The winner is Player 1 with a time of 0%i : 0%i : 0%i", hour, min, sec_print);
			}
			App->window->SetTitle(title);
		}
	}
}

void ModuleSceneIntro::Render_Circuit()
{
	for (p2List_item<Cube*>* circuit_item = circuitList.getFirst(); circuit_item; circuit_item = circuit_item->next)
	{
		circuit_item->data->Render();
	}
}

void ModuleSceneIntro::Render_Obstacles(PhysBody3D * body, Cube * cube)
{
	mat4x4 matrix;
	
	body->GetTransform(matrix.M);
	cube->transform = matrix;
	cube->Render();
	
}


