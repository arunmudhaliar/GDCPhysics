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
#include "core/Timer.h"

Scene::Scene() {
    this->gameState = GAME_STATE_MAX;
}

Scene::~Scene() {
}

void Scene::InitScene(float cx, float cy) {
    cx*=0.5f; cy*=0.5f;     // for retina display.
    windowSize.set(cx, cy);
    
    SetGameState(GAME_INIT);
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
    Timer::update();
    if (this->gameState == GAME_START) {
        physicsSolver.UpdateSolver();
    }
}

void Scene::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // render objects here
    ball.draw();
//    ball2.draw();
    ground.draw();
    leftWall.draw();
    rightWall.draw();
    topWall.draw();
    //
    
    glPopMatrix();
}

void Scene::MouseBtnUp() {
    if (this->gameState != GAME_START) {
        return;
    }
    
    auto vel = ball.GetRBVelocity();
    if (vel.lengthx()<FTOX(20.0f)) {
        ball.AddForce(vector2x(FTOX(-2000.0f), FTOX(5000.0f)));
    } else {
        vel.normalizex();
        ball.AddForce(vel*ITOX(20000));
    }
}


void Scene::OnNetworkMessage(const std::string& msg) {
    printf("Msg from other player %s\n", msg.c_str());
    
    if (msg == "first" || msg == "second") {
        if (this->gameState == GAME_INIT || this->gameState == GAME_RESET) {
            SetGameState(GAME_START);
        }
    }
}

void Scene::OnNetworkFail() {
}

void Scene::OnNetworkOpen() {
    NetworkManager::GetInstance().SendMessage("Hello from Mac");
}

void Scene::OnNetworkClose() {
}


void Scene::SetGameState(GAME_STATE state) {
    if (this->gameState!=state) {
        auto prevState = this->gameState;
        this->gameState = state;
        OnGameStateChange(prevState);
    }
}

void Scene::OnGameStateChange(GAME_STATE from) {
    switch (this->gameState) {
        case GAME_INIT: {
            OnGameInit();
        }
            break;
        case GAME_START: {
            OnGameStart();
        }
            break;
        case GAME_RESET: {
            OnGameReset();
        }
            break;
        default:
            break;
    }
}

void Scene::OnGameInit() {
    Timer::init();
    
    // load default states for ogl
    InternalGLStates();
    
    // init scene
    physicsSolver.InitSolver();
    ball.initBall(60.0f, 1.0f, vector2x(ITOX(600), ITOX(600)));
    //    ball2.initBall(60.0f, 10.0f, vector2x(ITOX(800), ITOX(600)));
    ground.InitBoxCollider(vector2x(ITOX(1400), ITOX(100)), vector2x(0, 0));
    leftWall.InitBoxCollider(vector2x(ITOX(60), ITOX(1000)), vector2x(0, 0));
    rightWall.InitBoxCollider(vector2x(ITOX(60), ITOX(1000)), vector2x(ITOX(1400), 0));
    topWall.InitBoxCollider(vector2x(ITOX(1400), ITOX(100)), vector2x(0, ITOX(700)));
    ball.AddForce(vector2x(FTOX(-5000.0f), 0));
    
    physicsSolver.AddRigidBody(&ball);
    //    physicsSolver.AddRigidBody(&ball2);
    physicsSolver.AddBoxCollider(&ground);
    physicsSolver.AddBoxCollider(&leftWall);
    physicsSolver.AddBoxCollider(&rightWall);
    physicsSolver.AddBoxCollider(&topWall);
    
    NetworkManager::GetInstance().InitNetwork(this);
}

void Scene::OnGameStart() {
    
}

void Scene::OnGameReset() {
    
}
