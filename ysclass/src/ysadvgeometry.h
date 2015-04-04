/* ////////////////////////////////////////////////////////////

YS Class Library
Copyright (c) 2014 Soji Yamakawa.  All rights reserved.

YS Class Library is an open-source class-library project since 1999.
It has been used and battle-tested in commercial, research, and free programs.
Please visit the following URL for more details.

http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File Name: ysadvgeometry.h
//////////////////////////////////////////////////////////// */

#ifndef YSADVGEOMETRY_IS_INCLUDED
#define YSADVGEOMETRY_IS_INCLUDED
/* { */

/*! \file */

#include "ysgeometry.h"
#include "ysarray.h"

/*! This function returns YSTRUE if the polygon given by p[0],p[1],...,p[np-1] is convex.
    \param np [In] Number of points of the polygon.
    \param p  [In] Points of the polygon.
    \param strictCheck [In] If it is YSTRUE, this function performs additional calculation to deal with a numerically difficult case.
*/
YSBOOL YsCheckConvex2(YSSIZE_T np,const YsVec2 p[],YSBOOL strictCheck=YSFALSE);

/*! Templated version of YsCheckConvex2. */
template <const int N>
YSBOOL YsCheckConvex2(const YsArray <YsVec2,N> &plg,YSBOOL strictCheck=YSFALSE)
{
	return YsCheckConvex2(plg.GetN(),plg,strictCheck);
}

/*! This function returns YSTRUE if the polygon given by p[0],p[1],...,p[np-1] is convex.
    \param np [In] Number of points of the polygon.
    \param p  [In] Points of the polygon.
    \param strictCheck [In] If it is YSTRUE, this function performs additional calculation to deal with a numerically difficult case.
*/
YSBOOL YsCheckConvex3(YSSIZE_T np,const YsVec3 p[],YSBOOL strictCheck=YSFALSE);

/*! Templated version of YsCheckConvex2. */
template <const int N>
YSBOOL YsCheckConvex3(const YsArray <YsVec3,N> &plg,YSBOOL strictCheck=YSFALSE)
{
	return YsCheckConvex3(plg.GetN(),plg,strictCheck);
}

/*! This function returns YSTRUE if the polygon given by p[0],p[1],...,p[np-1] is convex.
    This function tests the convexity based on only angles.  The result may become unstable
    when more than three points are nearly co-linear.
    \sa YsCheckConvex2
    \param np [In] Number of points of the polygon.
    \param p  [In] Points of the polygon.
*/
YSBOOL YsCheckConvexByAngle2(YSSIZE_T np,const YsVec2 *p);

/*! Templated version of YsCheckConvexByAngle2 */
template <const int N>
YSBOOL YsCheckConvexByAngle2(const YsArray <YsVec2,N> &plg)
{
	return YsCheckConvexByAngle2(plg.GetN(),plg);
}

/*! This function returns YSTRUE if the polygon given by p[0],p[1],...,p[np-1] is convex.
    This function tests the convexity based on only angles.  The result may become unstable
    when more than three points are nearly co-linear.
    \sa YsCheckConvex3
    \param np [In] Number of points of the polygon.
    \param p  [In] Points of the polygon.
*/
YSBOOL YsCheckConvexByAngle3(YSSIZE_T np,const YsVec3 *p);

/*! Templated version of YsCheckConvexByAngle3 */
template <const int N>
YSBOOL YsCheckConvexByAngle3(const YsArray <YsVec3,N> &plg)
{
	return YsCheckConvexByAngle3(plg.GetN(),plg);
}

/*! This function returns area of a triangle. */
double YsGetTriangleArea3(const YsVec3 &p1,const YsVec3 &p2,const YsVec3 &p3);

/*! This function returns area of a triangle. */
double YsGetTriangleArea2(const YsVec2 &p1,const YsVec2 &p2,const YsVec2 &p3);

/*! This function retusns area of the given polygon. */
double YsGetPolygonArea2(YSSIZE_T np,const YsVec2 p[]);

/*! Templated version of YsGetPolygonArea2. */
template <const int N>
double YsGetPolygonArea2(const YsArray <YsVec2,N> &plg)
{
	return YsGetPolygonArea2(plg.GetN(),plg);
}

/*! This function finds the largest triangle that consists of three points of the
    points of the given polygon and returns pointers to the three points.
    \param v [Out] The triangle will be returned in this.
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    */
YSRESULT YsGetLargestTriangleFromPolygon3(const YsVec3 *v[3],YSSIZE_T np,const YsVec3 p[]);

/*! Templated version of YsGetLargestTriangleFromPolygon3 */
template <const int N>
YSRESULT YsGetLargestTriangleFromPolygon3(const YsVec3 *v[3],const YsArray <YsVec3,N> &plg)
{
	return YsGetLargestTriangleFromPolygon3(v,plg.GetN(),plg);
}

/*! This function finds the largest triangle that consists of three 
    points of the given polygon and returns the three points of the triangle.
    \param v [Out] The triangle will be returned in this.
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    */
YSRESULT YsGetLargestTriangleFromPolygon3(YsVec3 v[3],YSSIZE_T np,const YsVec3 p[]);

/*! Templated version of YsGetLargestTriangleFromPolygon3 */
template <const int N>
YSRESULT YsGetLargestTriangleFromPolygon3(YsVec3 v[3],const YsArray <YsVec3,N> &plg)
{
	return YsGetLargestTriangleFromPolygon3(v,plg.GetN(),plg);
}

/*! This function finds the largest triangle that consists of three points of
    the given polygon and returns the three points of the triangle.
    \param v [Out] The triangle will be returned in this.
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    */
YSRESULT YsGetLargestTriangleFromPolygon2(const YsVec2 *v[3],YSSIZE_T np,const YsVec2 p[]);

/*! Templated version of YsGetLargestTriangleFromPolygon2 */
template <const int N>
YSRESULT YsGetLargestTriangleFromPolygon2(const YsVec2 *v[3],const YsArray <YsVec2,N> &plg)
{
	return YsGetLargestTriangleFromPolygon2(v,plg.GetN(),plg);
}

/*! This function finds the largest triangle that consists of three
    points of the given polygon and returns the three points of the triangle.
    \param v [Out] The triangle will be returned in this.
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    */
YSRESULT YsGetLargestTriangleFromPolygon2(YsVec2 v[3],YSSIZE_T np,const YsVec2 p[]);

/*! Templated version of YsGetLargestTriangleFromPolygon2 */
template <const int N>
YSRESULT YsGetLargestTriangleFromPolygon2(YsVec2 v[3],const YsArray <YsVec2,N> &plg)
{
	return YsGetLargestTriangleFromPolygon2(v,plg.GetN(),plg);
}

/*! This function calculates normal vector of the polygon based on the largest triangle
    that consists of three points of the given polygon.
    If successful, the returned normal is a unit vector.
    \sa YsGetLargestTriangleFromPolygon3
    \param nom [Out] Calculated normal vector
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    */
YSRESULT YsGetAverageNormalVector(YsVec3 &nom,YSSIZE_T np,const YsVec3 p[]);

