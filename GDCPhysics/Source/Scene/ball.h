#pragma once
//
//  ball.h
//  Copyright 2018 GDCPhysics
//  GDCPhysics
//
//  Created by Arun Mudaliar.
//
//

#include "rigidBody.h"
#include "renderObject.h"

#define SEGMENTS    36

class Ball : public RenderObject, public RigidBody {
public:
    Ball();
    ~Ball();
    
    void initBall(float size, float mass, const vector2x& pos);
    void SendBallState();
    
protected:
    float vertexBuffer[(SEGMENTS+2)*2];
    float size;
    virtual void OnRender();
    
    // rb
    virtual void UpdatePositionFromRB(const vector2x& displacement);
    virtual void SetPositionFromRB(const vector2x& pos);
    virtual void OnCollision(std::vector<BoxCollider*>& colliders);
};
