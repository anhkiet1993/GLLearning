#ifndef _MODEL_H_
#define _MODEL_H_

#include <string>
#include <vector>
#include <array>
#include "geometry.h"
#include "tgaimage.h"

class Model
{
public:
    Model(std::string file_name);
    ~Model();

    void Draw(Color c);

    void LoadTexture(std::string text_file);

	void RenderTexture();

private:
    struct face_t
    {
        int vert_idx[3], tex_idx[3], norm_idx[3];
        face_t(std::array<int, 3> &v, std::array<int, 3> &t, std::array<int, 3> &n) : vert_idx{ v[0], v[1], v[2] }, tex_idx{ t[0], t[1], t[2] }, norm_idx{ n[0], n[1], n[2] }{};
    };

	Color GetColor(Vec2i coor);

    TGAImage m_texture;
	unsigned char *m_tex_data;

    std::vector<Vec3f> m_verts;
    std::vector<Vec3f> m_vert_texs;
    std::vector<Vec3f> m_vert_normals;
    std::vector<Vec3f> m_vert_spaces;
    std::vector<face_t> m_faces;

    std::vector<std::string> extract_string(std::string, char);

	void triangle(Vec3i p0, Vec3i p1, Vec3i p3, Color c);
	void triangle(Vec3i p0, Vec3i p1, Vec3i p2, Vec2i vt0, Vec2i vt1, Vec2i vt2);
};

#endif // !_MODEL_H_