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

#define FIXED_DT    FTOX(0.022f)
#define GRAVITY -ITOX(10)

Solver::Solver() {
    
}

Solver::~Solver() {
    
}

void Solver::AddRigidBody(RigidBody* rb) {
    this->rigidBodies.push_back(rb);
}

void Solver::AddBoxCollider(BoxCollider* collider) {
    this->boxColliders.push_back(collider);
}

void Solver::UpdateSolver() {
    vector2x outDisplacement;
    vector2x outVelocity;
    for (auto rb : this->rigidBodies) {
        rb->SimulateStep(FIXED_DT, outDisplacement, outVelocity);
        rb->ClearForce();
        auto vel = rb->GetRBVelocity();
        auto pos = rb->GetRBPosition();
        auto newPos = pos+outDisplacement;
        bool collisionHappened = false;
        vector2x contactNormal;
        CheckCollisions(newPos, FTOX((20.0f*20.0f)), collisionHappened, contactNormal);
        if (collisionHappened) {
            //rb->SetRBVelocity((vel+outVelocity) * FTOX(0.65f));
            newPos = newPos + contactNormal*FTOX(4.0f);
            rb->SetRBPosition(newPos, true);
            int vel_mag = vel.lengthx();
            vel_mag = MULTX(vel_mag, FTOX(0.15f));
            rb->AddForce(contactNormal * (FTOX(4000.0f)+vel_mag));
        } else {
            rb->SetRBVelocity(vel+outVelocity);
            rb->SetRBPosition(pos+outDisplacement, true);
        }
        
        rb->AddForce(vector2x(0, GRAVITY));
    }
}

void Solver::CheckCollisions(vector2x& newPos, intx radiusSq, bool& collisionHappened, vector2x& contactNormal) {
    printf("newPos %f, %f\n", XTOF(newPos.x), XTOF(newPos.y));
    collisionHappened = false;
    int collision_check_cntr=5;
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
            bPenitration = false;
            
            //which one is the closest
            int closest_length=GX_MAX_INT;
            int closest_index=-1;
            
            for (int l=0; l<4; l++)
            {
                vector2x diff(newPos-closestPt[l]);
                __int64_t length=diff.lengthSquaredx();
                if(length<closest_length)
                {
                    closest_length=length;
                    closest_index=l;
                    
                    if(bPenitration)
                    {
                        diff=-diff;
                    }
                    
                    if (l==0) {
                        float lq = XTOF(closest_length);
                        printf("closestPt %f, %f, r:%f\n", XTOF(closestPt[0].x), XTOF(closestPt[0].y), sqrt(lq));
//                        printf("closest_length %f\n", XTOF(closest_length));
                    }
                    if(closest_length <= radiusSq)
                    {
                        //collision occured
                        diff.normalizex();
    //                    float val=(ACTOR_COLLISION_RADIUS*m_pCommonDataPtr->getPortingMultiplier())+0.1f;
                        intx radius = pxMath::SQRT((__int64_t)radiusSq);
                        vector2x calc_Pos(closestPt[closest_index] + diff*radius);
                        avgPos+=calc_Pos;
                        avgNrml+=diff;
                        cnt++;
                        //DEBUG_PRINT("(%d), closestPt(%f, %f), calc_pos(%f, %f)", l, closestPtf[closest_index].x, closestPtf[closest_index].y, calc_Pos.x, calc_Pos.y);
                    }
                }
            }//for
            //~circle-rectangle collision
        }
        
        if(cnt)
        {
            avgPos.x=DIVX(avgPos.x,ITOX(cnt));
            avgPos.y=DIVX(avgPos.y,ITOX(cnt));
            
            // normali
            avgNrml.normalizex();
            contactNormal = avgNrml;
            
            newPos=avgPos;    //this will cause the actor to come to a halt then move away, so i commented this line
            collisionHappened = true;
    //        bPositionUpdated=true;
            //            bCollisionOccuredDuringThisUpdate=true;
            //DEBUG_PRINT("collision occured %d, avgPos(%f, %f)", cnt, avgPosf.x, avgPosf.x);
        }
        else
        {
            break;  //break the loop
        }
    }
}

