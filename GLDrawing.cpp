#include "geometry.h"
#include "GLDrawing.h"
#include <algorithm>
#include <stdlib.h> 
#include <array>

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

void triangle(Vec3i p0, Vec3i p1, Vec3i p2, Color c)
{
    Vec2i top_left, bot_right;

    top_left.x = std::max(std::min(p0.x, std::min(p1.x, p2.x)), 0);
    top_left.y = std::max(std::min(p0.y, std::min(p1.y, p2.y)), 0);

    bot_right.x = std::min(std::max(p0.x, std::max(p1.x, p2.x)), RIGHT);
    bot_right.y = std::min(std::max(p0.y, std::max(p1.y, p2.y)), RIGHT);

    Vec3i face_normal = (p2 - p0) ^ (p1 - p0);
    float intensity = face_normal.normalize() * light_dir;

    Color face_color = { c.r * intensity, c.g * intensity, c.b * intensity, c.a };
   
    for (int i = top_left.x; i < bot_right.x; i++)
    {
        for (int j = top_left.y; j < bot_right.y; j++)
        {
            if (is_inside(Vec2i(p0.x, p0.y), Vec2i(p1.x, p1.y), Vec2i(p2.x, p2.y), Vec2i(i, j)))
            {
                int z = compute_z(p0, p1, p2, Vec2i(i, j));
                if (z > gZbuffer[i][j])
                {
                    gZbuffer[i][j] = z;
                    if (intensity > 0.f)
                    {
                        Point pt = { Vec2i(i, j), face_color };
                        point(pt);
                    }
                }
            }
        }
    }
}

Vec3f barycentric(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i P)
{
    Vec3i u = Vec3i(p3.x - p1.x, p2.x - p1.x, p1.x - P.x) ^ Vec3i(p3.y - p1.y, p2.y - p1.y, p1.y - P.y);
    if (std::abs(u.z) < 1)
    {
        return Vec3f(-1, 1, 1);
    }
    return Vec3f(1.f - (float)u.x / u.z - (float)u.y / u.z, (float)u.x / u.z, (float)u.y / u.z);
}

bool is_inside(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i P)
{
    Vec3f b = barycentric(p1, p2, p3, P);
    if (b.u < 0 || b.v < 0 || b.w < 0)
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