/*! Templated version of YsGetAverageNormalVector. */
template <const int N>
YSRESULT YsGetAverageNormalVector(YsVec3 &nom,const YsArray <YsVec3,N> &plVtPos)
{
	return YsGetAverageNormalVector(nom,plVtPos.GetN(),plVtPos);
}

/*! This function returns YSTRUE if the projection of the reference point 
    on the line that passes through the given two end points is between 
    the two end points or is lying on one of the end points.  It returns
    YSFALSE otherwise.
	\param ref [In] Reference point
	\param p1 [In] One of the end points
	\param p2 [In] The other end point
    */
YSBOOL YsCheckInBetween2(const YsVec2 &ref,const YsVec2 &p1,const YsVec2 &p2);

/*! This function returns YSTRUE if the projection of the reference point 
    on the line that passes through the given two end points is between 
    the two end points or is lying on one of the end points.  It returns
    YSFALSE otherwise.
	\param ref [In] Reference point
	\param p1 [In] One of the end points
	\param p2 [In] The other end point
    */
YSBOOL YsCheckInBetween3(const YsVec3 &ref,const YsVec3 &p1,const YsVec3 &p2);

/*! This function calculates an intersection between the two lines defined by
    the given passing point, and returns YSOK if the intersection was calculated,
    or YSERR otherwise.

    For lines passing through 3D space, use YsGetNearestPointOfTwoLine instead.

	\sa YsGetNearestPointOfTwoLine
    \param crs [Out]If the calculation is successful, the intersection point is returned to this.
    \param p1 [In] A point that the first line passes through
    \param p2 [In] The other point that the first line passes through
    \param q1 [In] A point that the second line passes through
    \param q2 [In] The other point that the second line passes through */
YSRESULT YsGetLineIntersection2(YsVec2 &crs,const YsVec2 &p1,const YsVec2 &p2,const YsVec2 &q1,const YsVec2 &q2);

/*! This function returns YSTRUE if the given two lines are identical. 
    \param o1 [In] A point that the first line passes through
    \param v1 [In] A vector that is parallel to the first line
    \param o2 [In] A point that the second line passes through
    \param v2 [In] A vector that is parallel to the second line */
YSBOOL YsCheckLineOverlap2(const YsVec2 &o1,const YsVec2 &v1,const YsVec2 &o2,const YsVec2 &v2);

/*! This function returns YSTRUE if the given two lines are identical. 
    \param o1 [In] A point that the first line passes through
    \param v1 [In] A vector that is parallel to the first line
    \param o2 [In] A point that the second line passes through
    \param v2 [In] A vector that is parallel to the second line */
YSBOOL YsCheckLineOverlap3(const YsVec3 &o1,const YsVec3 &v1,const YsVec3 &o2,const YsVec3 &v2);

/*! This function returns if the reference point is inside, outside, or on the boundary of the
    given polygon.
    \param ref [In] Reference point
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon */
YSSIDE YsCheckInsidePolygon2(const YsVec2 &ref,YSSIZE_T np,const YsVec2 p[]);

/*! Templated version of YsCheckInsidePolygon2 */
template <const int N>
YSSIDE YsCheckInsidePolygon2(const YsVec2 &ref,const YsArray <YsVec2,N> &plg)
{
	return YsCheckInsidePolygon2(ref,plg.GetN(),plg);
}

/*! This function returns if the reference point is inside, outside, or on the boundary of the
    given polygon.  The result is the most accurate when all the points of the polygon and the
    reference point is co-planar.  When those points are not lying on the plane, the result
    may becone inaccurate.
    \param ref [In] Reference point
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon */
YSSIDE YsCheckInsidePolygon3(const YsVec3 &ref,YSSIZE_T np,const YsVec3 p[]);

/*! Templated version of YsCheckInsidePolygon3 */
template <const int N>
YSSIDE YsCheckInsidePolygon3(const YsVec3 &ref,const YsArray <YsVec3,N> &plg)
{
	return YsCheckInsidePolygon3(ref,plg.GetN(),plg);
}

/*! This function returns if the reference point is inside, outside, or on the boundary of the
    given polygon.  The result is the most accurate when all the points of the polygon and the
    reference point is co-planar.  When those points are not lying on the plane, the result
    may becone inaccurate.
    \param ref [In] Reference point
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    \param nom [In] Normal of the polygon */
YSSIDE YsCheckInsidePolygon3(const YsVec3 &ref,YSSIZE_T np,const YsVec3 p[],const YsVec3 &nom);

/*! Templated version of YsCheckInsidePolygon3 */
template <const int N>
YSSIDE YsCheckInsidePolygon3(const YsVec3 &ref,const YsArray <YsVec3,N> &plg,const YsVec3 &nom)
{
	return YsCheckInsidePolygon3(ref,plg.GetN(),plg,nom);
}

/*! This function returns if the reference point is inside, outside, or on the boundary of the
    given triangle.
    \param ref [In] Reference point
    \param tri [In] Points of the triangle */
YSSIDE YsCheckInsideTriangle2(const YsVec2 &ref,const YsVec2 tri[]);

/*! This function returns if the reference point is inside, outside, or on the boundary of the
    given triangle.  The result is the most accurate when all the points of the polygon and the
    reference point is co-planar.  When those points are not lying on the plane, the result
    may becone inaccurate.
    \param ref [In] Reference point
    \param tri [In] Points of the triangle */
YSSIDE YsCheckInsideTriangle3(const YsVec3 &ref,const YsVec3 tri[]);

/*! This function calculates and returns the corner angle of the polygon.  This function does not 
    distinguish convex and concave corner.  The returned angle is always between 0.0 and YsPi. 
	The angle returned by this function is an inner angle at the corner.  If the polygon makes a
	sharp turn at the corner, the return value will be close to zero.  Or, the polygon is round
	and smooth at the corner, the return value will be close to YsPi.  To get how much the polygon is
	bending at the corner, subtract the return value from YsPi.
    The calling function must make sure 0<=corner and corner<nPlvt.  Also, the polygon must not have
    a degenerate edge, or the result of this function will be undefined.  */
const double YsGetPolygonCornerAngle(int nPlVt,const YsVec3 plVtPos[],int corner);

/*! Templated version of YsGetPolygonCornerAngle */
template <const int N>
const double YsGetPolygonCornerAngle(const YsArray <YsVec3,N> &plg,int corner)
{
	return YsGetPolygonCornerAngle(plg.GetN(),plg,corner);
}

/*! This function returns the minimum and maximum value that is calculated by 
    YsGetPolygonCornerAngle for the given polygon. */
void YsGetPolygonMinAndMaxCornerAngle(double &minAng,double &maxAng,int nPlVt,const YsVec3 plVtPos[]);

