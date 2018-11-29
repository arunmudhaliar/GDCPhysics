//
//  ball.cpp
//  Copyright 2018 GDCPhysics
//  GDCPhysics
//
//  Created by Arun Mudaliar.
//
//

#include "ball.h"
#include "../Physics/core/util.h"
#include "../Connection/NetworkManager.hpp"
#include "stricker.hpp"

Ball::Ball() {
    this->size = 0;
    memset(vertexBuffer, 0 , sizeof(vertexBuffer));
}

Ball::~Ball() {
}

void Ball::initBall(float size, float mass, const vector2x& pos) {
    this->size = size;
    SetRadius(FTOX(size));
    SetMass(FTOX(mass));
    int delta = (360/SEGMENTS);
    vertexBuffer[0]=vertexBuffer[1] = 0.0f;
    for(int xx=0; xx<SEGMENTS; xx++) {
        float cs=size*cos(DEG2RAD((float)xx*delta));
        float sn=size*sin(DEG2RAD((float)xx*delta));
        vertexBuffer[(xx+1)*2+0]=cs;
        vertexBuffer[(xx+1)*2+1]=sn;
    }
    vertexBuffer[SEGMENTS*2+0]=vertexBuffer[(1)*2+0];
    vertexBuffer[SEGMENTS*2+1]=vertexBuffer[(1)*2+1];
    SetRBPosition(pos, true);
}

void Ball::OnRender() {
    auto p = getOpenGLMatrix();
    glPushMatrix();
    glTranslatef(XTOF(p[12]), XTOF(p[13]), XTOF(p[14]));
    glColor3f(0.75f, 0, 0.2f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexBuffer);
    glDrawArrays(GL_TRIANGLE_FAN, 0, SEGMENTS+2);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void Ball::UpdatePositionFromRB(const vector2x& displacement) {
    updatePositionx(displacement.x, displacement.y, 0);
}

void Ball::SetPositionFromRB(const vector2x& pos) {
    setPositionx(pos);
}

void Ball::OnCollision(std::vector<BoxCollider*>& colliders) {
    for(auto c : colliders) {
        Stricker* stricker = dynamic_cast<Stricker*>(c);
        if (stricker) {
            SendBallState();

            // send striker force
            auto msg = util::stringFormat("stricker|%d,%d", this->force.x, this->force.y);
            NetworkManager::GetInstance().SendMessage(msg);
            //
            
            // there will be duplicates so break.
            break;
        }
    }
}

void Ball::SendBallState() {
    auto pos = GetRBPosition();
    auto velocity = GetRBVelocity();
    auto msg = util::stringFormat("ball|%d,%d,%d,%d", pos.x, pos.y, velocity.x, velocity.y);
    NetworkManager::GetInstance().SendMessage(msg);
}
