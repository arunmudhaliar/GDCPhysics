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
#include "../Physics/core/Timer.h"
#include "../Physics/core/util.h"

Scene& Scene::GetInstance() {
    static Scene instance;
    return instance;
}

Scene::Scene() {
    this->gameState = GAME_STATE_MAX;
    this->playerType = PLAYER_TYPE_MAX;
    this->pingTimeFromOtherPlayer = 0;
    this->window = nullptr;
    this->statusMsg = "Connecting...";
    this->inputMoveDown = false;
    this->inputMoveUp = false;
    this->remoteInputMoveDown = false;
    this->remoteInputMoveUp = false;
    this->player1Score = "0";
    this->player2Score = "0";
    this->goalElapsedTime = 0.0f;
}

Scene::~Scene() {
}

void Scene::InitScene(SDL_Window* window, float cx, float cy) {
    this->window = window;
    cx*=0.5f; cy*=0.5f;     // for retina display.
    windowSize.set(cx, cy);
    
    geFontManager::InitializeFonts();
    
    SetGameState(GAME_INIT);
    Resize(cx*2.0f, cy*2.0f);
}

void Scene::InternalGLStates() {
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
//    glShadeModel(GL_FLAT);                          // Enable Smooth Shading
    glClearDepth(1.0f);                             // Depth Buffer Setup
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);                             // Enable culling
    glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do (GL_LEQUAL is must for shadow fonts)
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    
//    glEnable( GL_LINE_SMOOTH );
//    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
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
        
        this->goalElapsedTime+= Timer::getDtinSec();
        
        // procss input
        if (this->inputMoveDown || this->inputMoveUp) {
            if (this->inputMoveDown) {
                if (this->playerType == PLAYER_FIRST) {
                    this->player1.MoveDown();
                } else if (this->playerType == PLAYER_SECOND) {
                    this->player2.MoveDown();
                }
            }
            if (this->inputMoveUp) {
                if (this->playerType == PLAYER_FIRST) {
                    this->player1.MoveUp();
                } else if (this->playerType == PLAYER_SECOND) {
                    this->player2.MoveUp();
                }
            }
        }
        
        // remote input
        if (this->remoteInputMoveDown || this->remoteInputMoveUp) {
            if (this->remoteInputMoveDown) {
                if (this->playerType == PLAYER_FIRST) {
                    this->player2.MoveDown();
                } else if (this->playerType == PLAYER_SECOND) {
                    this->player1.MoveDown();
                }
            }
            if (this->remoteInputMoveUp) {
                if (this->playerType == PLAYER_FIRST) {
                    this->player2.MoveUp();
                } else if (this->playerType == PLAYER_SECOND) {
                    this->player1.MoveUp();
                }
            }
        }
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
    ground.draw();
    leftWall.draw();
    rightWall.draw();
    topWall.draw();
    player1.draw();
    player2.draw();
    //
    
    DrawStats();
    
    glPopMatrix();
}

void Scene::DrawStats() {
    if (geFontManager::g_pFontArial10_84Ptr == nullptr) {
        return;
    }
    // Stats
    glPushMatrix();
    glScalef(1, -1, 1);
    int iterator = 0;
    geFontManager::g_pFontArial10_84Ptr->drawString(util::stringFormat("FPS %3.2f", Timer::getFPS()).c_str(), 45, -(60+(iterator++)*20), 200);
    geFontManager::g_pFontArial10_84Ptr->drawString(util::stringFormat("PING %d ms", this->pingTimeFromOtherPlayer).c_str(), 45, -(60+(iterator++)*20), 200);
    if (this->gameState == GAME_START) {
        geFontManager::g_pFontArial10_84Ptr->drawString((this->playerType == PLAYER_FIRST) ? "PLAYER 1" : "PLAYER 2", 45, -(60+(iterator++)*20), 200);
    }
    geFontManager::g_pFontArial10_84Ptr->drawString(util::stringFormat("ELAPSED %lu ms", this->physicsSolver.GetElapsedTime()).c_str(), 45, -(60+(iterator++)*20), 200);
    geFontManager::g_pFontArial10_84Ptr->drawString(util::stringFormat("STATUS : %s", this->statusMsg.c_str()).c_str(), 45, -(60+(iterator++)*20), 200);
    geFontManager::g_pFontArial10_84Ptr->drawString(util::stringFormat("BALL VEL : %d", XTOI(this->ball.GetRBVelocity().lengthx())).c_str(), 45, -(60+(iterator++)*20), 200);

    geFontManager::g_pFontArial10_84Ptr->drawString(util::stringFormat("PLAYER 1: %s", player1Score.c_str()).c_str(), windowSize.x*0.09f, -(windowSize.y*0.85f), 200);
    geFontManager::g_pFontArial10_84Ptr->drawString(util::stringFormat("PLAYER 2: %s", player2Score.c_str()).c_str(), windowSize.x*0.82f, -(windowSize.y*0.85f), 200);
    glPopMatrix();
    //
}

