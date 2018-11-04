//
//  wall.hpp
//  GDCPhysics
//
//  Created by Arun A on 03/11/18.
//  Copyright © 2018 Arun A. All rights reserved.
//

#ifndef wall_hpp
#define wall_hpp

#include "../Physics/colliders/boxCollider.hpp"

class Wall : public BoxCollider {
public:
    enum WALL_TYPE {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        WALL_TYPE_MAX
    };
    
    Wall();
    virtual ~Wall();

    void SetWallType(WALL_TYPE type)    { this->type = type; }
    
protected:
    void OnCollidedWithRB(RigidBody* rb, const vector2x& contactPt, const vector2x& contactNormal) override;
    void OnRender() override;
    
    WALL_TYPE type;
};
#endif /* wall_hpp */
