#pragma once
//
//  renderObject.h
//  Copyright 2018 GDCPhysics
//  GDCPhysics
//
//  Created by Arun Mudaliar.
//
//

#include "core/transformx.h"

class RenderObject : public transformx {
public:
    RenderObject() {}
    virtual ~RenderObject() {}
    
    void draw() {
        OnRender();
    }
    
protected:
    virtual void OnRender(){}
};
