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
}

RigidBody::~RigidBody() {
}

void RigidBody::UpdateRigidBody() {
    // F = m * a
    // a = F / m
    
    vector2x acceleration = this->force * invMass;
    vector2x velocity = acceleration * FTOX(0.022f);     // fixed dt for 45 FPS
    this->velocity += velocity;
    auto displacement = this->velocity * FTOX(0.022f);
    updatePositionx(displacement.x, displacement.y, 0);
    
    ClearForce();
    AddForce(vector2x(0, -ITOX(10)));
}

void RigidBody::SetMass(intx mass) {
    this->mass = mass;
    this->invMass = DIVX(FX_ONE, mass);
}

void RigidBody::AddForce(vector2x force) {
    this->force += force;
}
void RigidBody::ClearForce() {
    this->force.zerox();
}

void RigidBody::SetRBPosition(const vector2x pos) {
    this->setPositionx(pos);
}
