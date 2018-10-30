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

class Ball : public RenderObject, public RigidBody {
public:
    Ball();
    ~Ball();
    
    void initBall(float size, const vector2x& pos);
    
protected:
    float vertexBuffer[36*2];
    float size;
    virtual void OnRender();
};
