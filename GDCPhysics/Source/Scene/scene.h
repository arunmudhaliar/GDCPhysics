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
#include "stricker.hpp"
#include "wall.hpp"
#include "../Physics/colliders/boxCollider.hpp"
#include "../Physics/solver.hpp"
#include "../Connection/NetworkManager.hpp"
#include "../GUI/geFontManager.h"
#include <SDL2/SDL.h>

class Scene : protected NetworkManagerDelegate {
private:
    Scene();
    
public:
    enum GAME_STATE {
        GAME_INIT,
        GAME_START,
        GAME_RESET,
        GAME_STATE_MAX
    };
    
    enum PLAYER_TYPE {
        PLAYER_FIRST,
        PLAYER_SECOND,
        PLAYER_TYPE_MAX
    };
    
    static Scene& GetInstance();
    
    ~Scene();
    
    void InitScene(SDL_Window* window, float cx, float cy);
    void Resize(float cx, float cy);
    void Update();
    void Render();
    
    void MouseBtnUp();
    
    void MoveStrickerUP(bool keyDown);
    void MoveStrickerDown(bool keyDown);
    
    PLAYER_TYPE GetPlayerType() { return this->playerType; }
    
protected:
    vector2f windowSize;
    Ball ball;
//    Ball ball2;
    Wall ground;
    Wall topWall;
    Wall leftWall;
    Wall rightWall;

    Solver physicsSolver;
    
    Stricker player1;
    Stricker player2;
    
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
    
    void ApplyBoost();
    
    void DrawStats();
    
private:
    void InternalGLStates();
    GAME_STATE gameState;
    PLAYER_TYPE playerType;
    unsigned long pingTimeFromOtherPlayer;
    SDL_Window* window;
    std::string statusMsg;
    
    bool inputMoveDown;
    bool inputMoveUp;
    
    bool remoteInputMoveDown;
    bool remoteInputMoveUp;
    std::string player1Score;
    std::string player2Score;
};