/*! Templated version of YsGetPolygonMinAndMaxCornerAngle */
template <const int N>
void YsGetPolygonMinAndMaxCornerAngle(double &minAng,double &maxAng,const YsArray <YsVec3,N> &plg)
{
	return YsGetPolygonMinAndMaxCornerAngle(minAng,maxAng,plg.GetN(),plg);
}

/// \cond
YSBOOL YsIsLineSegIntersectionReliable2(const YsVec2 &p1,const YsVec2 &p2,const YsVec2 &q1,const YsVec2 &q2);
YSBOOL YsPointEdgeDistanceIsWithinRangeOf(const YsVec3 &pnt,int np,const YsVec3 p[],const double &ratio);
/// \endcond


/*! This function returns the center of the largest triangle that consists of three points
    of the given polygon.

    Note that this does not return the gravity center of the polygon, which is often biased
    by the non-uniform point distribution.

    \param cen [Out] Center of the largest triangle that consists of three points of the given polygon.
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    */
YSRESULT YsGetCenterOfPolygon3(YsVec3 &cen,YSSIZE_T np,const YsVec3 p[]);

/*! This function returns the center of the largest triangle that consists of three points
    of the given polygon.

    Note that this does not return the gravity center of the polygon, which is often biased
    by the non-uniform point distribution.

    \param cen [Out] Center of the largest triangle that consists of three points of the given polygon.
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    */
YSRESULT YsGetCenterOfPolygon2(YsVec2 &cen,YSSIZE_T np,const YsVec2 p[]);

/*! This function calculates the average of the points p[0]...p[np-1] */
YsVec3 YsGetCenter(YSSIZE_T np,const YsVec3 p[]);

/*! This function calculates the average of the points p[0]...p[np-1] */
YsVec2 YsGetCenter(YSSIZE_T np,const YsVec2 p[]);

/*! Templated version of YsGetCenter */
template <const int N>
YsVec3 YsGetCenter(const YsArray <YsVec3,N> &p)
{
	return YsGetCenter(p.GetN(),p);
}

/*! Templated version of YsGetCenter */
template <const int N>
YsVec2 YsGetCenter(const YsArray <YsVec2,N> &p)
{
	return YsGetCenter(p.GetN(),p);
}

/*! This function calculates a 4x4 affine-trnasformation matrix that transforms the given polygon 
    on the XY plane.  By the matrix returned by this function, a point of the polygon p[i] {0<=i<np} 
    is transformed to a point on XY plane p'[i] as:
        p'[i]=mat*p[i]

    \param mat [Out] A 4x4 matrix that receives the calculation result
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    */
YSRESULT YsGetPolygonProjectionMatrix(YsMatrix4x4 &mat,YSSIZE_T np,const YsVec3 p[]);
	// Returns a matrix that project a Polygon into XY plane.
	// Note:Twist direction will be reversed in Left-Handed coordinate.

/*! Templated version of YsGetPolygonProjectionMatrix */
template <const int N>
YSRESULT YsGetPolygonProjectionMatrix(YsMatrix4x4 &mat,const YsArray <YsVec3,N> &plg)
{
	return YsGetPolygonProjectionMatrix(mat,plg.GetN(),plg);
}

/*! This function checks if how two pieces of line are located.
    \sa YSINTERSECTION
    \param p1 [In] An end point of the first line
    \param p2 [In] The other end point of the first line
    \param q1 [In] An end point of the second line
    \param q2 [In] The other end point of the second line
    */
YSINTERSECTION YsGetLinePenetration2(const YsVec2 &p1,const YsVec2 &p2,const YsVec2 &q1,const YsVec2 &q2);

/*! This function checks if how two pieces of line are located.
    \sa YSINTERSECTION
    \param p1 [In] An end point of the first line
    \param p2 [In] The other end point of the first line
    \param q1 [In] An end point of the second line
    \param q2 [In] The other end point of the second line
    */
YSINTERSECTION YsGetLinePenetration3(const YsVec3 &p1,const YsVec3 &p2,const YsVec3 &q1,const YsVec3 &q2);


/*! This function checks if a polygon that consists of p[0], p[1], ..., p[np-1] can
    be split by connecting p[idx1] and p[idx2].
    */
YSBOOL YsCheckSeparatability2(YSSIZE_T np,const YsVec2 p[],int idx1,int idx2);

/*! Templated version of YsCheckSeparatability2 */
template <const int N>
YSBOOL YsCheckSeparatability2(const YsArray <YsVec2,N> &plg,int idx1,int idx2)
{
	return YsCheckSeparatability2(plg.GetN(),plg,idx1,idx2);
}

/*! This function checks if a polygon that consists of p[0], p[1], ..., p[np-1] can
    be split by connecting p[idx1] and p[idx2].
    */
YSBOOL YsCheckSeparatability3(YSSIZE_T np,const YsVec3 p[],int idx1,int idx2);
	// Note:YsCheckSeparatability3 is extremely slow function.
	//      Not recommended to used it in a loop.
	//      It is recommended to make PolygonProjectionMatrix
	//      and project the polygon into XY plane. Then, use it to
	//      Check separatability.

/*! Templated version of YsCheckSeparatability2 */
template <const int N>
YSBOOL YsCheckSeparatability3(const YsArray <YsVec3,N> &plg,int idx1,int idx2)
{
	return YsCheckSeparatability3(plg.GetN(),plg,idx1,idx2);
}

/*! This function returns YSTRUE if two bounding boxes intersects or touches each other or YSFALSE otherwise.
    \param min1 [In] Minimum x and y of bounding box 1
    \param max1 [In] Maximum x and y of bounding box 1
    \param min2 [In] Minimum x and y of bounding box 2
    \param max2 [In] Maximum x and y of bounding box 2
    */
YSBOOL YsCheckBoundingBoxCollision3(const YsVec3 &min1,const YsVec3 &max1,const YsVec3 &min2,const YsVec3 &max2);

/*! This function returns YSTRUE if two bounding boxes intersects or touches each other or YSFALSE otherwise.
    \param min1 [In] Minimum x, y, and z of bounding box 1
    \param max1 [In] Maximum x, y, and z of bounding box 1
    \param min2 [In] Minimum x, y, and z of bounding box 2
    \param max2 [In] Maximum x, y, and z of bounding box 2
    */
YSBOOL YsCheckBoundingBoxCollision2(const YsVec2 &min1,const YsVec2 &max1,const YsVec2 &min2,const YsVec2 &max2);

/*! This function returns YSTRUE if the reference point is inside or on the boundary of the given bounding box
    or YSFALSE otherwise.
    \param ref [In] Reference point
    \param min [In] Minimum x, y, and z of the bounding box
    \param max [In] Maximum x, y, and z of the bounding box
    */
YSBOOL YsCheckInsideBoundingBox3(const YsVec3 &ref,const YsVec3 &min,const YsVec3 &max);

/*! This function returns YSTRUE if the reference point is inside or on the boundary of the given bounding box
    or YSFALSE otherwise.
    \param ref [In] Reference point
    \param min [In] Minimum x and y of the bounding box
    \param max [In] Maximum x and y of the bounding box
    */
