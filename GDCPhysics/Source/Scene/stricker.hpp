//
//  stricker.hpp
//  GDCPhysics
//
//  Created by Arun A on 03/11/18.
//  Copyright © 2018 Arun A. All rights reserved.
//

#ifndef stricker_hpp
#define stricker_hpp

#include "../Physics/colliders/boxCollider.hpp"

class Stricker : public BoxCollider {
public:
    Stricker();
    virtual ~Stricker();
    
    void MoveUp();
    void MoveDown();
    void SetWindowHeight(float height)    { windowHeight = height; }
protected:
    void OnCollidedWithRB(RigidBody* rb, const vector2x& contactPt, const vector2x& contactNormal) override;
    void OnRender() override;
    
    float windowHeight;
};
#endif /* stricker_hpp */
