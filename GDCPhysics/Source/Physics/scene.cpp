//
//  scene.cpp
//  Copyright 2018 GDCPhysics
//  GDCPhysics
//
//  Created by Arun Mudaliar.
//
//

#include "scene.h"
#include <OpenGL/gl.h>

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::InitScene(float cx, float cy) {
    cx*=0.5f; cy*=0.5f;     // for retina display.
    windowSize.set(cx, cy);
    
    // load default states for ogl
    InternalGLStates();
    
    // init scene
    ball.initBall(20.0f, vector2x(FTOX(windowSize.x*0.5f), FTOX(windowSize.y*0.85f)));
    ground.InitBoxCollider(vector2x(ITOX(2000), ITOX(100)), vector2x(0, 0));
}

void Scene::InternalGLStates() {
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glShadeModel(GL_FLAT);                          // Enable Smooth Shading
    glClearDepth(1.0f);                             // Depth Buffer Setup
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);                             // Enable culling
    glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do (GL_LEQUAL is must for shadow fonts)
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
}

void Scene::Resize(float cx, float cy) {
    cx*=0.5f; cy*=0.5f;     // for retina display.
    windowSize.set(cx, cy);
    glViewport(0, 0, (int)cx, (int)cy);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, (int)cx, 0.0f, (int)cy, 0.0f, 10.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Scene::Update() {
    ball.UpdateRigidBody();
}

void Scene::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // render objects here
    ball.draw();
    ground.draw();
    //
    
    glPopMatrix();
}

