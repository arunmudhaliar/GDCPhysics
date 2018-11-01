//
//  util.h
//  waves2
//
//  Created by arun on 07/04/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef UTIL_H
#define UTIL_H

#include "vector2.h"

class util
{
public:
    static vector2f closestPointOnLine(vector2f& p, vector2f& a, vector2f& b)
    {
        // a-b is the line, p the point in question
        vector2f c = p-a;
        vector2f V = b-a; 
        float d = V.Length();
        
        V.Normalize();
        float t = V.Dot(c);
        
        // Check to see if 奏?is beyond the extents of the line segment
        if (t < 0.0f) return (a);
        if (t > d) return (b);
        
        // Return the point between 疎?and 礎?		//set length of V to t. V is normalized so this is easy
        V.x = V.x * t;
        V.y = V.y * t;
        
        return (a+V);	
    }
    
    static vector2x closestPointOnLine(vector2x& p, vector2x& a, vector2x& b)
    {
        // a-b is the line, p the point in question
        vector2x c(p-a);
        vector2x V(b-a); 
        __int64_t d = V.lengthx();
        
        V.normalizex();
        __int64_t t = V.dotx(c);
        
        // Check to see if 奏?is beyond the extents of the line segment
        if (t < 0) return (a);
        if (t > d) return (b);
        
        // Return the point between 疎?and 礎?		//set length of V to t. V is normalized so this is easy
        V = V * (int)t;
        
        return (a+V);	
    }
    
//    static bool twoLineSegmentIntersection(vector2f& a, vector2f& b, vector2f& c, vector2f& d, vector2f& i1, vector2f& i2)
//    {
//        float n1 = b.x-c.x;
//        float d1 = d.x-c.x;
//        float n2 = d.y-a.y;
//        float d2 = b.y-a.y;
//        
//        if(abs(d1)<0.00000001f || abs(d2)<0.00000001f) return false;
//        
//        float s=n2/d2;
//        float t=n1/d1;
//        
//        if(s<0 || s>1.0f) return false;
//        if(t<0 || t>1.0f) return false;
//        
//        i1=a+((b-a)*s);
//        i2=c+((d-c)*t);
//        
//        return true;
//    }
};

#endif
