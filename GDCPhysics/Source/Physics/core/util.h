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
#include <string>
#include <vector>

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
    
    static void splitString(const std::string& str, std::vector<std::string>& cont, char delim = '|') {
        std::size_t current, previous = 0;
        current = str.find(delim);
        while (current != std::string::npos) {
            cont.push_back(str.substr(previous, current - previous));
            previous = current + 1;
            current = str.find(delim, previous);
        }
        cont.push_back(str.substr(previous, current - previous));
    }
    
    static std::string stringFormat(const char *fmt, ...) {
        std::vector<char> str(150, '\0');
        va_list ap;
        while (1) {
            va_start(ap, fmt);
            auto n = vsnprintf(str.data(), str.size(), fmt, ap);
            va_end(ap);
            if ((n > -1) && (size_t(n) < str.size())) {
                return str.data();
            }
            if (n > -1)
                str.resize(n + 1);
            else
                str.resize(str.size() * 2);
        }
        return str.data();
    }
    
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first)
        {
            return str;
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
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
