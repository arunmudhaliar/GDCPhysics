#pragma once
//
//  scene.h
//  Copyright 2018 GDCPhysics
//  GDCPhysics
//
//  Created by Arun Mudaliar.
//
//

#include "ball.h"
#include "colliders/boxCollider.hpp"
#include "solver.hpp"

class Scene {
public:
    Scene();
    ~Scene();
    
    void InitScene(float cx, float cy);
    void Resize(float cx, float cy);
    void Update();
    void Render();
    
protected:
    vector2f windowSize;
    Ball ball;
    BoxCollider ground;
    BoxCollider leftWall;
    BoxCollider rightWall;
    Solver physicsSolver;
    
private:
    void InternalGLStates();
};