void Scene::MouseBtnUp() {
    if (this->gameState != GAME_START) {
        return;
    }
    
    this->ball.SendBallState();
    NetworkManager::GetInstance().SendMessage("boost");
    ApplyBoost();
}

void Scene::MoveStrickerUP(bool keyDown) {
    if (this->inputMoveUp != keyDown) {
        std::string msg = (keyDown) ? "stricker_up|1" : "stricker_up|0";
        if (this->playerType == PLAYER_FIRST) {
            auto pos = this->player1.GetPosition();
            msg+=util::stringFormat("|%d,%d", pos.x, pos.y);
        } else if (this->playerType == PLAYER_SECOND) {
            auto pos = this->player2.GetPosition();
            msg+=util::stringFormat("|%d,%d", pos.x, pos.y);
        }
        NetworkManager::GetInstance().SendMessage(msg);
    }
    this->inputMoveUp = keyDown;
}

void Scene::MoveStrickerDown(bool keyDown) {
    if (this->inputMoveDown != keyDown) {
        std::string msg = (keyDown) ? "stricker_down|1" : "stricker_down|0";
        if (this->playerType == PLAYER_FIRST) {
            auto pos = this->player1.GetPosition();
            msg+=util::stringFormat("|%d,%d", pos.x, pos.y);
        } else if (this->playerType == PLAYER_SECOND) {
            auto pos = this->player2.GetPosition();
            msg+=util::stringFormat("|%d,%d", pos.x, pos.y);
        }
        NetworkManager::GetInstance().SendMessage(msg);
    }
    this->inputMoveDown = keyDown;
}

void Scene::ApplyBoost() {
    auto vel = ball.GetRBVelocity();
    if (vel.lengthx()<FTOX(20.0f)) {
        ball.AddForce(vector2x(FTOX(-2000.0f), FTOX(5000.0f)));
    } else {
        vel.normalizex();
        ball.AddForce(vel*ITOX(8000));
    }
}

void Scene::StartGameFromMenu() {
    if (this->gameState == GAME_INIT || this->gameState == GAME_RESET) {
        NetworkManager::GetInstance().SendMessage("ping");
        pingTimeFromOtherPlayer = Timer::getCurrentTimeInMilliSec();
    }
}

void Scene::OnNetworkMessage(const std::string& msg) {
    printf("Msg from other player %s\n", msg.c_str());
    
    if (msg == "first" || msg == "second") {
        if (this->gameState == GAME_INIT || this->gameState == GAME_RESET) {
            this->physicsSolver.RemoveBoxCollider(&player1);
            this->physicsSolver.RemoveBoxCollider(&player2);
            if (msg == "first") {
                this->playerType = PLAYER_FIRST;
                this->physicsSolver.AddBoxCollider(&player1);
            } else if (msg == "second") {
                this->playerType = PLAYER_SECOND;
                this->physicsSolver.AddBoxCollider(&player2);
            }
            NetworkManager::GetInstance().SendMessage("ping");
            pingTimeFromOtherPlayer = Timer::getCurrentTimeInMilliSec();
        }
    } else if (msg == "ping") {
        pingTimeFromOtherPlayer = Timer::getCurrentTimeInMilliSec()-pingTimeFromOtherPlayer;
        printf("PING TIME %lu ms.\n", pingTimeFromOtherPlayer);
        if (this->gameState == GAME_INIT || this->gameState == GAME_RESET) {
            NetworkManager::GetInstance().SendMessage("ping_akn");
        }
    } else if (msg == "startgame") {
        if (this->gameState == GAME_INIT || this->gameState == GAME_RESET) {
            SetGameState(GAME_START);
        }
    } else if (msg == "boost") {
        ApplyBoost();
    } else {
        std::vector<std::string> lines;
        util::splitString(msg, lines);
        if (lines.size()) {
            if (lines[0] == "ball" && lines.size()==2) {
                std::vector<std::string> args;
                util::splitString(lines[1], args, ',');
                if (args.size()==4) {
                    intx px = atoi(args[0].c_str());
                    intx py = atoi(args[1].c_str());
                    intx vx = atoi(args[2].c_str());
                    intx vy = atoi(args[3].c_str());
                    
                    this->ball.SetRBVelocity(vector2x(vx, vy));
                    this->ball.SetRBPosition(vector2x(px, py));
                }
            } else if (lines[0] == "stricker" && lines.size()==2) {
                std::vector<std::string> args;
                util::splitString(lines[1], args, ',');
                if (args.size()==2) {
                    intx fx = atoi(args[0].c_str());
                    intx fy = atoi(args[1].c_str());
                    this->ball.AddForce(vector2x(fx, fy));
                }
            } else if (lines[0] == "stricker_up" && lines.size()==3) {
                int val = atoi(lines[1].c_str());
                this->remoteInputMoveUp = (val==1);
                std::vector<std::string> args;
                util::splitString(lines[2], args, ',');
                if (args.size()==2) {
                    intx px = atoi(args[0].c_str());
                    intx py = atoi(args[1].c_str());
                    if (this->playerType == PLAYER_FIRST) {
                        this->player2.SetPosition(vector2x(px, py));
                    } else if (this->playerType == PLAYER_SECOND) {
                        this->player1.SetPosition(vector2x(px, py));
                    }
                }
            } else if (lines[0] == "stricker_down" && lines.size()==3) {
                int val = atoi(lines[1].c_str());
                this->remoteInputMoveDown = (val==1);
                std::vector<std::string> args;
                util::splitString(lines[2], args, ',');
                if (args.size()==2) {
                    intx px = atoi(args[0].c_str());
                    intx py = atoi(args[1].c_str());
                    if (this->playerType == PLAYER_FIRST) {
                        this->player2.SetPosition(vector2x(px, py));
                    } else if (this->playerType == PLAYER_SECOND) {
                        this->player1.SetPosition(vector2x(px, py));
                    }
                }
            } else if (lines[0] == "score" && lines.size()==2) {
                std::vector<std::string> args;
                util::splitString(lines[1], args, ',');
                if (args.size()==2) {
                    player1Score = args[0];
                    player2Score = args[1];
                }
            }
        }
    }
}

