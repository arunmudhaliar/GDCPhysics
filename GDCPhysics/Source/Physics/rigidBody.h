#pragma once
//
//  rigidBody.h
//  Copyright 2018 GDCPhysics
//  GDCPhysics
//
//  Created by Arun Mudaliar.
//
//

#include "core/transformx.h"
#include "core/vector2x.h"
#include "colliders/boxCollider.hpp"
#include <vector>

class BoxCollider;

class RigidBody {
public:
    RigidBody();
    virtual ~RigidBody();
    
    void SetMass(intx mass);
    void SetRadius(intx radius);
    void AddForce(vector2x force);
    void ClearForce();
    void SimulateStep(intx fixedDT, vector2x& displacement, vector2x& velocity);

    void SetRBPosition(const vector2x pos, bool updateTransformImmediate = false);
    void SetRBVelocity(const vector2x& velocity);
    
    inline vector2x& GetRBPosition()   { return position; }
    inline vector2x& GetRBVelocity()   { return velocity; }
    inline intx GetRBMass() { return mass; }
    inline intx GetRadius() { return radius; }
    inline intx GetRadiusSq() { return radiusSq; }
    inline vector2x& GetRBForce()   { return force; }
    
    void TriggerCollisionEvent(std::vector<BoxCollider*>& colliders) { OnCollision(colliders); }
protected:
    vector2x force;
    vector2x velocity;
    intx mass;
    intx invMass;
    vector2x position;
    intx radius;
    intx radiusSq;
    
    virtual void UpdatePositionFromRB(const vector2x& displacement) = 0;
    virtual void SetPositionFromRB(const vector2x& pos) = 0;
    
    virtual void OnCollision(std::vector<BoxCollider*>& colliders) = 0;
};
