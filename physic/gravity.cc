#include "../include/engine.hh"

void
applyFriction(fl::vec3 &velocity, float frictionFactor) {
    velocity = fl::vec3mulValue(velocity, (1.0f - frictionFactor));
}

void
applyRestitution(fl::vec3 &velocity, float restitutionFactor) {
    velocity = fl::vec3mulValue(velocity, -restitutionFactor);  // Invert the velocity and multiply by restitution
}

void
applyGravity(fl::vec3 &acceleration, const fl::vec3 &gravity, float mass) {
    acceleration = fl::vec3add(acceleration, fl::vec3mulValue(gravity, mass));
}