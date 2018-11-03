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
    
    void InitSolver();
    void AddRigidBody(RigidBody* rb);
    void AddBoxCollider(BoxCollider* collider);
    void RemoveRigidBody(RigidBody* rb);
    void RemoveBoxCollider(BoxCollider* collider);
    void UpdateSolver();
    
    unsigned long GetElapsedTime()  { return elapsedTime; }
    
private:
    std::vector<RigidBody*> rigidBodies;
    std::vector<BoxCollider*> boxColliders;
    
    void UpdatePhysics(intx t, intx fixedDT);
    void CheckCollisions(RigidBody* rb, vector2x& newPos, intx radiusSq, bool& collisionHappened, vector2x& contactNormal, std::vector<BoxCollider*>& colliders);
    
    // timer
    unsigned long elapsedTime;
    unsigned long currentTime;
    unsigned long accumulator;
};
#endif /* solver_hpp */
