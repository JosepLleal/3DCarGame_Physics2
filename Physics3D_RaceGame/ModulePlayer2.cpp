#include "Globals.h"
#include "Application.h"
#include "ModulePlayer2.h"
#include "Primitive.h"
#include "ModulePhysics3D.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer2::ModulePlayer2(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer2::~ModulePlayer2()
{}

// Load assets
bool ModulePlayer2::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------

	car.car_color = Blue;

	car.chassis_size.Set(1, 0.75, 5);
	car.chassis_offset.Set(0, 1.75, 0);

	car.left_chassis_size.Set(0.5, 0.75, 2);
	car.left_chassis_offset.Set(0.75, 1.75, 0);

	car.right_chassis_size.Set(0.5, 0.75, 2);
	car.right_chassis_offset.Set(-0.75, 1.75, 0);

	car.front_chassis_size.Set(1, 0.5, 1.5);
	car.front_chassis_offset.Set(0, 1.6, 3);

	car.front_wing_size.Set(2.5, 0.1, 0.5);
	car.front_wing_offset.Set(0, 1.3, 3.5);

	car.back_wing_size.Set(3, 0.1, 1);
	car.back_wing_offset.Set(0, 2.5, -2);

	car.back_wingsupport_size.Set(0.1, 0.5, 0.5);
	car.back_wingsupport_offset.Set(0, 2.2, -2);

	car.back_wingright_size.Set(0.1, 0.5, 1);
	car.back_wingright_offset.Set(-1.5, 2.5, -2);

	car.back_wingleft_size.Set(0.1, 0.5, 1);
	car.back_wingleft_offset.Set(1.5, 2.5, -2);

	car.pilot.Set(1, 0.5, 1);
	car.pilot_offset.Set(0, 2.25, -0.5);

	car.mass = 300.0f;
	car.suspensionStiffness = 15.0f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.0f;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 2.75f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.75f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 2.0f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = false;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 2.0f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = false;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 2.0f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = true;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 2.0f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = true;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 7, 10);

	return true;
}

// Unload assets
bool ModulePlayer2::CleanUp()
{
	LOG("Unloading player");
	if(vehicle!=nullptr)
		vehicle->SetPos(0, 0, 10);
	App->physics->vehicles.clear();

	return true;
}

// Update: draw background
update_status ModulePlayer2::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		acceleration = MAX_DEACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY() < 3.0f)
	{
		vehicle->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
		if (actual_checkpoint == CheckPoint::First_CP)
		{
			vehicle->SetTransform(App->player->CheckPoint_1_mat.M);
		}
		else if (actual_checkpoint == CheckPoint::Second_CP)
		{
			vehicle->SetTransform(App->player->CheckPoint_2_mat.M);
		}
		else if (actual_checkpoint == CheckPoint::Third_CP)
		{
			vehicle->SetTransform(App->player->CheckPoint_3_mat.M);
		}
		else if (actual_checkpoint == CheckPoint::Fourth_CP)
		{
			vehicle->SetTransform(App->player->CheckPoint_4_mat.M);
		}

	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();


	return UPDATE_CONTINUE;
}



