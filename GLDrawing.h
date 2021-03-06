#pragma once
#include <vector>
#include "Matrix.h"
extern std::vector< Point > points;

void point(Point p);
void line(Vec2i p0, Vec2i p1, Color c);
Vec3f barycentric(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i P);
bool is_inside(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i P);
bool is_inside(Vec3f bary_coor);
int compute_z(Vec3i p1, Vec3i p2, Vec3i p3, Vec3f bary_coor);
int compute_z(Vec3i p1, Vec3i p2, Vec3i p3, Vec2i P);
int interpolate(int n0, int n1, int n2, Vec3f bary_coor);
float interpolate_f(float n0, float n1, float n2, Vec3f bary_coor);
Vec3f transform(Vec3f p0);
void InitZbuffer();
void LookAt(Vec3f camera, Vec3f up, Vec3f root);
void CalNormal(Vec3i normal);
Vec3f TransformPoint(Vec3f point);