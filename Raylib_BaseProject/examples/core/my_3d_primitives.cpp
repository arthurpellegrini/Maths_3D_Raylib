#include "my_3D_primitives.h"
#include <rlgl.h>
#include <iostream>


/******************************************************************
*							QUAD								  *
*******************************************************************/
void MyDrawPolygonQuad(Quad quad, Color color)
{
	int numVertex = 6;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(quad.ref.origin.x, quad.ref.origin.y, quad.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(quad.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(quad.extents.x, 1, quad.extents.z);
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	rlVertex3f(1, 0, 1);
	rlVertex3f(1, 0, -1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(1, 0, 1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(-1, 0, 1);
	rlEnd();
	rlPopMatrix();
}


void MyDrawWireframeQuad(Quad quad, Color color)
{
	int numVertex = 10;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(quad.ref.origin.x, quad.ref.origin.y, quad.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(quad.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(quad.extents.x, 1, quad.extents.z);
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	rlVertex3f(1, 0, 1);
	rlVertex3f(1, 0, -1);
	rlVertex3f(1, 0, -1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(1, 0, 1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(-1, 0, 1);
	rlVertex3f(-1, 0, 1);
	rlVertex3f(1, 0, 1);
	rlEnd();
	rlPopMatrix();
}


void MyDrawQuad(Quad quad, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor)
{
	if (drawPolygon) MyDrawPolygonQuad(quad, polygonColor);
	if (drawWireframe) MyDrawWireframeQuad(quad, wireframeColor);
}


/******************************************************************
*							PLANE								  *
*******************************************************************/
void MyDrawPlane(Plane plane, Color color)
{
	Vector3 origin = plane.n;
	Vector2 size = { plane.d, plane.d };
	DrawPlane(origin, size, color);
}


/******************************************************************
*							DISK								  *
*******************************************************************/
void MyDrawPolygonDisk(Disk disk, int nSectors, Color color) {
	int numVertex = nSectors * 3;
	rlPushMatrix();
	rlTranslatef(disk.ref.origin.x, disk.ref.origin.y, disk.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(disk.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(disk.radius, 1, disk.radius);

	Cylindrical v1, v2;

	for (int i = 0; i < nSectors; i++) {
		// On calcule les coordonnées cylindriques des sommets du triangle
		v1 = { 1, 2 * PI / nSectors * i, 0 };
		v2 = { 1, 2 * PI / nSectors * (i+1), 0 };

		if (rlCheckBufferLimit(numVertex)) rlglDraw();

		DrawTriangle3D(CylindricToCartesien(v2), { 0 }, CylindricToCartesien(v1), color);
	}
	rlPopMatrix();
}

void MyDrawWireframeDisk(Disk disk, int nSectors, Color color) {
	int numVertex = nSectors;
	rlPushMatrix();
	rlTranslatef(disk.ref.origin.x, disk.ref.origin.y, disk.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(disk.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(disk.radius, 1, disk.radius);

	Cylindrical v1, v2;

	for (int i = 0; i < nSectors; i++) {
		// On calcule les coordonnées cylindriques des points des segments
		v1 = { 1, 2 * PI / nSectors * i, 0 };
		v2 = { 1, 2 * PI / nSectors * (i + 1), 0 };

		if (rlCheckBufferLimit(numVertex)) rlglDraw();

		DrawLine3D(CylindricToCartesien(v1), CylindricToCartesien(v2), color);
		DrawLine3D({ 0 }, CylindricToCartesien(v2), color);
	}
	rlPopMatrix();
}

void MyDrawDisk(Disk disk, int nSectors, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonDisk(disk, nSectors, polygonColor);
	if (drawWireframe) MyDrawWireframeDisk(disk, nSectors, wireframeColor);
}


/******************************************************************
*							BOX 								  *
*******************************************************************/
void MyDrawPolygonBox(Box box, Color color) {
	int numVertex = 36;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(box.ref.origin.x, box.ref.origin.y, box.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(box.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(box.extents.x, box.extents.y, box.extents.z);

	//{ X -> horizontal; Y -> vertical; Z -> far }
	Vector3 front_top_left		= { -1,  1,  1 };
	Vector3 front_top_right		= {  1,  1,  1 };
	Vector3 front_bottom_left	= { -1, -1,  1 };
	Vector3 front_bottom_right	= {  1, -1,  1 };
	Vector3 back_top_left		= { -1,  1, -1 };
	Vector3 back_top_right		= {  1,  1, -1 };
	Vector3 back_bottom_left	= { -1, -1, -1 };
	Vector3 back_bottom_right	= {  1, -1, -1 };


	//It is necessary to follow the trigonometric direction to have a good orientation of the triangle.
	// 
	//FRONT  
	DrawTriangle3D(front_bottom_right, front_top_left, front_bottom_left, color);
	DrawTriangle3D(front_bottom_right, front_top_right, front_top_left, color);
	//BACK
	DrawTriangle3D(back_bottom_left, back_top_left, back_top_right, color);
	DrawTriangle3D(back_top_right, back_bottom_right, back_bottom_left, color);
	//TOP 
	DrawTriangle3D(back_top_left, front_top_left, front_top_right, color);
	DrawTriangle3D(front_top_right, back_top_right, back_top_left, color);
	//BOTTOM
	DrawTriangle3D(back_bottom_right, front_bottom_left, back_bottom_left, color);
	DrawTriangle3D(back_bottom_right, front_bottom_right, front_bottom_left, color);
	//LEFT
	DrawTriangle3D(front_bottom_left, back_top_left, back_bottom_left, color);
	DrawTriangle3D(front_bottom_left, front_top_left, back_top_left, color);
	//RIGHT
	DrawTriangle3D(front_top_right, front_bottom_right, back_bottom_right, color);
	DrawTriangle3D(front_top_right, back_bottom_right, back_top_right, color);
	
	rlPopMatrix();
}

void MyDrawWireframeBox(Box box, Color color) {
	int numVertex = 36;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(box.ref.origin.x, box.ref.origin.y, box.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(box.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(box.extents.x, box.extents.y, box.extents.z);
	
	//{ X -> horizontal; Y -> vertical; Z -> far }
	Vector3 front_top_left		= { -1,  1,  1 };
	Vector3 front_top_right		= {  1,  1,  1 };
	Vector3 front_bottom_left	= { -1, -1,  1 };
	Vector3 front_bottom_right	= {  1, -1,  1 };
	Vector3 back_top_left		= { -1,  1, -1 };
	Vector3 back_top_right		= {  1,  1, -1 };
	Vector3 back_bottom_left	= { -1, -1, -1 };
	Vector3 back_bottom_right	= {  1, -1, -1 };

	//FRONT
	DrawLine3D(front_top_left, front_top_right, color);
	DrawLine3D(front_bottom_left, front_bottom_right, color);
	DrawLine3D(front_top_left, front_bottom_left, color);
	DrawLine3D(front_top_right, front_bottom_right, color);
	DrawLine3D(front_bottom_right, front_top_left, color);
	//BACK
	DrawLine3D(back_top_left, back_top_right, color);
	DrawLine3D(back_bottom_left, back_bottom_right, color);
	DrawLine3D(back_top_left, back_bottom_left, color);
	DrawLine3D(back_top_right, back_bottom_right, color);
	DrawLine3D(back_bottom_left, back_top_right, color);
	//TOP 
	DrawLine3D(front_top_left, back_top_left, color);
	DrawLine3D(front_top_right, back_top_right, color);
	DrawLine3D(front_top_right, back_top_left, color);
	//BOTTOM
	DrawLine3D(front_bottom_left, back_bottom_left, color);
	DrawLine3D(front_bottom_right, back_bottom_right, color);
	DrawLine3D(front_bottom_left, back_bottom_right, color);
	//LEFT
	DrawLine3D(front_bottom_left, back_top_left, color);
	//RIGHT
	DrawLine3D(back_bottom_right, front_top_right, color);

	rlPopMatrix();
}

void MyDrawBox(Box box, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonBox(box, polygonColor);
	if (drawWireframe) MyDrawWireframeBox(box, wireframeColor);
}

/******************************************************************
*							SPHERE 								  *
*******************************************************************/
void MyDrawPolygonSphere(Sphere sphere, int nMeridians, int nParallels, Color color) {
	int numVertex = nMeridians * nParallels * 4;
	rlPushMatrix();
	rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(sphere.radius, sphere.radius, sphere.radius);

	// Points position for one quad in the sphere
	// 1-----2
	// | \   |
	// |  \  |
	// |   \ |
	// 3-----4
	Spherical pt1, pt2, pt3, pt4; // = {rho, theta, phi}

	//	MERIDIAN -> phi € [0°, 180°]
	//		|
	//		|
	// -----|----- PARALLEL -> theta € [0°, 360°]
	//		|
	//		|

	for (int m = 0; m < nMeridians; m++)
	{
		for (int p = 0; p < nParallels; p++)
		{
			pt1 = { 1, 2 * PI / nMeridians * m, PI / nParallels * p};
			pt2 = { 1, 2 * PI / nMeridians * (m+1), PI / nParallels * p };
			pt3 = { 1, 2 * PI / nMeridians * m, PI / nParallels * (p+1) };
			pt4 = { 1, 2 * PI / nMeridians * (m+1), PI / nParallels * (p+1) };

			if (rlCheckBufferLimit(numVertex)) rlglDraw();
			DrawTriangle3D(SphericalToCartesian(pt1), SphericalToCartesian(pt4), SphericalToCartesian(pt2), color);
			DrawTriangle3D(SphericalToCartesian(pt1), SphericalToCartesian(pt3), SphericalToCartesian(pt4), color);
		}
	}
	rlPopMatrix();
}

void MyDrawWireframeSphere(Sphere sphere, int nMeridians, int nParallels, Color color) {
	int numVertex = nMeridians * nParallels * 4;
	rlPushMatrix();
	rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(sphere.radius, sphere.radius, sphere.radius);

	// Points position for one quad in the sphere
	// 1-----2
	// | \   |
	// |  \  |
	// |   \ |
	// 3-----4
	Spherical pt1, pt2, pt3, pt4; // = {rho, theta, phi}

	//	MERIDIAN -> phi € [0°, 180°]
	//		|
	//		|
	// -----|----- PARALLEL -> theta € [0°, 360°]
	//		|
	//		|

	for (int m = 0; m < nMeridians; m++)
	{
		for (int p = 0; p < nParallels; p++)
		{
			pt1 = { 1, 2 * PI / nMeridians * m, PI / nParallels * p };
			pt2 = { 1, 2 * PI / nMeridians * (m + 1), PI / nParallels * p };
			pt3 = { 1, 2 * PI / nMeridians * m, PI / nParallels * (p + 1) };
			pt4 = { 1, 2 * PI / nMeridians * (m + 1), PI / nParallels * (p + 1) };

			if (rlCheckBufferLimit(numVertex)) rlglDraw();
			DrawLine3D(SphericalToCartesian(pt1), SphericalToCartesian(pt4), color);
			DrawLine3D(SphericalToCartesian(pt1), SphericalToCartesian(pt2), color);
			DrawLine3D(SphericalToCartesian(pt1), SphericalToCartesian(pt3), color);
		}
	}
	rlPopMatrix();
}

void MyDrawSphere(Sphere sphere, int nMeridians, int nParallels, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonSphere(sphere, nMeridians, nParallels, polygonColor);
	if (drawWireframe) MyDrawWireframeSphere(sphere, nMeridians, nParallels, wireframeColor);
}

/******************************************************************
*							CYLINDER							  *
*******************************************************************/
void MyDrawPolygonCylinder(Cylinder cylinder, int nSectors, bool drawCaps, Color color)
{
	int numVertex = nSectors * 3 * 2;
	rlPushMatrix();
	rlTranslatef(cylinder.ref.origin.x, cylinder.ref.origin.y, cylinder.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(cylinder.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(cylinder.radius, cylinder.halfHeight, cylinder.radius);

	Cylindrical v1, v2, v3, v4;

	for (int i = 0; i < nSectors; i++) {
		v1 = { 1, 2 * PI / nSectors * i, 1 };
		v2 = { 1, 2 * PI / nSectors * (i + 1), 1 };
		v3 = { 1, 2 * PI / nSectors * i, -1 };
		v4 = { 1, 2 * PI / nSectors * (i + 1), -1 };

		if (rlCheckBufferLimit(numVertex)) rlglDraw();

		if (drawCaps) { // alors dessin des disques supérieurs et inférieurs (formes discoïdales)
			DrawTriangle3D(CylindricToCartesien(v2), { 0, 1, 0 }, CylindricToCartesien(v1), color);
			DrawTriangle3D({ 0, -1, 0 }, CylindricToCartesien(v4), CylindricToCartesien(v3), color);
		}

		DrawTriangle3D(CylindricToCartesien(v1), CylindricToCartesien(v4), CylindricToCartesien(v2), color);
		DrawTriangle3D(CylindricToCartesien(v1), CylindricToCartesien(v3), CylindricToCartesien(v4), color);
	}
	rlPopMatrix();
}

void MyDrawWireframeCylinder(Cylinder cylinder, int nSectors, bool drawCaps, Color color)
{
	int numVertex = nSectors * 3 * 2;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(cylinder.ref.origin.x, cylinder.ref.origin.y, cylinder.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(cylinder.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(cylinder.radius, cylinder.halfHeight, cylinder.radius);

	Cylindrical v1, v2, v3, v4;

	for (int i = 0; i < nSectors; i++) {
		v1 = { 1, 2 * PI / nSectors * i, 1 };
		v2 = { 1, 2 * PI / nSectors * (i + 1), 1 };
		v3 = { 1, 2 * PI / nSectors * i, -1 };
		v4 = { 1, 2 * PI / nSectors * (i + 1), -1 };

		if (rlCheckBufferLimit(numVertex)) rlglDraw();

		if (drawCaps) { // alors dessin des disques supérieurs et inférieurs (formes discoïdales)
			DrawLine3D(CylindricToCartesien(v1), { 0, 1, 0 }, color);
			DrawLine3D(CylindricToCartesien(v2), { 0, 1, 0 }, color);
			DrawLine3D(CylindricToCartesien(v3), { 0, -1, 0 }, color);
			DrawLine3D(CylindricToCartesien(v4), { 0, -1, 0 }, color);
		}

		DrawLine3D(CylindricToCartesien(v1), CylindricToCartesien(v2), color);
		DrawLine3D(CylindricToCartesien(v3), CylindricToCartesien(v4), color);
		DrawLine3D(CylindricToCartesien(v1), CylindricToCartesien(v3), color);
		DrawLine3D(CylindricToCartesien(v2), CylindricToCartesien(v4), color);
		DrawLine3D(CylindricToCartesien(v1), CylindricToCartesien(v4), color);
	}
	rlPopMatrix();
}

void MyDrawCylinder(Cylinder cylinder, int nSectors, bool drawCaps, bool drawPolygon, bool drawWireframe, Color polygonColor, Color wireframeColor) {
	if (drawPolygon) MyDrawPolygonCylinder(cylinder, nSectors, drawCaps, polygonColor);
	if (drawWireframe) MyDrawWireframeCylinder(cylinder, nSectors, drawCaps, wireframeColor);
}