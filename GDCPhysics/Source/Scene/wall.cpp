//
//  wall.cpp
//  GDCPhysics
//
//  Created by Arun A on 03/11/18.
//  Copyright © 2018 Arun A. All rights reserved.
//

#include "wall.hpp"
#include "../Physics/core/Timer.h"
#include "ball.h"
#include "scene.h"

Wall::Wall() : BoxCollider() {
}

Wall::~Wall() {
}

void Wall::OnCollidedWithRB(RigidBody* rb, const vector2x& contactPt, const vector2x& contactNormal) {
    Scene& scene = Scene::GetInstance();
    if (scene.GetGoalElapsed()<1.0f) {
        return;
    }
    auto playerType = scene.GetPlayerType();
    if (playerType == Scene::PLAYER_FIRST && this->type==WALL_TYPE::LEFT) {
        NetworkManager::GetInstance().SendMessage("goal");
        scene.ClearGoalElapsed();
    } else if (playerType == Scene::PLAYER_SECOND && this->type==WALL_TYPE::RIGHT) {
        NetworkManager::GetInstance().SendMessage("goal");
        scene.ClearGoalElapsed();
    }
}

void Wall::OnRender() {
    glPushMatrix();
    glTranslatef(XTOF(this->position.x), XTOF(this->position.y), 0);
    glColor3f(0.3f, 0.3f, 0.3f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexBuffer);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}
