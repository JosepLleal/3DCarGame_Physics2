#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Grey;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis.color = Red;
	// LEFT CHASSIS ---------------------------------------------------------------------------------------------------------
	Cube left_chassis(info.left_chassis_size.x, info.left_chassis_size.y, info.left_chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&left_chassis.transform);
	btVector3 left_chassis_offset(info.left_chassis_offset.x, info.left_chassis_offset.y, info.left_chassis_offset.z);
	left_chassis_offset = left_chassis_offset.rotate(q.getAxis(), q.getAngle());

	left_chassis.transform.M[12] += left_chassis_offset.getX();
	left_chassis.transform.M[13] += left_chassis_offset.getY();
	left_chassis.transform.M[14] += left_chassis_offset.getZ();

	left_chassis.color = Red;
	// RIGHT CHASSIS ---------------------------------------------------------------------------------------------------------
	Cube right_chassis(info.right_chassis_size.x, info.right_chassis_size.y, info.right_chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&right_chassis.transform);
	btVector3 right_chassis_offset(info.right_chassis_offset.x, info.right_chassis_offset.y, info.right_chassis_offset.z);
	right_chassis_offset = right_chassis_offset.rotate(q.getAxis(), q.getAngle());

	right_chassis.transform.M[12] += right_chassis_offset.getX();
	right_chassis.transform.M[13] += right_chassis_offset.getY();
	right_chassis.transform.M[14] += right_chassis_offset.getZ();

	right_chassis.color = Red;

	// FRONT CHASSIS -----------------------------------------------------------------------------------------------------------

	Cube front_chassis(info.front_chassis_size.x, info.front_chassis_size.y, info.front_chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&front_chassis.transform);
	btVector3 front_chassis_offset(info.front_chassis_offset.x, info.front_chassis_offset.y, info.front_chassis_offset.z);
	front_chassis_offset = front_chassis_offset.rotate(q.getAxis(), q.getAngle());

	front_chassis.transform.M[12] += front_chassis_offset.getX();
	front_chassis.transform.M[13] += front_chassis_offset.getY();
	front_chassis.transform.M[14] += front_chassis_offset.getZ();

	front_chassis.color = Red;

	// FRONT WING ----------------------------------------------------------------------------------------------------------------

	Cube front_wing(info.front_wing_size.x, info.front_wing_size.y, info.front_wing_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&front_wing.transform);
	btVector3 front_wing_offset(info.front_wing_offset.x, info.front_wing_offset.y, info.front_wing_offset.z);
	front_wing_offset = front_wing_offset.rotate(q.getAxis(), q.getAngle());

	front_wing.transform.M[12] += front_wing_offset.getX();
	front_wing.transform.M[13] += front_wing_offset.getY();
	front_wing.transform.M[14] += front_wing_offset.getZ();

	front_wing.color = Red;

	// BACK WING --------------------------------------------------------------------------------------------------------------------

	Cube back_wing(info.back_wing_size.x, info.back_wing_size.y, info.back_wing_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&back_wing.transform);
	btVector3 back_wing_offset(info.back_wing_offset.x, info.back_wing_offset.y, info.back_wing_offset.z);
	back_wing_offset = back_wing_offset.rotate(q.getAxis(), q.getAngle());

	back_wing.transform.M[12] += back_wing_offset.getX();
	back_wing.transform.M[13] += back_wing_offset.getY();
	back_wing.transform.M[14] += back_wing_offset.getZ();

	back_wing.color = Red;

	// BACK WING SUPPORT-------

	Cube back_wingsupport(info.back_wingsupport_size.x, info.back_wingsupport_size.y, info.back_wingsupport_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&back_wingsupport.transform);
	btVector3 back_wingsupport_offset(info.back_wingsupport_offset.x, info.back_wingsupport_offset.y, info.back_wingsupport_offset.z);
	back_wingsupport_offset = back_wingsupport_offset.rotate(q.getAxis(), q.getAngle());

	back_wingsupport.transform.M[12] += back_wingsupport_offset.getX();
	back_wingsupport.transform.M[13] += back_wingsupport_offset.getY();
	back_wingsupport.transform.M[14] += back_wingsupport_offset.getZ();

	back_wingsupport.color = Red;

	// BACK WING RIGHT-------

	Cube back_wingright(info.back_wingright_size.x, info.back_wingright_size.y, info.back_wingright_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&back_wingright.transform);
	btVector3 back_wingright_offset(info.back_wingright_offset.x, info.back_wingright_offset.y, info.back_wingright_offset.z);
	back_wingright_offset = back_wingright_offset.rotate(q.getAxis(), q.getAngle());

	back_wingright.transform.M[12] += back_wingright_offset.getX();
	back_wingright.transform.M[13] += back_wingright_offset.getY();
	back_wingright.transform.M[14] += back_wingright_offset.getZ();

	back_wingright.color = Red;

	// BACK WING LEFT-------

	Cube back_wingleft(info.back_wingleft_size.x, info.back_wingleft_size.y, info.back_wingleft_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&back_wingleft.transform);
	btVector3 back_wingleft_offset(info.back_wingleft_offset.x, info.back_wingleft_offset.y, info.back_wingleft_offset.z);
	back_wingleft_offset = back_wingleft_offset.rotate(q.getAxis(), q.getAngle());

	back_wingleft.transform.M[12] += back_wingleft_offset.getX();
	back_wingleft.transform.M[13] += back_wingleft_offset.getY();
	back_wingleft.transform.M[14] += back_wingleft_offset.getZ();

	back_wingleft.color = Red;

	// PILOT ---------------------------------------------------------------------------------------------------------------
	Cube pilot(info.pilot.x, info.pilot.y, info.pilot.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&pilot.transform);
	btVector3 pilot_offset(info.pilot_offset.x, info.pilot_offset.y, info.pilot_offset.z);
	pilot_offset = pilot_offset.rotate(q.getAxis(), q.getAngle());

	pilot.transform.M[12] += pilot_offset.getX();
	pilot.transform.M[13] += pilot_offset.getY();
	pilot.transform.M[14] += pilot_offset.getZ();

	pilot.color = Black;

	//Renders ------------
	chassis.Render();
	left_chassis.Render();
	right_chassis.Render();
	front_chassis.Render();
	front_wing.Render();
	back_wing.Render();
	back_wingsupport.Render();
	back_wingright.Render();
	back_wingleft.Render();
	pilot.Render();
	//--------------------
	
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}