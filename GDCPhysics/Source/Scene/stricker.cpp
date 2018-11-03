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
    
//    Ball* ball = dynamic_cast<Ball*>(rb);
//    if (ball) {
//        ball->SendBallState();
//    }
    
//    auto pos = GetRBPosition();
//    auto velocity = GetRBVelocity();
//    auto msg = util::stringFormat("ball|%d,%d,%d,%d", pos.x, pos.y, velocity.x, velocity.y);
//    NetworkManager::GetInstance().SendMessage(msg);
}
