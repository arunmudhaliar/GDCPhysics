//
//  ball.cpp
//  Copyright 2018 GDCPhysics
//  GDCPhysics
//
//  Created by Arun Mudaliar.
//
//

#include "ball.h"

Ball::Ball() {
    this->size = 0;
    memset(vertexBuffer, 0 , sizeof(vertexBuffer));
}

Ball::~Ball() {
}

void Ball::initBall(float size, const vector2x& pos) {
    this->size = size;
    for(int xx=36-1; xx>=0; xx--) {
        float cs=size*cos(DEG2RAD((float)xx*10));
        float sn=size*sin(DEG2RAD((float)xx*10));
        vertexBuffer[xx*2+0]=cs;
        vertexBuffer[xx*2+1]=sn;
    }
    SetRBPosition(pos, true);
}

void Ball::OnRender() {
    auto p = getOpenGLMatrix();
    glPushMatrix();
    glTranslatef(XTOF(p[12]), XTOF(p[13]), XTOF(p[14]));
    glColor3f(1, 1, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexBuffer);
//    glEnableClientState(GL_COLOR_ARRAY);
//    glColorPointer(4, GL_FLOAT, 0, vbuffer->m_cszVertexColorList);
    glDrawArrays(GL_LINE_LOOP, 0, 36);
//    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void Ball::UpdatePositionFromRB(const vector2x& displacement) {
    updatePositionx(displacement.x, displacement.y, 0);
}

void Ball::SetPositionFromRB(const vector2x& pos) {
    setPositionx(pos);
}
