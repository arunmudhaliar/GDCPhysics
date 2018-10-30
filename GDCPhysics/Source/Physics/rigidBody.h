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

class RigidBody : public transformx {
public:
    RigidBody();
    virtual ~RigidBody();
    
    void UpdateRigidBody();
    void SetMass(intx mass);
    void AddForce(vector2x force);
    void ClearForce();
    
    void SetRBPosition(const vector2x pos);
protected:
    vector2x force;
    vector2x velocity;
    intx mass;
    intx invMass;
};
