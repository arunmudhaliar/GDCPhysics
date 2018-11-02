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
#include "../Connection/NetworkManager.hpp"

class Scene : protected NetworkManagerDelegate {
public:
    enum GAME_STATE {
        GAME_INIT,
        GAME_START,
        GAME_RESET,
        GAME_STATE_MAX
    };
    
    Scene();
    ~Scene();
    
    void InitScene(float cx, float cy);
    void Resize(float cx, float cy);
    void Update();
    void Render();
    
    void MouseBtnUp();
    
protected:
    vector2f windowSize;
    Ball ball;
//    Ball ball2;
    BoxCollider ground;
    BoxCollider leftWall;
    BoxCollider rightWall;
    BoxCollider topWall;
    Solver physicsSolver;
    
    // NetworkManagerDelegate
    void OnNetworkMessage(const std::string& msg) override;
    void OnNetworkFail() override;
    void OnNetworkOpen() override;
    void OnNetworkClose() override;
    
    void SetGameState(GAME_STATE state);
    void OnGameStateChange(GAME_STATE from);
    void OnGameInit();
    void OnGameStart();
    void OnGameReset();
    
private:
    void InternalGLStates();
    GAME_STATE gameState;
};
