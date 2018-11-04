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
    auto playerType = scene.GetPlayerType();
    if (playerType == Scene::PLAYER_FIRST && this->type==WALL_TYPE::LEFT) {
        NetworkManager::GetInstance().SendMessage("goal");
    } else if (playerType == Scene::PLAYER_SECOND && this->type==WALL_TYPE::RIGHT) {
        NetworkManager::GetInstance().SendMessage("goal");
    }
}

void Wall::OnRender() {
    glPushMatrix();
    glTranslatef(XTOF(this->position.x), XTOF(this->position.y), 0);
    glColor3f(0.6f, 0.6f, 0.6f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexBuffer);
    //    glEnableClientState(GL_COLOR_ARRAY);
    //    glColorPointer(4, GL_FLOAT, 0, vbuffer->m_cszVertexColorList);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    //    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}
