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
    
protected:
    virtual void OnRender();
    
    vector2x size;
    vector2x position;
    float vertexBuffer[4*2];
};
#endif /* boxCollider_hpp */
