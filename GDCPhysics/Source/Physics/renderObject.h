#pragma once
//
//  renderObject.h
//  Copyright 2018 GDCPhysics
//  GDCPhysics
//
//  Created by Arun Mudaliar.
//
//

class RenderObject {
public:
    RenderObject() {}
    virtual ~RenderObject() {}
    
    void draw() {
        OnRender();
    }
    
protected:
    virtual void OnRender(){}
};
