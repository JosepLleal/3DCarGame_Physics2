#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define MAX_DEACCELERATION -1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 70.0f

class ModulePlayer2 : public Module
{
public:
	ModulePlayer2(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer2();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	CheckPoint actual_checkpoint = CheckPoint::First_CP;

public:

	bool reached_CP4 = false;

	PhysVehicle3D * vehicle;
	float turn;
	float acceleration;
	float brake;
};