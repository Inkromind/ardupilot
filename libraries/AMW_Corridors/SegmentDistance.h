/*
 * SegmentDistance.h
 *
 *  Created on: 19-jul.-2015
 *      Author: Arne
 */

// Copyright 2001 softSurfer, 2012 Dan Sunday
// This code may be freely used, distributed and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

#ifndef SEGMENTDISTANCE_H_
#define SEGMENTDISTANCE_H_

#include "../AP_Math/vector3.h"

// dist3D_Segment_to_Segment(): get the 3D minimum distance between 2 segments
//    Input:  two 3D line segments S1 (<Vector3f s1p0, Vector3f s1p1>) and S2
//            (<Vector3f s2p0, Vector3f s2p1>)
//    Return: the shortest distance between S1 and S2
float dist3D_Segment_to_Segment(Vector3f s1p0, Vector3f s1p1, Vector3f s2p0, Vector3f s2p1);

// dist_Point_to_Segment(): get the distance of a point to a segment
//     Input:  a Point p (Vector3f) and a Segment S
//             (<Vector3f sp0, Vector3f sp1>)
//     Return: the shortest distance from P to S
float dist_Point_to_Segment(Vector3f p, Vector3f sp0, Vector3f sp1);

float dist_Point_to_Point(Vector3f p1, Vector3f p2);

#endif /* SEGMENTDISTANCE_H_ */
