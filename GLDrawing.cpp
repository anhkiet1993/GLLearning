#include "geometry.h"
#include "GLDrawing.h"
#include <algorithm>
#include <stdlib.h> 
#include <array>
#include "Matrix.h"

std::array<std::array<int, TOP>, RIGHT> gZbuffer; //buffer[RIGHT][TOP]
std::vector< Point > points;
Vec3f light_dir(0, 0, -1);

void point(Point p)
{
	points.push_back(p);
}

void line(Vec2i p0, Vec2i p1, Color c)
{
	for (float t = 0.; t < 1.; t += .01)
	{

		float x = p0.x + (p1.x - p0.x)*t;
		float y = p0.y + (p1.y - p0.y)*t;

		point({ Vec2i(int(x), int(y)), c });
	}
}

Vec3f barycentric(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i P)
{
    Vec3i u = Vec3i(p3.x - p1.x, p2.x - p1.x, p1.x - P.x) ^ Vec3i(p3.y - p1.y, p2.y - p1.y, p1.y - P.y);
    if (std::abs(u.z) < 1)
    {
        return Vec3f(-1, 1, 1);
    }
    return Vec3f(1.f - (float)u.x / u.z - (float)u.y / u.z, (float)u.y / u.z, (float)u.x / u.z);
}

bool is_inside(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i P)
{
	Vec3f b = barycentric(p1, p2, p3, P);

	return is_inside(b);
}

bool is_inside(Vec3f bary_coor)
{
	if (bary_coor.u < 0 || bary_coor.v < 0 || bary_coor.w < 0)
	{
		return false;
	}
	return true;
}

int compute_z(Vec3i p1, Vec3i p2, Vec3i p3, Vec3f bary_coor)
{
    return bary_coor.u * p1.z + bary_coor.v * p2.z + bary_coor.w * p3.z;
}

int compute_z(Vec3i p1, Vec3i p2, Vec3i p3, Vec2i P)
{
    Vec3f b = barycentric(Vec2i(p1.x, p1.y), Vec2i(p2.x, p2.y), Vec2i(p2.x, p2.y), P);
    return compute_z(p1, p2, p3, b);
}

int interpolate(int n0, int n1, int n2, Vec3f bary_coor)
{
	return bary_coor.u * n0 + bary_coor.v * n1 + bary_coor.w * n2;
}

Vec3f transform(Vec3f p0, int c)
{
	float array_m[] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,-1.f/c,1 };
	Matrix<float> transform_matrix(4, 4, array_m);

	float fmp0[] = { p0.x,p0.y,p0.z,1 };
	Matrix<float> mp0(1, 4, fmp0);

	Matrix<float>rel = transform_matrix*mp0;

	Vec3f p_t(rel[0][0] / rel[3][0], rel[1][0] / rel[3][0], rel[2][0] / rel[3][0]);
	//p_t.x = (float)p0.x / (1.f - (float)p0.z / c);
	//p_t.y = (float)p0.y / (1.f - (float)p0.z / c);
	//p_t.z = (float)p0.z / (1.f - (float)p0.z / c);

	return p_t;
}

void InitZbuffer()
{
    for (int i = 0; i < RIGHT; i++)
    {
        for (int j = 0; j < TOP; j++)
        {
            gZbuffer[i][j] = std::numeric_limits<int>::min();
        }
    }
}