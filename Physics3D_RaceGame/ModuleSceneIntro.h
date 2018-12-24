#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

enum class CheckPoint
{
	First_CP,
	Second_CP, 
	Third_CP,
	Fourth_CP
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void CreateCircuit(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, float angle = 0.0f, vec3 rotation = { 0, 0, 0 }, Color color = White);
	void CreateObstacles();
	void TimeSet(uint timer);
	
	
	void Render_Circuit();
	void Render_Obstacles(PhysBody3D* body, Cube* cube);

	p2List<Cube*> circuitList;
	p2List<Cube*> obstacleList;


public:
	
	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	Timer chrono;
	int chrono_player1 = 0;
	int chrono_player2 = 0;

	bool ended = false;
	bool startTimer = false;

	int winner = 0;

	//sfx
	uint car_accelerate;
	uint car_brake;
	uint car_fall;

private:

	// SENSORS -----------------------
	PhysBody3D* sensor1;
	Cube* c_sensor1;

	PhysBody3D* sensor2;
	Cube* c_sensor2;

	PhysBody3D* sensor3;
	Cube* c_sensor3;

	PhysBody3D* sensor4;
	Cube* c_sensor4;

	// HINGE ---------------------------
	PhysBody3D* anchor_physbody;
	Cube* obstacleAnchor;
	PhysBody3D* body_physbody;
	Cube* obstacleBody;

	PhysBody3D* anchor_physbody1;
	Cube* obstacleAnchor1;
	PhysBody3D* body_physbody1;
	Cube* obstacleBody1;

	PhysBody3D* anchor_physbody2;
	Cube* obstacleAnchor2;
	PhysBody3D* body_physbody2;
	Cube* obstacleBody2;

	PhysBody3D* anchor_physbody3;
	Cube* obstacleAnchor3;
	PhysBody3D* body_physbody3;
	Cube* obstacleBody3;


};