void CheckCollisionsTest() {
#if 0
    int collision_check_cntr=5;
    //bool bCollision=true;
    while(collision_check_cntr--)
    {
        oldPos=newPos;
        //DEBUG_PRINT("newPos(%f, %f)", newPosf.x, newPosf.y);
        vector2x avgPos;
        int cnt=0;
        
        int row0=iamontile->getTileID()/getMapPtr()->getCol();
        int col0=iamontile->getTileID()%getMapPtr()->getCol();
        int min_row=row0-1;
        int min_col=col0-1;
        int max_row=row0+1;
        int max_col=col0+1;
        
        min_row=(min_row<0)?0:min_row;
        min_col=(min_col<0)?0:min_col;
        max_row=(max_row>=getMapPtr()->getRow())?getMapPtr()->getRow()-1:max_row;
        max_col=(max_col>=getMapPtr()->getCol())?getMapPtr()->getCol()-1:max_col;
        
        for (int y = min_row; y <= max_row; y++)
        {
            for (int x = min_col; x <= max_col; x++)
            {
                ECTile* tile=getMapPtr()->getTile(y*getMapPtr()->getCol()+x);
                if(tile->isWalkable()) continue;
                CircleCollider* collider=tile->getCircleCollider();
                if(!collider)
                {
                    //circle-rectangle collision
                    vector2x left(tile->getLeftWorld());
                    vector2x right(tile->getRightWorld());
                    vector2x top(tile->getTopWorld());
                    vector2x bottom(tile->getBottomWorld());
                    
                    //check for penitration
                    bool bPenitration=true;
                    if(newPos.x<left.x || newPos.y<top.y || newPos.x>right.x || newPos.y>bottom.y)
                        bPenitration=false;
                    
                    //                    if(bPenitration)
                    //                    {
                    //                        DEBUG_PRINT("=======================PENITRATION OCCURED=================");
                    //                    }
                    
                    vector2x closestPt[4];
                    
                    closestPt[0]=(util::closestPointOnLine(newPos, left, top));
                    closestPt[1]=(util::closestPointOnLine(newPos, top, right));
                    closestPt[2]=(util::closestPointOnLine(newPos, right, bottom));
                    closestPt[3]=(util::closestPointOnLine(newPos, bottom, left));
                    
                    //for debug
                    //                    tile->debug_closestPtf[0].set(XTOF(closestPt[0].x), XTOF(closestPt[0].y));
                    //                    tile->debug_closestPtf[1].set(XTOF(closestPt[1].x), XTOF(closestPt[1].y));
                    //                    tile->debug_closestPtf[2].set(XTOF(closestPt[2].x), XTOF(closestPt[2].y));
                    //                    tile->debug_closestPtf[3].set(XTOF(closestPt[3].x), XTOF(closestPt[3].y));
                    //
                    
                    //which one is the closest
                    int closest_length=GX_MAX_INT;
                    int closest_index=-1;
                    
                    for (int l=0; l<4; l++)
                    {
                        vector2x diff(newPos-closestPt[l]);
                        __int64_t length=diff.lengthSquaredx();
                        if(length<closest_length)
                        {
                            closest_length=length;
                            closest_index=l;
                            
                            if(bPenitration)
                            {
                                diff=-diff;
                            }
                            
                            if(closest_length<=m_xActorRadiusSq)
                            {
                                //collision occured
                                diff.normalizex();
                                float val=(ACTOR_COLLISION_RADIUS*m_pCommonDataPtr->getPortingMultiplier())+0.1f;
                                vector2x calc_Pos(closestPt[closest_index] + (diff*val));
                                avgPos+=calc_Pos;
                                cnt++;
                                //DEBUG_PRINT("(%d), closestPt(%f, %f), calc_pos(%f, %f)", l, closestPtf[closest_index].x, closestPtf[closest_index].y, calc_Pos.x, calc_Pos.y);
                            }
                        }
                    }//for
                    //~circle-rectangle collision
                }
                else
                {
                    vector2x center(tile->getCenterWorldx());
                    vector2x cpos(center.x+ITOX(collider->getOffsetX()), center.y+ITOX(collider->getOffsetY()));
                    
                    vector2x diff(newPos-cpos);
                    __int64_t length=diff.lengthSquaredx();
                    int check_val=(ACTOR_COLLISION_RADIUS*m_pCommonDataPtr->getPortingMultiplier())+collider->getRadius();
                    if(length<=ITOX((check_val*check_val)))
                    {
                        diff.normalizex();
                        float val=(ACTOR_COLLISION_RADIUS*m_pCommonDataPtr->getPortingMultiplier())+collider->getRadius()+0.1f;
                        vector2x calc_Pos(cpos + diff*val);
                        avgPos+=calc_Pos;
                        cnt++;
                    }
                }
            }
        }
        //for entire map
        
        if(cnt)
        {
            avgPos.x=DIVX(avgPos.x,ITOX(cnt));
            avgPos.y=DIVX(avgPos.y,ITOX(cnt));
            
            newPos=avgPos;    //this will cause the actor to come to a halt then move away, so i commented this line
            bPositionUpdated=true;
            //            bCollisionOccuredDuringThisUpdate=true;
            //DEBUG_PRINT("collision occured %d, avgPos(%f, %f)", cnt, avgPosf.x, avgPosf.x);
        }
        else
        {
            break;  //break the loop
        }
    }//while
#endif
}