void Scene::OnNetworkFail() {
    statusMsg = "Network Error...";
}

void Scene::OnNetworkOpen() {
    NetworkManager::GetInstance().SendMessage("Hello from Mac");
    statusMsg = "CONNECTED";
}

void Scene::OnNetworkClose() {
    statusMsg = "DISCONECTED";
    SetGameState(GAME_RESET);
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
    ball.initBall(35.0f, 1.0f, vector2x(FTOX(windowSize.x*0.5f), FTOX(windowSize.y*0.75f)));
    ground.InitBoxCollider(vector2x(FTOX(windowSize.x), ITOX(40)), vector2x(0, 0));
    leftWall.InitBoxCollider(vector2x(ITOX(40), FTOX(windowSize.y)), vector2x(0, 0));
    rightWall.InitBoxCollider(vector2x(ITOX(40), FTOX(windowSize.y)), vector2x(FTOX(windowSize.x-40.0f), 0));
    topWall.InitBoxCollider(vector2x(FTOX(windowSize.x), ITOX(40)), vector2x(0, FTOX(windowSize.y-40.0f)));
    ball.AddForce(vector2x(FTOX(-5000.0f), 0));
    
    ground.SetWallType(Wall::BOTTOM);
    leftWall.SetWallType(Wall::LEFT);
    rightWall.SetWallType(Wall::RIGHT);
    topWall.SetWallType(Wall::TOP);
    
    player1.InitBoxCollider(vector2x(ITOX(35), FTOX(windowSize.y*0.25f)), vector2x(ITOX(45), FTOX(windowSize.y*0.5f-(windowSize.y*0.25f*0.5f))));
    player2.InitBoxCollider(vector2x(ITOX(35), FTOX(windowSize.y*0.25f)), vector2x(FTOX(windowSize.x-80.0f), FTOX(windowSize.y*0.5f-(windowSize.y*0.25f*0.5f))));
    player1.SetWindowHeight(this->windowSize.y);
    player2.SetWindowHeight(this->windowSize.y);

    physicsSolver.AddRigidBody(&ball);
    physicsSolver.AddBoxCollider(&ground);
    physicsSolver.AddBoxCollider(&leftWall);
    physicsSolver.AddBoxCollider(&rightWall);
    physicsSolver.AddBoxCollider(&topWall);
    
    NetworkManager::GetInstance().InitNetwork(this);
}

void Scene::OnGameStart() {
    statusMsg = "GAMEPLAY";
}

void Scene::OnGameReset() {
    this->physicsSolver.RemoveBoxCollider(&player1);
    this->physicsSolver.RemoveBoxCollider(&player2);
    
    this->remoteInputMoveDown = false;
    this->remoteInputMoveUp = false;
    this->inputMoveUp = false;
    this->inputMoveDown = false;
}
