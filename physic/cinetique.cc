#include "../include/engine.hh"

#define FORCE_LOSS 1

fl::vec3
resolveAccel(fl::vec3 &velocity, fl::vec3 &acceleration, float mass, float deltaTime) {
    // Calculate the force using F = m * a
    fl::vec3 force = fl::vec3mulValue(acceleration, mass);

    // Integrate the force to update the velocity
    velocity = fl::vec3add(velocity, fl::vec3mulValue(force, deltaTime));

    // Integrate the velocity to update the position (if needed)
	if (fl::distance3(velocity, {0}) > 0) {
		velocity = fl::vec3sub(velocity, fl::vec3mulValue(velocity, FORCE_LOSS * deltaTime));
	}


    // Reset acceleration for the next frame
    acceleration = fl::vec3{0.0f, 0.0f, 0.0f};

    return velocity;
}

void
addImpulse(fl::vec3 &acceleration, const fl::vec3 &impulse, float mass, float speed) {
    // Calculate the resulting acceleration using F = m * a
    fl::vec3 resultingAccel = fl::vec3mulValue(fl::vec3divValue(impulse, mass), speed);

    // Add the resulting acceleration to the current acceleration
   acceleration = fl::vec3add(acceleration, resultingAccel);
}