YSBOOL YsCheckInsideBoundingBox2(const YsVec2 &ref,const YsVec2 &min,const YsVec2 &max);

/*! This function returns if the given polygon is oriented clockwise or counter clockwise. */
YSFLIPDIRECTION YsCheckFlipDirection2(YSSIZE_T np,const YsVec2 p[]);

/*! This function returns if the given polygon is oriented clockwise or counter clockwise. */
template <const int N>
YSFLIPDIRECTION YsCheckFlipDirection2(const YsArray <YsVec2,N> &plg)
{
	return YsCheckFlipDirection2(plg.GetN(),plg);
}

/*! This function returns if the given polygon is oriented clockwise or counter clockwise
    with respect to the given normal. 
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    \param n [In]  Normal vector */
YSFLIPDIRECTION YsCheckFlipDirection3(YSSIZE_T np,const YsVec3 p[],const YsVec3 &n);

/*! This function returns if the given polygon is oriented clockwise or counter clockwise
    with respect to the given normal. 
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    \param n [In]  Normal vector */
template <const int N>
YSFLIPDIRECTION YsCheckFlipDirection3(const YsArray <YsVec3,N> &plg,const YsVec3 &n)
{
	return YsCheckFlipDirection3(plg.GetN(),plg,n);
}

/*! This function calculates a line formed by intersecting two planes.  This function returns YSOK
    if the calculation is sucessful or YSERR if the line could not be calculated.
    \param o [Out] A point that the line passes through
    \param v [Out] A vector parallel so the line
    \param pln1 [In] A plane
    \param pln2 [In] A plane */
YSRESULT YsGetTwoPlaneCrossLine(YsVec3 &o,YsVec3 &v,const YsPlane &pln1,const YsPlane &pln2);

/*! This function calculates nearest points of two lines.  Even when two lines defined in the
    3D space are linearly independent, two lines may not intersect.  Therefore, this function
    calculates a point on each line that is closest to the other line.
    
    If the calculation is successful, it returns YSOK and the points are returned in np and nq.

    \param np [Out] A point on line 1 closest to line 2
    \param nq [Out] A point on line 2 closest to line 1
    \param p1 [In] A point that line 1 passes through
    \param p2 [In] Another point that line 1 passes through
    \param q1 [In] A point that line 2 passes through
    \param q2 [In] Another point that line 2 passes through
    */
YSRESULT YsGetNearestPointOfTwoLine(YsVec3 &np,YsVec3 &nq,const YsVec3 &p1,const YsVec3 &p2,const YsVec3 &q1,const YsVec3 &q2);

/*! This function calculates the nearest point on the given line from the given reference point and
    returns it in np.  The line is defined by two points that the line passes through.
    \param np [Out] Nearest point
    \param p1 [In] A point that the line passes through
    \param p2 [In] A point that the line passes through
    \param ref [In] Reference point */
YSRESULT YsGetNearestPointOnLine3(YsVec3 &np,const YsVec3 &p1,const YsVec3 &p2,const YsVec3 &ref);

/*! This function calculates the nearest point on the given line from the given reference point and
    returns it in np.  The line is defined by two points that the line passes through.
    \param np [Out] Nearest point
    \param p1 [In] A point that the line passes through
    \param p2 [In] A point that the line passes through
    \param ref [In] Reference point */
YSRESULT YsGetNearestPointOnLine2(YsVec2 &np,const YsVec2 &p1,const YsVec2 &p2,const YsVec2 &ref);

/*! This function returns distance from a reference point to a line that passes through given points.
    \param p1 [In] A point that the line passes through
    \param p2 [In] A point that the line passes through
    \param ref [In] Reference point */
const double YsGetPointLineDistance2(const YsVec2 &p1,const YsVec2 &p2,const YsVec2 &ref);

/*! This function returns distance from a reference point to a line that passes through given points.
    \param p1 [In] A point that the line passes through
    \param p2 [In] A point that the line passes through
    \param ref [In] Reference point */
const double YsGetPointLineDistance3(const YsVec3 &p1,const YsVec3 &p2,const YsVec3 &ref);

/*! This function returns distance between two pieces of line.
    \param p1 [In] A end point of line 1
    \param p2 [In] The other end point of line 1
    \param q1 [In] A end point of line 2
    \param q2 [In] The other end point of line 2 */
double YsGetDistanceBetweenTwoLineSegment(const YsVec3 &p1,const YsVec3 &p2,const YsVec3 &q1,const YsVec3 &q2);

/*! This function returns distance from a reference point to a piece of line
    \param p1 [In] A end point of the line
    \param p2 [In] The other end point of the line
    \param ref [In] Reference point */
const double YsGetPointLineSegDistance2(const YsVec2 &p1,const YsVec2 &p2,const YsVec2 &ref); 

/*! This function returns distance from a reference point to a piece of line
    \param p1 [In] A end point of the line
    \param p2 [In] The other end point of the line
    \param ref [In] Reference point */
const double YsGetPointLineSegDistance3(const YsVec3 &p1,const YsVec3 &p2,const YsVec3 &ref); 

/*! This function returns YSTRUE if the projection of the reference point on the given line
    is between two end points of the piece of the line.
    \param ref [In] Reference point
    \param p1 [In] One end point of the piece of line
    \param p2 [In] The other end point of the piece of line */
YSBOOL YsCheckPointIsOnLineSegment3(const YsVec3 &ref,const YsVec3 &p1,const YsVec3 &p2);

/*! This function returns how two polygons are intersecting each other.
    \sa YSINTERSECTION
    \param np [In] Number of points of polygon 1
    \param p [In] Points of polygon 1
    \param nq [In] Number of points of polygon 2
    \param q [In] Points of polygon 2
    */
YSINTERSECTION YsGetPolygonPenetration(YSSIZE_T np,const YsVec3 p[],YSSIZE_T nq,const YsVec3 q[]);

/*! This function returns a point inside the given polygon.
    \param ret [Out] A point inside the given polygon 
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    \param strictCheckOfConvexity [In] This parameter will be passed internally to YsCheckConvex2
    */
YSRESULT YsGetArbitraryInsidePointOfPolygon2(YsVec2 &ret,YSSIZE_T np,const YsVec2 p[],YSBOOL strictCheckOfConvexity=YSFALSE);

/*! Templated version of YsGetArbitraryInsidePointOfPolygon2 */
template <const int N>
YSRESULT YsGetArbitraryInsidePointOfPolygon2(YsVec2 &ret,const YsArray <YsVec2,N> &plg,YSBOOL strictCheckOfConvexity=YSFALSE)
{
	return YsGetArbitraryInsidePointOfPolygon2(ret,plg.GetN(),plg,strictCheckOfConvexity);
}

