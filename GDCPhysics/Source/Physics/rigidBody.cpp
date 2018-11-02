//
//  rigidBody.cpp
//  Copyright 2018 GDCPhysics
//  GDCPhysics
//
//  Created by Arun Mudaliar.
//
//

#include "rigidBody.h"

RigidBody::RigidBody() {
    SetMass(FX_ONE);
    SetRadius(ITOX(20));
}

RigidBody::~RigidBody() {
}

void RigidBody::UpdateRigidBody(intx fixedDT) {
    // F = m * a
    // a = F / m
    
//    vector2x acceleration = this->force * invMass;
//    vector2x velocity = acceleration * fixedDT;     // fixed dt for 45 FPS
//    this->velocity += velocity;
//    auto displacement = this->velocity * fixedDT;
//    this->position += displacement;
    
//    vector2x outDisplacement;
//    vector2x outVelocity;
//    SimulateStep(fixedDT, outDisplacement, outVelocity);
//    
//    // check if new position is safe or not here.
//    this->position += outDisplacement;
//    this->velocity += outVelocity;
//    //
//    
//    SetPositionFromRB(this->position);
//    ClearForce();
//    AddForce(vector2x(0, GRAVITY));
}

void RigidBody::SimulateStep(intx fixedDT, vector2x& displacement, vector2x& velocity) {
    vector2x acceleration = this->force * invMass;
    velocity = acceleration * fixedDT;     // fixed dt for 45 FPS
    displacement = (velocity + this->velocity) * fixedDT;
}

void RigidBody::SetMass(intx mass) {
    this->mass = mass;
    this->invMass = DIVX(FX_ONE, mass);
}

void RigidBody::SetRadius(intx radius) {
    this->radius = radius;
    this->radiusSq = MULTX(radius, radius);
}

void RigidBody::AddForce(vector2x force) {
    this->force += force;
}
void RigidBody::ClearForce() {
    this->force.zerox();
}

void RigidBody::SetRBPosition(const vector2x pos, bool updateTransformImmediate) {
    this->position = pos;
    if (updateTransformImmediate) {
        SetPositionFromRB(this->position);
    }
}

void RigidBody::SetRBVelocity(const vector2x& velocity) {
    this->velocity = velocity;
}
