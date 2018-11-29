//
//  stricker.cpp
//  GDCPhysics
//
//  Created by Arun A on 03/11/18.
//  Copyright © 2018 Arun A. All rights reserved.
//

#include "stricker.hpp"
#include "../Physics/core/Timer.h"
#include "ball.h"

Stricker::Stricker() : BoxCollider() {
    windowHeight = 600.0f;
}

Stricker::~Stricker() {
}

void Stricker::MoveUp() {
    float speed = 300.0f;
    position += vector2x(0, FTOX(speed*Timer::getDtinSec()));
    if (XTOF(position.y)>windowHeight*0.68f) {
        position.y = FTOX(windowHeight*0.68f);
    }
}

void Stricker::MoveDown() {
    float speed = 300.0f;
    position += vector2x(0, -FTOX(speed*Timer::getDtinSec()));
    
    if (XTOF(position.y)<windowHeight*0.07f) {
        position.y = FTOX(windowHeight*0.07f);
    }
}

void Stricker::OnCollidedWithRB(RigidBody* rb, const vector2x& contactPt, const vector2x& contactNormal) {
    vector2x n = contactNormal;
    rb->AddForce(n * ITOX(1000));
}

void Stricker::OnRender() {
    glPushMatrix();
    glTranslatef(XTOF(this->position.x), XTOF(this->position.y), 0);
    glColor3f(0.7f, 0.5, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexBuffer);
    glDrawArrays(GL_QUADS, 0, 4);
//    glDrawElements(<#GLenum mode#>, <#GLsizei count#>, <#GLenum type#>, <#const GLvoid *indices#>)(GL_LINE_LOOP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}
