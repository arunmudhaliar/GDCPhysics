//
//  boxCollider.hpp
//  GDCPhysics
//
//  Created by Arun A on 31/10/18.
//  Copyright © 2018 Arun A. All rights reserved.
//

#ifndef boxCollider_hpp
#define boxCollider_hpp

#include "../core/vector2x.h"
#include "../renderObject.h"

class BoxCollider : public RenderObject {
public:
    BoxCollider();
    ~BoxCollider();
    
    void InitBoxCollider(const vector2x& size, const vector2x& pos);
    
    inline vector2x GetBottomLeft()    { return this->position + vector2x(FTOX(vertexBuffer[0]), FTOX(vertexBuffer[1])); }
    inline vector2x GetBottomRight()   { return this->position + vector2x(FTOX(vertexBuffer[2]), FTOX(vertexBuffer[3])); }
    inline vector2x GetTopLeft()       { return this->position + vector2x(FTOX(vertexBuffer[6]), FTOX(vertexBuffer[7])); }
    inline vector2x GetTopRight()      { return this->position + vector2x(FTOX(vertexBuffer[4]), FTOX(vertexBuffer[5])); }
protected:
    virtual void OnRender();
    
    vector2x size;
    vector2x position;
    float vertexBuffer[4*2];
};
#endif /* boxCollider_hpp */
