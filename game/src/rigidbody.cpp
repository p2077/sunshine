#include "rigidbody.h"
#include "raylib.h"

Rigidbody::Rigidbody() {
    _velocity = { 0.0f, 0.0f };
    _position = { 0.0f, 0.0f };
}
Rigidbody::Rigidbody(Vector2 velocity, Vector2 position) {
    _velocity = velocity;
    _position = position;
}
Rigidbody::~Rigidbody() {

}