/*! This function returns a point inside the given polygon.
    \param ret [Out] A point inside the given polygon 
    \param np [In] Number of points of the polygon
    \param p [In] Points of the polygon
    \param strictCheckOfConvexity [In] This parameter will be passed internally to YsCheckConvex3
    */
YSRESULT YsGetArbitraryInsidePointOfPolygon3(YsVec3 &ret,YSSIZE_T np,const YsVec3 p[],YSBOOL strictCheckOfConvexity=YSFALSE);

/*! Templated version of YsGetArbitraryInsidePointOfPolygon3 */
template <const int N>
YSRESULT YsGetArbitraryInsidePointOfPolygon3(YsVec3 &ret,const YsArray <YsVec3,N> &plg,YSBOOL strictCheckOfConvexity=YSFALSE)
{
	return YsGetArbitraryInsidePointOfPolygon3(ret,plg.GetN(),plg,strictCheckOfConvexity);
}

/*! This function sorts points p[0], p[1], ..., p[np-1]. 
    The function first calculates the diameter of the given points (distance between two points 
    that are the farthest apart) and then sorts points based on the distance from one of the two points.
    If each point is associated with an index, this function can sort indices together.
    \sa YsSortPointSet3Template
    \param np [In] Number of points
    \param p [In/Out] Points
    \param idx [In/Out] Indices associated with the points.  Can be NULL.
    */
YSRESULT YsSortPointSet3(YSSIZE_T np,YsVec3 p[],int idx[]);

/*! This function sorts points p[0], p[1], ..., p[np-1] based on the distance from a reference point. 
    If each point is associated with an index, this function can sort indices together.
    \sa YsSortPointSet3Template
    \param np [In] Number of points
    \param p [In/Out] Points
    \param idx [In/Out] Indices associated with the points.  Can be NULL.
    \param ref [In] Reference point
    */
YSRESULT YsSortPointSet3(YSSIZE_T np,YsVec3 p[],int idx[],const YsVec3 &ref);

/*! This function sorts points p[0], p[1], ..., p[np-1] based on the distance from a reference point. 
    If each point is associated with an index, this function can sort indices together.
    \sa YsSortPointSet3Template
    \param np [In] Number of points
    \param p [In/Out] Points
    \param idx [In/Out] Indices associated with the points.  Can be NULL.
    \param ref [In] Reference point
    \param sqKnownMaxDistance [In] Square of diameter of the set of points (Distance between two points that are the farthest apart)
    */
YSRESULT YsSortPointSet3(YsVec3 p[],int idx[],YSSIZE_T np,const YsVec3 ref,double sqKnownMaxDistance);

/*! This function calculates a circumscribed circle of the triangle.
    \param cen [Out] Center of the circumscribed circle
    \param rad [Out] Radius of the circumscribed circle
    \param p1 [In] A point of the triangle
    \param p2 [In] A point of the triangle
    \param p3 [In] A point of the triangle
    */
YSRESULT YsGetCircumCircle(YsVec2 &cen,double &rad,const YsVec2 &p1,const YsVec2 &p2,const YsVec2 &p3);

/*! This function calculates a inscribed circle of the triangle.
    \param cen [Out] Center of the inscribed circle
    \param rad [Out] Radius of the inscribed circle
    \param p1 [In] A point of the triangle
    \param p2 [In] A point of the triangle
    \param p3 [In] A point of the triangle
    */
YSRESULT YsGetInscribedCircle(YsVec2 &cen,double &rad,const YsVec2 &p1,const YsVec2 &p2,const YsVec2 &p3);

/*! This function calculates the radius of the circumscribed sphere of a tetrahedron.  If the calculation
    is successful, it returns YSOK, or YSERR otherwise.
    \param rad [Out] Radius of the circumscribed sphere
    \param p [In] Array of constant pointers to the points of the tetrahedron
    */
YSRESULT YsGetCircumSphereRadius(double &rad,YsVec3 const *p[4]);

/*! This function calculates the center and radius of the circumscribed sphere of a tetrahedron.  
    If the calculation is successful, it returns YSOK, or YSERR otherwise.
    \param cen [Out] Center of the circumscribed sphere
    \param rad [Out] Radius of the circumscribed sphere
    \param p [In] Array of constant pointers to the points of the tetrahedron
    */
YSRESULT YsGetCircumSphere(YsVec3 &cen,double &rad,YsVec3 const *p[4]);

/*! This function calculates the radius of the circumscribed sphere of a tetrahedron.  
    If the calculation is successful, it returns YSOK, or YSERR otherwise.
    \param rad [Out] Radius of the circumscribed sphere
    \param p [In] Points of the tetrahedron
    */
YSRESULT YsGetCircumSphereRadius(double &rad,const YsVec3 p[4]);

/*! This function calculates the center and radius of the circumscribed sphere of a tetrahedron.  
    If the calculation is successful, it returns YSOK, or YSERR otherwise.
    \param cen [Out] Center of the circumscribed sphere
    \param rad [Out] Radius of the circumscribed sphere
    \param p [In] Points of the tetrahedron
    */
YSRESULT YsGetCircumSphere(YsVec3 &cen,double &rad,const YsVec3 p[4]);

/*! This function calculates normal of the given triangle. 
    \param nom [Out] Unit normal vector of the triangle
    \param t1 [In] A point of the triangle
    \param t2 [In] A point of the triangle
    \param t3 [In] A point of the triangle
    */
YSRESULT YsComputeNormalOfTriangle(YsVec3 &nom,const YsVec3 &t1,const YsVec3 &t2,const YsVec3 &t3);

/*! This function calculates normal vectors of the four faces of a tetrahedron. 
    \param nom [Out] Unit normal vectors of the faces of the tetrahedron
    \param p [In] Constant pointers to the points of the tetrahedron.
    */
YSRESULT YsGetNormalOfTet(YsVec3 nom[4],YsVec3 const *p[4]);

/*! This function calculates the radius of the inscribed sphere of a tetrahedron.  
    If the calculation is successful, it returns YSOK, or YSERR otherwise.
    \param rad [Out] Radius of the incribed sphere
    \param p [In] Array of constant pointers to the points of the tetrahedron
    */
YSRESULT YsGetInscribedSphereRadius(double &rad,YsVec3 const *p[4]);

/*! This function calculates the center and radius of the incribed sphere of a tetrahedron.  
    If the calculation is successful, it returns YSOK, or YSERR otherwise.
    \param cen [Out] Center of the inscribed sphere
    \param rad [Out] Radius of the inscribed sphere
    \param p [In] Array of constant pointers to the points of the tetrahedron
    */
YSRESULT YsGetInscribedSphere(YsVec3 &cen,double &rad,YsVec3 const *p[4]);

/*! This function calculates the radius of the inscribed sphere of a tetrahedron.  
    If the calculation is successful, it returns YSOK, or YSERR otherwise.
    \param rad [Out] Radius of the incribed sphere
    \param p [In] Array of pointsof the tetrahedron
    */
YSRESULT YsGetInscribedSphereRadius(double &rad,const YsVec3 p[4]);

