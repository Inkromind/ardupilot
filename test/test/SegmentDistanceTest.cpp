/*
 * SegmentDistanceTest.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"

//Class being tested
#include "../../libraries/AMW_Corridors/SegmentDistance.h"

//Includes
#include "../../libraries/AP_Math/vector3.h"

TEST_GROUP(SegmentDistance)
{
};

TEST(SegmentDistance, PointToPoint)
{
   CHECK_EQUAL(2, dist_Point_to_Point(Vector3f(0,0,0), Vector3f(2,0,0)));
   CHECK_EQUAL(2, dist_Point_to_Point(Vector3f(0,0,0), Vector3f(0,2,0)));
   CHECK_EQUAL(2, dist_Point_to_Point(Vector3f(0,0,0), Vector3f(0,0,2)));
   CHECK_EQUAL(0, dist_Point_to_Point(Vector3f(0,0,0), Vector3f(0,0,0)));
   CHECK_EQUAL((Vector3f(1,2,3)-Vector3f(5,7,9)).length(), dist_Point_to_Point(Vector3f(1,2,3), Vector3f(5,7,9)));
}

TEST(SegmentDistance, PointToSegmentOnSegment)
{
    CHECK_EQUAL(0, dist_Point_to_Segment(Vector3f(5,1,1), Vector3f(1,1,1), Vector3f(7,1,1)));
}

TEST(SegmentDistance, PointToSegmentInExtension)
{
    CHECK_EQUAL(2, dist_Point_to_Segment(Vector3f(9,1,1), Vector3f(1,1,1), Vector3f(7,1,1)));
}

TEST(SegmentDistance, PointToSegment)
{
    CHECK_EQUAL((Vector3f(5,4,3)-Vector3f(5,1,1)).length(), dist_Point_to_Segment(Vector3f(5,4,3), Vector3f(1,1,1), Vector3f(7,1,1)));
}

TEST(SegmentDistance, PointToSegment2)
{
    CHECK_EQUAL((Vector3f(9,10,11)-Vector3f(7,1,1)).length(), dist_Point_to_Segment(Vector3f(9,10,11), Vector3f(1,1,1), Vector3f(7,1,1)));
}

TEST(SegmentDistance, PointToSegmentReallyClose)
{
    CHECK_EQUAL((Vector3f(8,2,1)-Vector3f(7,1,1)).length(), dist_Point_to_Segment(Vector3f(8,2,1), Vector3f(1,1,1), Vector3f(7,1,1)));
}

TEST(SegmentDistance, SegmentToSegmentParallelOverlap)
{
    CHECK_EQUAL((Vector3f(4,1,1)-Vector3f(4,2,3)).length(), dist3D_Segment_to_Segment(Vector3f(1, 1, 1), Vector3f(7, 1, 1), Vector3f(4,2,3), Vector3f(10,2,3)));
}

TEST(SegmentDistance, SegmentToSegmentParallelNoOverlap)
{
    CHECK_EQUAL((Vector3f(7,1,1)-Vector3f(9,10,11)).length(), dist3D_Segment_to_Segment(Vector3f(1, 1, 1), Vector3f(7, 1, 1), Vector3f(9,10,11), Vector3f(20,10,11)));
}

TEST(SegmentDistance, SegmentToSegmentCrossOverlap)
{
    CHECK_EQUAL((Vector3f(3,1,1)-Vector3f(3,1,5)).length(), dist3D_Segment_to_Segment(Vector3f(1, 1, 1), Vector3f(7, 1, 1), Vector3f(3,0,5), Vector3f(3,8,5)));
}

TEST(SegmentDistance, SegmentToSegmentCrossNoOverlap)
{
    CHECK_EQUAL((Vector3f(3,1,1)-Vector3f(3,4,5)).length(), dist3D_Segment_to_Segment(Vector3f(1, 1, 1), Vector3f(7, 1, 1), Vector3f(3,4,5), Vector3f(3,8,5)));
}

TEST(SegmentDistance, SegmentToSegmentExtension)
{
    CHECK_EQUAL((Vector3f(7,1,1)-Vector3f(10,1,1)).length(), dist3D_Segment_to_Segment(Vector3f(1, 1, 1), Vector3f(7, 1, 1), Vector3f(10,1,1), Vector3f(20,1,2)));
}

