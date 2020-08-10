#pragma once
#include <vector>
 extern std::vector< Point > points;

 void point(Point p);
 void line(Vec2i p0, Vec2i p1, Color c);
 void triangle(Vec3i p0, Vec3i p1, Vec3i p3, Color c);
 Vec3f barycentric(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i P);
 bool is_inside(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i P);
 int compute_z(Vec3i p1, Vec3i p2, Vec3i p3, Vec3f bary_coor);
 int compute_z(Vec3i p1, Vec3i p2, Vec3i p3, Vec2i P);
 void InitZbuffer();