/*! This function calculates the radius of the inscribed sphere of a tetrahedron.  
    If the calculation is successful, it returns YSOK, or YSERR otherwise.
    \param cen [Out] Center of the inscribed sphere
    \param rad [Out] Radius of the incribed sphere
    \param p [In] Array of pointsof the tetrahedron
    */
YSRESULT YsGetInscribedSphere(YsVec3 &cen,double &rad,const YsVec3 p[4]);

/*! This function calculates a sphere that circumscribes the given triangle and whose center is
    lying on the plane of the triangle.
    If the calculation is successful, it returns YSOK, or YSERR otherwise.
    \param cen [Out] Center of the circumscribed sphere
    \param rad [Out] Radius of the circumscribed sphere
    \param p1 [In] A point of the triangle
    \param p2 [In] A point of the triangle
    \param p3 [In] A point of the triangle
    */
YSRESULT YsGetCircumSphereOfTriangle(YsVec3 &cen,double &rad,const YsVec3 &p1,const YsVec3 &p2,const YsVec3 &p3);

/*! This function calculates a sphere that inscribes the given triangle and whose center is
    lying on the plane of the triangle.
    If the calculation is successful, it returns YSOK, or YSERR otherwise.
    \param cen [Out] Center of the inscribed sphere
    \param rad [Out] Radius of the inscribed sphere
    \param p1 [In] A point of the triangle
    \param p2 [In] A point of the triangle
    \param p3 [In] A point of the triangle
    */
YSRESULT YsGetInscribedSphereOfTriangle(YsVec3 &cen,double &rad,const YsVec3 &p1,const YsVec3 &p2,const YsVec3 &p3);

/*! This function scales the input bounding box by given scaling ratio.
    The center of the box stays unchanged.
    Although this function is called 'Inflate', you can shrink the bounding box by giving
    scaling ratio of less than 1.0.
    \param newMin [Out] Minimum x,y, and z of the scaled bounding box
    \param newMax [Out] Maximum x,y, and z of the scaled bounding box
    \param min [In] Minimum x,y, and z of the input bounding box
    \param max [In] Maximum x,y, and z of the input bounding box
    \param ratio [In] Scaling ratio
    */
YSRESULT YsInflateBoundingBox(YsVec3 &newMin,YsVec3 &newMax,const YsVec3 &min,const YsVec3 &max,const double ratio);

/*! This function calculates a circle that is made by intersection of two spheres.
    If the calculation is successful, this function returns YSOK, or YSERR otherwise.
    \param o [Out] Center of the circle
    \param n [Out] Normal of the circle
    \param r [Out] Radius of the circle
    \param c1 [In] Center of the first sphere
    \param r1 [In] Radius of the first sphere
    \param c2 [In] Center of the second sphere
    \param r2 [In] Radius of the second sphere
    */
YSRESULT YsComputeTwoSphereIntersection
(YsVec3 &o,YsVec3 &n,double &r,const YsVec3 &c1,const double &r1,const YsVec3 &c2,const double &r2);

/*! This function calculates two intersection points of three spheres.
    If the calculation is successful, this function returns YSOK, or YSERR otherwise.
    \param i1 [Out] An intersection
    \param i2 [Out] An intersection
    \param c1 [In] Center of the first sphere
    \param r1 [In] Radius of the first sphere
    \param c2 [In] Center of the second sphere
    \param r2 [In] Radius of the second sphere
    \param c3 [In] Center of the third sphere
    \param r3 [In] Radius of the third sphere
    */
YSRESULT YsComputeThreeSphereIntersection
(YsVec3 &i1,YsVec3 &i2,
 const YsVec3 &c1,const double &r1,const YsVec3 &c2,const double &r2,const YsVec3 &c3,const double &r3);


/*! This function calculates a point on a sequence of line segments that is nearest to the reference point.
    This function returns YSOK if the calculation is successful, or YSERR otherwise.
    \param nearPos [Out] A point on the line segments that is nearest to the reference point
    \param nearIdx [Out] Index to the segment on which nearPos is located.
    \param nearNode [Out] If it is YSTRUE, the nearest point is p[nearIdx].  Or, the nearest point is between p[nearIdx] and p[nearIdx+1]
    \param np [In] Number of points of the line segments
    \param p [In] Pints of the line segments
    \param ref [In] Reference point
    */
YSRESULT YsGetNearestPointOnLineSegments3(YsVec3 &nearPos,int &nearIdx,YSBOOL &nearNode,YSSIZE_T np,const YsVec3 p[],const YsVec3 &ref);

/*! This function calculates a point on a sequence of line segments that is nearest to the reference point.
    This function returns YSOK if the calculation is successful, or YSERR otherwise.
    \param nearPos [Out] A point on the line segments that is nearest to the reference point
    \param nearIdx [Out] Index to the segment on which nearPos is located.
    \param nearNode [Out] If it is YSTRUE, the nearest point is p[nearIdx].  Or, the nearest point is between p[nearIdx] and p[nearIdx+1]
    \param np [In] Number of points of the line segments
    \param p [In] Pints of the line segments
    \param ref [In] Reference point
    */
YSRESULT YsGetNearestPointOnLineSegments2(YsVec2 &nearPos,int &nearIdx,YSBOOL &nearNode,int np,const YsVec2 p[],const YsVec2 &ref);

/*! This function calculates tangential vector of line segments at a node.
    \param unitTan [Out] Unit tangential vector
    \param ndId [In] Index to the node where the tangential vector is calculated (0<=ndId<np)
    \param np [In] Number of nodes of the line segments
    \param p [In] Points of the line segments
    */
YSRESULT YsGetTangentialVectorAtNodeOfLineSegments3(YsVec3 &unitTan,YSSIZE_T ndId,YSSIZE_T np,const YsVec3 p[]);

/*! This function calculates tangential vector of line segments of a segment (p[ndId+1]-p[ndId]).
    \param unitTan [Out] Unit tangential vector
    \param edId [In] Index to the segment where the tangential vector is calculated (0<=ndId<np-1)
    \param np [In] Number of nodes of the line segments
    \param p [In] Points of the line segments
    */
YSRESULT YsGetTangentialVectorOnEdgeOfLineSegments3(YsVec3 &unitTan,int edId,int np,const YsVec3 p[]);


/// \cond
template <class T>
int YsFindNearestPoint(const T &refp,int np,const T p[])
{
	if(np>0)
	{
		int i,id;
		double d,dMin;
		id=0;
		dMin=(p[0]-refp).GetSquareLength();
		for(i=1; i<np; i++)
		{
			d=(p[i]-refp).GetSquareLength();
			if(d<dMin)
			{
				id=i;
				dMin=d;
			}
		}
		return id;
	}
	return -1;
}
/// \endcond

/*! This function finds the point nearest to the reference point among the given set of points
    and returns the index to the nearest point.
    \param ref [In] Refernce point
    \param np [In] Number of points
    \param p [In] Set of points
    */
