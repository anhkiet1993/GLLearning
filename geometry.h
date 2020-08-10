#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_
#include <cmath>

template <typename T>
struct Vec2
{
    union
    {
        struct { T x, y; };
        struct { T u, v; };
        T raw[2];
    };

    Vec2(T x1 = 0, T y1 = 0) : x(x1), y(y1) {};

    inline Vec2<T> operator+ (const Vec2<T> &n) { return Vec2<T>(x + n.x, y + n.y); };
    inline Vec2<T> operator- (const Vec2<T> &n) { return Vec2<T>(x - n.x, y - n.y); };
    inline Vec2<T> operator* (const float &n) { return Vec2<T>(n*x, n*y); };
    inline T operator* (const Vec2<T> &n) { return ((x * n.x) + (y * n.y)); };
};

template <typename T>
struct Vec3
{
    union
    {
        struct { T x, y, z; };
        struct { T u, v, w; };
        T raw[3];
    };

    Vec3(T x1 = 0, T y1 = 0, T z1 = 0) : x(x1), y(y1), z(z1) {};

    inline Vec3<T> operator+ (const Vec3<T> &n) { return Vec3<T>(x + n.x, y + n.y, z + n.z); };
    inline Vec3<T> operator- (const Vec3<T> &n) { return Vec3<T>(x - n.x, y - n.y, z - n.z); };
    inline Vec3<T> operator* (const float &n) { return Vec3<T>(n*x, n*y, n*z); };
    inline T operator* (const Vec3<T> &n) { return ((x * n.x) + (y * n.y) + (z * n.z)); };
    inline Vec3<T> operator^ (const Vec3<T> &n) { return Vec3<T>(y * n.z - z * n.y, z * n.x - x * n.z, x * n.y - y * n.x); };
    inline Vec3<float> normalize() {
        return Vec3<float>((float)x / std::sqrt(x * x + y * y + z * z), (float)y / std::sqrt(x * x + y * y + z * z), (float)z / std::sqrt(x * x + y * y + z * z));
    };
};

typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;
typedef Vec3<int> Vec3i;
typedef Vec2<int> Vec2i;

struct Color
{
    unsigned char r, g, b, a;
};

struct Point
{
    Vec2i coor;
    Color color;
};


#define WHITE Color{255,255,255,255}
#define RED   Color{255,0,0,255}
#define TOP 800
#define RIGHT 800
#endif // !_GEOMETRY_H_