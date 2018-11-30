//
//  solver.cpp
//  GDCPhysics
//
//  Created by Arun A on 31/10/18.
//  Copyright © 2018 Arun A. All rights reserved.
//

#include "solver.hpp"
#include "core/util.h"
#include "core/pxMath.h"
#include "core/Timer.h"

#define FIXED_DT_READABLE    16             // 60 fps
#define FIXED_DT_THRESHOLD_READABLE    40   // 25 fps
#define FIXED_DT    FTOX(0.016f)            // 60 fps
#define GRAVITY -ITOX(100)

Solver::Solver() {
    this->fixedDTObserver = nullptr;
    this->elapsedTime = 0;
    this->currentTime = 0;
    this->accumulator = 0;
}

Solver::~Solver() {
}

void Solver::InitSolver(FixedUpdateObserver* fixedDTObserver) {
    this->fixedDTObserver = fixedDTObserver;
    this->elapsedTime = 0;
    this->currentTime = Timer::getCurrentTimeInMilliSec();
    this->accumulator = 0;
}

void Solver::AddRigidBody(RigidBody* rb) {
    if (std::find(this->rigidBodies.begin(), this->rigidBodies.end(), rb) == this->rigidBodies.end()) {
        this->rigidBodies.push_back(rb);
    }
}

void Solver::AddBoxCollider(BoxCollider* collider) {
    if (std::find(this->boxColliders.begin(), this->boxColliders.end(), collider) == this->boxColliders.end()) {
        this->boxColliders.push_back(collider);
    }
}

void Solver::RemoveRigidBody(RigidBody* rb) {
    this->rigidBodies.erase(std::remove(this->rigidBodies.begin(), this->rigidBodies.end(), rb), this->rigidBodies.end());
}

void Solver::RemoveBoxCollider(BoxCollider* collider) {
    this->boxColliders.erase(std::remove(this->boxColliders.begin(), this->boxColliders.end(), collider), this->boxColliders.end());
}

void Solver::UpdateSolver() {
    unsigned long newTime = Timer::getCurrentTimeInMilliSec();
    unsigned long frameTime = newTime - this->currentTime;
    if ( frameTime > FIXED_DT_THRESHOLD_READABLE ) {
        frameTime = FIXED_DT_THRESHOLD_READABLE;
    }
    this->currentTime = newTime;
    this->accumulator += frameTime;
    
    while ( this->accumulator >= FIXED_DT_READABLE ) {
        if (this->fixedDTObserver) {
            this->fixedDTObserver->OnFixedUpdate(FIXED_DT);
        }
        UpdatePhysics(FTOX(this->elapsedTime/1000.0f), FIXED_DT);
        this->elapsedTime += FIXED_DT_READABLE;
        this->accumulator -= FIXED_DT_READABLE;
    }
}

void Solver::UpdatePhysics(intx t, intx fixedDT) {
    vector2x outDisplacement;
    vector2x outVelocity;
    std::vector<BoxCollider*> outColliders;
    for (auto rb : this->rigidBodies) {
        rb->SimulateStep(fixedDT, outDisplacement, outVelocity);
        rb->ClearForce();
        auto vel = rb->GetRBVelocity();
        auto pos = rb->GetRBPosition();
        auto newPos = pos+outDisplacement;
        bool collisionHappened = false;
        vector2x contactNormal;
        CheckCollisions(rb, newPos, rb->GetRadiusSq(), collisionHappened, contactNormal, outColliders);
        if (collisionHappened) {
            vector2x oldVel = vel+outVelocity;
            int vel_mag = oldVel.lengthx();
//            printf("vel %f\n", XTOF(vel_mag));
            oldVel.normalizex();
            
            intx impulseForce = vel_mag;
            if (vel_mag > FTOX(5.0f)) {
                if (vel_mag > ITOX(600)) {
                    vel_mag = ITOX(600);
                }
                impulseForce = MULTX(vel_mag, FTOX(25.0f));
                if (impulseForce > ITOX(8000)) {
                    impulseForce = ITOX(8000);
                }
            }
            
            // (amudaliar) : uncomment this code for gradual reduction of velocity.
            // vel_mag = MULTX(vel_mag, FTOX(0.8f));
            //
            
            auto newVel = (oldVel + contactNormal) * vel_mag;
            rb->SetRBVelocity(newVel);
            newPos = newPos + contactNormal*FTOX(0.1f);
            rb->SetRBPosition(newPos, true);
            
            rb->AddForce(contactNormal * impulseForce);
            rb->TriggerCollisionEvent(outColliders);
        } else {
            rb->SetRBVelocity(vel+outVelocity);
            rb->SetRBPosition(pos+outDisplacement, true);
        }
        
        intx gravity = MULTX(GRAVITY, rb->GetRBMass());
        rb->AddForce(vector2x(0, gravity));
    }
}

void Solver::CheckCollisions(RigidBody* rb, vector2x& newPos, intx radiusSq, bool& collisionHappened, vector2x& contactNormal, std::vector<BoxCollider*>& colliders) {
//    printf("newPos %f, %f\n", XTOF(newPos.x), XTOF(newPos.y));
    collisionHappened = false;
    int collision_check_cntr=5;
    colliders.clear();
    //bool bCollision=true;
    while(collision_check_cntr--) {
        bool bPenitration = false;
        vector2x avgPos;
        vector2x avgNrml;
        int cnt=0;
        
        for(auto boxCollider : this->boxColliders) {
            vector2x bottomLeft(boxCollider->GetBottomLeft());
            vector2x bottomRight(boxCollider->GetBottomRight());
            vector2x topLeft(boxCollider->GetTopLeft());
            vector2x topRight(boxCollider->GetTopRight());

            vector2x closestPt[4];
            closestPt[0]=(util::closestPointOnLine(newPos, bottomLeft, bottomRight));
            closestPt[1]=(util::closestPointOnLine(newPos, bottomRight, topRight));
            closestPt[2]=(util::closestPointOnLine(newPos, topRight, topLeft));
            closestPt[3]=(util::closestPointOnLine(newPos, topLeft, bottomLeft));

            // check for penetration
            // Note: we are not checking penitration for now.
            bPenitration = false;
            
            //which one is the closest
            __int64_t closest_length=GX_MAX_INT;
            int closest_index=-1;
            
            for (int l=0; l<4; l++) {
                vector2x diff(newPos-closestPt[l]);
                __int64_t length=diff.lengthSquaredx();
                if(length<closest_length) {
                    closest_length=length;
                    closest_index=l;
                    
                    if(bPenitration) {
                        diff=-diff;
                    }
                    
                    if(closest_length <= radiusSq) {
                        //collision occured
                        diff.normalizex();
                        intx radius = pxMath::SQRT((__int64_t)radiusSq);
                        auto contactPt = closestPt[closest_index];
                        vector2x calc_Pos(contactPt + diff*radius);
                        avgPos+=calc_Pos;
                        avgNrml+=diff;
                        cnt++;
                        colliders.push_back(boxCollider);
                        boxCollider->CollidedWithRB(rb, contactPt, diff);
                    }
                }
            }//for
            //~circle-rectangle collision
        }
        
        if(cnt) {
            avgPos.x=DIVX(avgPos.x,ITOX(cnt));
            avgPos.y=DIVX(avgPos.y,ITOX(cnt));
            // normali
            avgNrml.normalizex();
            contactNormal = avgNrml;
            newPos=avgPos;    //this will cause the actor to come to a halt then move away, so i commented this line
            collisionHappened = true;
        } else {
            break;  //break the loop
        }
    }
}