inline int YsFindNearestPoint3(const YsVec3 &ref,int np,const YsVec3 p[])
{
	return YsFindNearestPoint <YsVec3> (ref,np,p);
}

/*! This function finds the point nearest to the reference point among the given set of points
    and returns the index to the nearest point.
    \param ref [In] Refernce point
    \param np [In] Number of points
    \param p [In] Set of points
    */
inline int YsFindNearestPoint2(const YsVec2 &ref,int np,const YsVec2 p[])
{
	return YsFindNearestPoint <YsVec2> (ref,np,p);
}


/// \cond
template <class T>
int YsFindFarthestPoint(const T &refp,int np,const T p[])
{
	if(np>0)
	{
		int i,id;
		double d,dMax;
		id=0;
		dMax=(p[0]-refp).GetSquareLength();
		for(i=1; i<np; i++)
		{
			d=(p[i]-refp).GetSquareLength();
			if(d>dMax)
			{
				id=i;
				dMax=d;
			}
		}
		return id;
	}
	return -1;
}
/// \endcond


/*! This function finds the point farthest to the reference point among the given set of points
    and returns the index to the nearest point.
    \param ref [In] Refernce point
    \param np [In] Number of points
    \param p [In] Set of points
    */
inline int YsFindFarthestPoint3(const YsVec3 &ref,int np,const YsVec3 p[])
{
	return YsFindFarthestPoint <YsVec3> (ref,np,p);
}

/*! This function finds the point farthest to the reference point among the given set of points
    and returns the index to the nearest point.
    \param ref [In] Refernce point
    \param np [In] Number of points
    \param p [In] Set of points
    */
inline int YsFindFarthestPoint2(const YsVec2 &ref,int np,const YsVec2 p[])
{
	return YsFindFarthestPoint <YsVec2> (ref,np,p);
}


template <class T>
inline YSBOOL YsCheckSamePolygon(int nPlVt,const T plVt1[],const T plVt2[],YSBOOL takeReverse=YSTRUE)
{
	int i;
	int j0=-1;
	for(i=0; i<nPlVt; i++)
	{
		if(plVt1[0]==plVt2[i])
		{
			j0=i;
			break;
		}
	}

	if(0<=j0)
	{
		YSBOOL different=YSFALSE;
		for(i=1; i<nPlVt; i++)
		{
			if(plVt1[i]!=plVt2[(j0+i)%nPlVt])
			{
				different=YSTRUE;
				break;
			}
		}
		if(YSTRUE!=different)
		{
			return YSTRUE;
		}

		if(YSTRUE==takeReverse)
		{
			for(i=1; i<nPlVt; i++)
			{
				if(plVt1[i]!=plVt2[(j0+nPlVt-i)%nPlVt])
				{
					different=YSTRUE;
					break;
				}
			}
			if(YSTRUE!=different)
			{
				return YSTRUE;
			}
		}
	}

	return YSFALSE;
}

template <class T,const int N>
inline YSBOOL YsCheckSamePolygon(const YsArray <T,N> &plg1,const YsArray <T,N> &plg2,YSBOOL takeReverse=YSTRUE)
{
	return YsCheckSamePolygon <T> (plg1.GetN(),plg1,plg2.GetN(),plg2(),takeReverse);
}

////////////////////////////////////////////////////////////

/// \cond
// Internal use only.  Do not use this template function directly.
template <class T>
void YsQuickSortPointSet3Template(YsVec3 tab[],T idx[],YSSIZE_T nTab,const YsVec3 &refp,double mind,double maxd)
{
	if(nTab>1)
	{
		double threshold;

		threshold=(mind+maxd)/2.0;

		// smaller comes first
		YSBOOL allTheSame;
		double d0;
		YSSIZE_T i,nSmall;

		nSmall=0;
		allTheSame=YSTRUE;
		d0=(tab[0]-refp).GetSquareLength();

		for(i=0; i<nTab; i++) // Actually can begin with 1
		{
			double var;
			var=(tab[i]-refp).GetSquareLength();
			if(allTheSame==YSTRUE && YsAbs(sqrt(var)-sqrt(d0))>YsTolerance)  // Modified 2000/11/10
			{
				allTheSame=YSFALSE;
			}
			if(var<=threshold)
			{
				YsVec3 a;
				a=tab[i];
				tab[i]=tab[nSmall];
				tab[nSmall]=a;

				if(idx!=NULL)
				{
					T x;
					x=idx[i];
					idx[i]=idx[nSmall];
					idx[nSmall]=x;
				}

				nSmall++;
			}
		}

		if(allTheSame!=YSTRUE)
		{
			if(idx!=NULL)
			{
				YsQuickSortPointSet3Template <T> (tab       ,idx       ,nSmall       ,refp,mind,threshold);
				YsQuickSortPointSet3Template <T> (tab+nSmall,idx+nSmall,(nTab-nSmall),refp,threshold,maxd);
			}
			else
			{
				YsQuickSortPointSet3Template <T> (tab       ,NULL,nSmall       ,refp,mind,threshold);
				YsQuickSortPointSet3Template <T> (tab+nSmall,NULL,(nTab-nSmall),refp,threshold,maxd);
			}
		}
	}
}
/// \endcond

/*! This function sorts set of points based on the distance from the reference point.

    \tparam T A class that is associated with the points.  If nothing is associated with the points, make it int and give NULL to idx
    \param p [In/Out] Points to be sorted
    \param idx [In/Out] Values associated with the points.  If nothing is associated with the points, it can be NULL.
    \param np [In] Number of points
    \param ref [In] Reference point
    \param sqKnownMaxDistance [In] Maximum distance from the reference point.
    */
template <class T>
YSRESULT YsSortPointSet3Template(YsVec3 p[],T idx[],YSSIZE_T np,const YsVec3 ref,double sqKnownMaxDistance)
{                                                               //  ^^^^ ref must not be a pointer/reference
	YsQuickSortPointSet3Template(p,idx,np,ref,0.0,sqKnownMaxDistance);
	return YSOK;
}

/*! This function sorts set of points based on the distance from the reference point.
    The reference point is taken from one of the two points among the given points that
    makes the maximum distance.

    \tparam T A class that is associated with the points.  If nothing is associated with the points, make it int and give NULL to idx
    \param np [In] Number of points
    \param p [In/Out] Points to be sorted
    \param idx [In/Out] Values associated with the points.  If nothing is associated with the points, it can be NULL.
    */
template <class T>
YSRESULT YsSortPointSet3Template(YSSIZE_T np,YsVec3 p[],T idx[])
{
	double sqDist;
	YSSIZE_T vt1,vt2;
	vt1=0;
	vt2=0;
	sqDist=0.0;
	for(YSSIZE_T i=0; i<np; i++)
	{
		for(YSSIZE_T j=i+1; j<np; j++)
		{
			if((p[i]-p[j]).GetSquareLength()>sqDist)
			{
				vt1=i;
				vt2=j;
				sqDist=(p[i]-p[j]).GetSquareLength();
			}
		}
	}

	return YsSortPointSet3(p,idx,np,p[vt1],sqDist);

}

