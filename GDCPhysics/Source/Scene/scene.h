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

class Scene : protected NetworkManagerDelegate, protected FixedUpdateObserver {
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
    
    void StartGameFromMenu();
    void MouseBtnUp();
    void MoveStrickerUP(bool keyDown);
    void MoveStrickerDown(bool keyDown);
    
    PLAYER_TYPE GetPlayerType() { return this->playerType; }
    float GetGoalElapsed()  { return this->goalElapsedTime; }
    void ClearGoalElapsed() { this->goalElapsedTime = 0.0f; }

protected:
    vector2f windowSize;
    Ball ball;
    Wall ground;
    Wall topWall;
    Wall leftWall;
    Wall rightWall;
    Solver physicsSolver;
    
    Stricker player1;
    Stricker player2;
    
    void ApplyBoost();
    
    void OnFixedUpdate(intx fixedDT) override;
    
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
    
    void DrawStats();
    
    void SendPing();
    
private:
    void InternalGLStates();
    GAME_STATE gameState;
    PLAYER_TYPE playerType;
    unsigned long pingTimeFromOtherPlayer;
    unsigned long pingStartTime;
    SDL_Window* window;

    bool inputMoveDown;
    bool inputMoveUp;
    bool remoteInputMoveDown;
    bool remoteInputMoveUp;
    std::string player1Score;
    std::string player2Score;
    std::string statusMsg;
    float goalElapsedTime;
    float pingElapsed;
};
