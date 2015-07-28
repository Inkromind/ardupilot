/*
 * SegmentDistance.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

// Copyright 2001 softSurfer, 2012 Dan Sunday
// This code may be freely used, distributed and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

#include "SegmentDistance.h"

#define SMALL_NUM   0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define norm(v)    sqrt(dot(v,v))  // norm = length of  vector
#define d(u,v)     norm(u-v)        // distance = norm of difference
#define abs(x)     ((x) >= 0 ? (x) : -(x))   //  absolute value

// dist3D_Segment_to_Segment(): get the 3D minimum distance between 2 segments
//    Input:  two 3D line segments S1 (<Vector3f s1p0, Vector3f s1p1>) and S2
//            (<Vector3f s2p0, Vector3f s2p1>)
//    Return: the shortest distance between S1 and S2
float dist3D_Segment_to_Segment(Vector3f s1p0, Vector3f s1p1, Vector3f s2p0, Vector3f s2p1)
{
    Vector3f   u = s1p1 - s1p0;
    Vector3f   v = s2p1 - s2p0;
    Vector3f   w = s1p0 - s2p0;
    float    a = dot(u,u);         // always >= 0
    float    b = dot(u,v);
    float    c = dot(v,v);         // always >= 0
    float    d = dot(u,w);
    float    e = dot(v,w);
    float    D = a*c - b*b;        // always >= 0
    float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
    float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
    if (D < SMALL_NUM) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else {                 // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    }
    else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d +  b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
    tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);

    // get the difference of the two closest points
    Vector3f   dP = w + (u * sc) - (v * tc);  // =  S1(sc) - S2(tc)

    return norm(dP);   // return the closest distance
}

// dist_Point_to_Segment(): get the distance of a point to a segment
//     Input:  a Point p (Vector3f) and a Segment S
//             (<Vector3f sp0, Vector3f sp1>)
//     Return: the shortest distance from P to S
float dist_Point_to_Segment(Vector3f p, Vector3f sp0, Vector3f sp1)
{
     Vector3f v = sp1 - sp0;
     Vector3f w = p - sp0;

     float c1 = dot(w,v);
     if ( c1 <= 0 )
          return d(p, sp0);

     float c2 = dot(v,v);
     if ( c2 <= c1 )
          return d(p, sp1);

     float b = c1 / c2;
     Vector3f Pb = sp0 + v * b;
     return d(p, Pb);
}

float dist_Point_to_Point(Vector3f p1, Vector3f p2) {
    return d(p1, p2);
}
