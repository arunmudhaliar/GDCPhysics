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

#define TOP_BOTTOM_BUFFER_HEIGHT    ITOX(4)

Stricker::Stricker() : BoxCollider() {
    // default value. Calculate and assign it at runtime.
    this->windowHeight = ITOX(600);
    this->bottomWallHeight = ITOX(45);
}

Stricker::~Stricker() {
}

void Stricker::SetWindowAndBottomWallHeight(intx windowdHeight, intx bottomWallHeight) {
    this->windowHeight = windowdHeight;
    this->bottomWallHeight = bottomWallHeight;
}

void Stricker::MoveUp(intx fixedDT) {
    intx speed = ITOX(300);
    position += vector2x(0, MULTX(speed, fixedDT));
    intx validAreaToMove = windowHeight-(size.y+bottomWallHeight+TOP_BOTTOM_BUFFER_HEIGHT);
    if (position.y > validAreaToMove) {
        position.y = validAreaToMove;
    }
}

void Stricker::MoveDown(intx fixedDT) {
    intx speed = ITOX(300);
    position += vector2x(0, -MULTX(speed, fixedDT));
    
    intx validAreaToMove = bottomWallHeight+TOP_BOTTOM_BUFFER_HEIGHT;
    if (position.y < validAreaToMove) {
        position.y = validAreaToMove;
    }
}

void Stricker::OnCollidedWithRB(RigidBody* rb, const vector2x& contactPt, const vector2x& contactNormal) {
    vector2x n = contactNormal;
    rb->AddForce(n * ITOX(2000));
}

void Stricker::OnRender() {
    glPushMatrix();
    glTranslatef(XTOF(this->position.x), XTOF(this->position.y), 0);
    glColor3f(0.7f, 0.5, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexBuffer);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}
