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

class RigidBody {
public:
    RigidBody();
    virtual ~RigidBody();
    
    void UpdateRigidBody(intx fixedDT);
    void SetMass(intx mass);
    void AddForce(vector2x force);
    void ClearForce();
    
    void SetRBPosition(const vector2x pos, bool updateTransformImmediate = false);
    void SetRBVelocity(const vector2x& velocity);
    
    void SimulateStep(intx fixedDT, vector2x& displacement, vector2x& velocity);
    inline vector2x& GetRBPosition()   { return position; }
    inline vector2x& GetRBVelocity()   { return velocity; }
    
protected:
    vector2x force;
    vector2x velocity;
    intx mass;
    intx invMass;
    vector2x position;
    
    virtual void UpdatePositionFromRB(const vector2x& displacement) = 0;
    virtual void SetPositionFromRB(const vector2x& pos) = 0;
};
