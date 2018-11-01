//
//  solver.hpp
//  GDCPhysics
//
//  Created by Arun A on 31/10/18.
//  Copyright © 2018 Arun A. All rights reserved.
//

#ifndef solver_hpp
#define solver_hpp

#include <vector>

#include "rigidBody.h"
#include "colliders/boxCollider.hpp"

class Solver {
public:
    Solver();
    ~Solver();
    
    void AddRigidBody(RigidBody* rb);
    void AddBoxCollider(BoxCollider* collider);
    void UpdateSolver();
    
private:
    std::vector<RigidBody*> rigidBodies;
    std::vector<BoxCollider*> boxColliders;
    
    void CheckCollisions(vector2x& newPos, intx radiusSq, bool& collisionHappened, vector2x& contactNormal);
};
#endif /* solver_hpp */