/*! This function sorts set of points based on the distance from the reference point.

    \tparam T A class that is associated with the points.  If nothing is associated with the points, make it int and give NULL to idx
    \param p [In/Out] Points to be sorted
    \param idx [In/Out] Values associated with the points.  If nothing is associated with the points, it can be NULL.
    \param np [In] Number of points
    \param ref [In] Reference point
    */
template <class T>
YSRESULT YsSortPointSet3Template(YSSIZE_T  np,YsVec3 p[],T idx[],const YsVec3 &ref)
{
	double sqDist;
	sqDist=0.0;
	for(YSSIZE_T i=0; i<np; i++)
	{
		if((ref-p[i]).GetSquareLength()>sqDist)
		{
			sqDist=(ref-p[i]).GetSquareLength();
		}
	}
	return YsSortPointSet3Template(p,idx,np,ref,sqDist);
}

////////////////////////////////////////////////////////////

/*! This function finds an equation of the least-square fitting plane of the given set of points.

    The plane equation will be: ax+by+cz+d=0

    \param a [Out] A coefficient for x
    \param b [Out] A coefficient for y
    \param c [Out] A coefficient for z
    \param d [Out] Constant term
    \param np [In] Number of points
    \param p [In] Array of points
    */
YSRESULT YsFindLeastSquareFittingPlane(double &a,double &b,double &c,double &d,YSSIZE_T np,const YsVec3 p[]);

/*! This function finds an equation of the least-square fitting plane of the given set of points.

    The plane equation will be: ax+by+cz+d=0

    \param a [Out] A coefficient for x
    \param b [Out] A coefficient for y
    \param c [Out] A coefficient for z
    \param d [Out] Constant term
    \param p [In] Array of points
    */
template <const int N>
YSRESULT YsFindLeastSquareFittingPlane(double &a,double &b,double &c,double &d,const YsArray <YsVec3,N> &vtxList)
{
	return YsFindLeastSquareFittingPlane(a,b,c,d,vtxList.GetN(),vtxList);
}


/*! This function calculates a unit normal vector of the least-square fitting plane to the given set of points.

    \param nom [Out] Unit normal vector
    \param np [In] Number of points
    \param p [In] Array of points
    */
YSRESULT YsFindLeastSquareFittingPlaneNormal(YsVec3 &nom,YSSIZE_T np,const YsVec3 p[]);

/*! This function calculates a unit normal vector of the least-square fitting plane to the given set of points.

    \param nom [Out] Unit normal vector
    \param p [In] Array of points
    */
template <const int N>
YSRESULT YsFindLeastSquareFittingPlaneNormal(YsVec3 &nom,const YsArray <YsVec3,N> &vtxList)
{
	return YsFindLeastSquareFittingPlaneNormal(nom,vtxList.GetN(),vtxList);
}


/*! This function calculates a least-square fitting plane to the given set of points and returns
    a point that the plane passes through and a unit normal vector.

    \param org [Out] A point that the plane passes through
    \param nom [Out] Unit normal vector
    \param np [In] Number of points
    \param p [In] Array of points
    */
YSRESULT YsFindLeastSquareFittingPlane(YsVec3 &org,YsVec3 &nom,YSSIZE_T np,const YsVec3 p[]);

/*! This function calculates a least-square fitting plane to the given set of points and returns
    a point that the plane passes through and a unit normal vector.

    \param org [Out] A point that the plane passes through
    \param nom [Out] Unit normal vector
    \param p [In] Array of points
    */
template <const int N>
YSRESULT YsFindLeastSquareFittingPlane(YsVec3 &org,YsVec3 &nom,const YsArray <YsVec3,N> &vtxList)
{
	return YsFindLeastSquareFittingPlane(org,nom,vtxList.GetN(),vtxList);
}

/*! This function calculates a point that minimizes the sum of square distances from given set of lines.
    \param pos [Out] The point
    \param nLine [In] Number of lines
    \param o [In] Points that lines passes through
    \param v [In] Vectors parallel to the lines
    */
YSRESULT YsFindLeastSquarePointFromLine(YsVec3 &pos,const YSSIZE_T nLine,const YsVec3 o[],const YsVec3 v[]);

/*! This function returns the total length of the given line segments
    \param np [In] Number of points
    \param p [In] Points of the line segments
    \param isLoop [In] If it is YSTRUE, the given line segment is a closed loop.
    */
double YsCalculateTotalLength2(int np,const YsVec2 p[],YSBOOL isLoop);

/*! Templated version of YsCalculateTotalLength2 */
template <const int N>
double YsCalculateTotalLength2(const YsArray <YsVec2,N> &p,YSBOOL isLoop)
{
	return YsCalculateTotalLength2(p.GetN(),p,isLoop);
}

/*! This function returns the total length of the given line segments
    \param np [In] Number of points
    \param p [In] Points of the line segments
    \param isLoop [In] If it is YSTRUE, the given line segment is a closed loop.
    */
double YsCalculateTotalLength3(int np,const YsVec3 p[],YSBOOL isLoop);

/*! Templated version of YsCalculateTotalLength3 */
template <const int N>
double YsCalculateTotalLength3(const YsArray <YsVec3,N> &p,YSBOOL isLoop)
{
	return YsCalculateTotalLength3(p.GetN(),p,isLoop);
}

/*! This function returns cosine of maximum quadrilateral warpage angle. */
double YsCalculateQuadWarpage(const YsVec3 p[4]);


/*! This function returns an angle from the refVec to the tstVec counter-clockwise with respect to the unitClockFaceDir.

    The angle is measured on the clock-face plane.

    unitClockFaceDir must be a unit vector, but tstVec and refVec don't have to be of unit length.
    */
double YsCalculateCounterClockwiseAngle(const YsVec3 &tstVec,const YsVec3 &refVec,const YsVec3 &unitClockFaceDir);

/*! This function calculates number of blocks in x-, y-, and z-directions if the bounding box is divided into rougly nTotal cells. 
    If unitBox is non-NULL, size of the lattice cell that divides the box into nx,ny,nz will be returned.
    unitBox can be NULL if this information is unnecessary.  */
void YsCalculateLatticeDivision3(int &nx,int &ny,int &nz,YsVec3 *unitBox,const YsVec3 bbx[2],int nLatticeCell);

/*! This function calculates number of blocks in x-, y-, and z-directions if the bounding box is divided into rougly nTotal cells. 
    If unitBox is non-NULL, size of the lattice cell that divides the box into nx,ny,nz will be returned.
    unitBox can be NULL if this information is unnecessary.  */
void YsCalculateLatticeDivision3(int &nx,int &ny,int &nz,YsVec3 *unitBox,const YsVec3 &bbx0,const YsVec3 &bbx1,int nLatticeCell);


////////////////////////////////////////////////////////////


/* } */
#endif
