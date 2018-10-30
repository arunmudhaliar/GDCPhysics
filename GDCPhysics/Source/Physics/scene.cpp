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

void Scene::InitScene() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void Scene::Update() {
    
}

void Scene::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

