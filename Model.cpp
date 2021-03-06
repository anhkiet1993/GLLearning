#include "Model.h"
#include <fstream>
#include <sstream>
#include <assert.h>
#include "GLDrawing.h"
#include "geometry.h"
#include <string>
#include <iostream>

extern Vec3f light_dir;
extern Vec3f camera;
extern std::array<std::array<int, TOP>, RIGHT> gZbuffer;
extern Vec3f eye;
extern Vec3f center;
extern Vec3f up;

Model::Model(std::string file_name) : ModelView(4,4)
{
	std::ifstream f(file_name);
    assert(f);
    
    while (!f.eof())
    {
        std::string line;
        std::getline(f, line);

        if (!line.empty())
        {
            std::vector<std::string> parts = extract_string(line, ' ');

            if (parts[0] == "v")
            {
                m_verts.push_back(Vec3f(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
            }
            else if (parts[0] == "vt")
            {
                m_vert_texs.push_back(Vec3f(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
            }
            else if (parts[0] == "vn")
            {
                m_vert_normals.push_back(Vec3f(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
            }
            else if (parts[0] == "vp")
            {
                //m_vert_space.push_back(Vec3f(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
            }
            else if (parts[0] == "f")
            {
                std::string d_sep;
                std::vector<std::string>v1 = extract_string(parts[1], '/');
                std::vector<std::string>v2 = extract_string(parts[2], '/');
                std::vector<std::string>v3 = extract_string(parts[3], '/');

                std::array<int, 3> v = { std::stoi(v1[0]), std::stoi(v2[0]), std::stoi(v3[0]) };
                std::array<int, 3> t = { std::stoi(v1[1]), std::stoi(v2[1]), std::stoi(v3[1]) };
                std::array<int, 3> n = { std::stoi(v1[2]), std::stoi(v2[2]), std::stoi(v3[2]) };

                m_faces.push_back(face_t(v, t, n));
            }
            else
            {

            }
        }
    }
}

Model::~Model()
{

}

void Model::Draw(Color c)
{
    for (auto face : m_faces)
    {
        Vec3i p1((m_verts[face.vert_idx[0] - 1].x + 1)*400, (m_verts[face.vert_idx[0] - 1].y + 1)*400, (m_verts[face.vert_idx[0] - 1].z + 1)*400);
        Vec3i p2((m_verts[face.vert_idx[1] - 1].x + 1)*400, (m_verts[face.vert_idx[1] - 1].y + 1)*400, (m_verts[face.vert_idx[1] - 1].z + 1)*400);
        Vec3i p3((m_verts[face.vert_idx[2] - 1].x + 1)*400, (m_verts[face.vert_idx[2] - 1].y + 1)*400, (m_verts[face.vert_idx[2] - 1].z + 1)*400);
        triangle(p1, p2, p3, c);
    }
}

std::vector<std::string> Model::extract_string(std::string input, char delimit)
{
    std::istringstream ss(input);
    std::vector<std::string> ret_val;
    while (!ss.eof())
    {
        std::string part;
        std::getline(ss, part, delimit);
        if (part != "")
        {
            ret_val.push_back(part);
        }
    }
    return ret_val;
}

void Model::LoadTexture(std::string text_file)
{
    m_texture.read_tga_file(text_file.c_str());
	m_tex_data = m_texture.buffer();
}

Color Model::GetColor(Vec2i coor)
{
	coor.y = m_texture.get_width() - coor.y - 1;
	TGAColor c = m_texture.get(coor.x, coor.y);
	return Color{ c.r, c.g, c.b, c.a };
}

void Model::RenderTexture()
{
	for (auto face : m_faces)
	{
		Vec3i p0((m_verts[face.vert_idx[0] - 1].x + 1) * 400, (m_verts[face.vert_idx[0] - 1].y + 1) * 400, (m_verts[face.vert_idx[0] - 1].z + 1) * 400);
		Vec3i p1((m_verts[face.vert_idx[1] - 1].x + 1) * 400, (m_verts[face.vert_idx[1] - 1].y + 1) * 400, (m_verts[face.vert_idx[1] - 1].z + 1) * 400);
		Vec3i p2((m_verts[face.vert_idx[2] - 1].x + 1) * 400, (m_verts[face.vert_idx[2] - 1].y + 1) * 400, (m_verts[face.vert_idx[2] - 1].z + 1) * 400);

		Vec2i vt0((m_vert_texs[face.tex_idx[0] - 1].x) * m_texture.get_width()/2, (m_vert_texs[face.tex_idx[0] - 1].y) * m_texture.get_height()/2);
		Vec2i vt1((m_vert_texs[face.tex_idx[1] - 1].x) * m_texture.get_width()/2, (m_vert_texs[face.tex_idx[1] - 1].y) * m_texture.get_height()/2);
		Vec2i vt2((m_vert_texs[face.tex_idx[2] - 1].x) * m_texture.get_width()/2, (m_vert_texs[face.tex_idx[2] - 1].y) * m_texture.get_height()/2);

		triangle(p0, p1, p2, vt0, vt1, vt2);
	}
}

void Model::RenderInScence()
{
	for (auto face : m_faces)
	{
		Vec3f p0 = m_verts[face.vert_idx[0] - 1];
		Vec3f p1 = m_verts[face.vert_idx[1] - 1];
		Vec3f p2 = m_verts[face.vert_idx[2] - 1];

		Vec2i vt0((m_vert_texs[face.tex_idx[0] - 1].x) * m_texture.get_width(), (m_vert_texs[face.tex_idx[0] - 1].y) * m_texture.get_height());
		Vec2i vt1((m_vert_texs[face.tex_idx[1] - 1].x) * m_texture.get_width(), (m_vert_texs[face.tex_idx[1] - 1].y) * m_texture.get_height());
		Vec2i vt2((m_vert_texs[face.tex_idx[2] - 1].x) * m_texture.get_width(), (m_vert_texs[face.tex_idx[2] - 1].y) * m_texture.get_height());

		Vec3f normal = (p2 - p0) ^ (p1 - p0);
		float intensity = normal.normalize() * light_dir;

		p0 = transform(p0);
		p1 = transform(p1);
		p2 = transform(p2);

		Vec3i pt0((p0.x + 1) * 400, (p0.y + 1) * 400, p0.z * 400);
		Vec3i pt1((p1.x + 1) * 400, (p1.y + 1) * 400, p1.z * 400);
		Vec3i pt2((p2.x + 1) * 400, (p2.y + 1) * 400, p2.z * 400);

		if (intensity > 0.f)
		{
			triangle(pt0, pt1, pt2, vt0, vt1, vt2, intensity);
		}
	}
}

void Model::RenderShading()
{
	for (auto face : m_faces)
	{
		Vec3i p0((m_verts[face.vert_idx[0] - 1].x + 1) * 400, (m_verts[face.vert_idx[0] - 1].y + 1) * 400, (m_verts[face.vert_idx[0] - 1].z) * 400);
		Vec3i p1((m_verts[face.vert_idx[1] - 1].x + 1) * 400, (m_verts[face.vert_idx[1] - 1].y + 1) * 400, (m_verts[face.vert_idx[1] - 1].z) * 400);
		Vec3i p2((m_verts[face.vert_idx[2] - 1].x + 1) * 400, (m_verts[face.vert_idx[2] - 1].y + 1) * 400, (m_verts[face.vert_idx[2] - 1].z) * 400);

		float intensity0 = m_vert_normals[face.norm_idx[0] - 1] * light_dir;
		float intensity1 = m_vert_normals[face.norm_idx[1] - 1] * light_dir;
		float intensity2 = m_vert_normals[face.norm_idx[2] - 1] * light_dir;
		
		triangle(p0, p1, p2, intensity0, intensity1, intensity2);
	}
}

void Model::RenderFromLook()
{
	for (auto face : m_faces)
	{
		Vec3f trp0 = TransformPoint(Vec3f(m_verts[face.vert_idx[0] - 1].x, m_verts[face.vert_idx[0] - 1].y, m_verts[face.vert_idx[0] - 1].z));
		Vec3f trp1 = TransformPoint(Vec3f(m_verts[face.vert_idx[1] - 1].x, m_verts[face.vert_idx[1] - 1].y, m_verts[face.vert_idx[1] - 1].z));
		Vec3f trp2 = TransformPoint(Vec3f(m_verts[face.vert_idx[2] - 1].x, m_verts[face.vert_idx[2] - 1].y, m_verts[face.vert_idx[2] - 1].z));

		Vec3i p0((trp0.x + 1) * 400, (trp0.y + 1) * 400, (trp0.z) * 400);
		Vec3i p1((trp1.x + 1) * 400, (trp1.y + 1) * 400, (trp1.z) * 400);
		Vec3i p2((trp2.x + 1) * 400, (trp2.y + 1) * 400, (trp2.z) * 400);

		float intensity0 = m_vert_normals[face.norm_idx[0] - 1] * light_dir;
		float intensity1 = m_vert_normals[face.norm_idx[1] - 1] * light_dir;
		float intensity2 = m_vert_normals[face.norm_idx[2] - 1] * light_dir;
		
		triangle(p0, p1, p2, intensity0, intensity1, intensity2);
	}
}

void Model::LookAt()
{
	Vec3f vec1 = (eye - center).normalize(); //z
	Vec3f vec2 = up.normalize(); //y
	Vec3f vec3 = (vec1 ^ vec2).normalize(); //x

	Matrix<int> Minv(4, 4);
	Minv.identity();

	Matrix<int> R(4, 4);
	R.identity();

	for (int i = 0; i < 3; i++)
	{
		Minv[0][i] = vec3.raw[i];
		Minv[1][i] = vec2.raw[i];
		Minv[2][i] = vec1.raw[i];

		R[i][3] = -center.raw[i];
	}

	ModelView = Minv * R;
	
}

void Model::triangle(Vec3i p0, Vec3i p1, Vec3i p2, Color c)
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

void Model::triangle(Vec3i p0, Vec3i p1, Vec3i p2, Vec2i vt0, Vec2i vt1, Vec2i vt2)
{
	Vec2i top_left, bot_right;

	top_left.x = std::max(std::min(p0.x, std::min(p1.x, p2.x)), 0);
	top_left.y = std::max(std::min(p0.y, std::min(p1.y, p2.y)), 0);

	bot_right.x = std::min(std::max(p0.x, std::max(p1.x, p2.x)), RIGHT);
	bot_right.y = std::min(std::max(p0.y, std::max(p1.y, p2.y)), RIGHT);

	Vec3i face_normal = (p2 - p0) ^ (p1 - p0);
	float intensity = face_normal.normalize() * light_dir;

	//Color face_color = { c.r * intensity, c.g * intensity, c.b * intensity, c.a };

	for (int i = top_left.x; i < bot_right.x; i++)
	{
		for (int j = top_left.y; j < bot_right.y; j++)
		{
			Vec3f bary_coor = barycentric(Vec2i(p0.x, p0.y), Vec2i(p1.x, p1.y), Vec2i(p2.x, p2.y), Vec2i(i, j));
			if (is_inside(bary_coor))
			{
				int z = interpolate(p0.z, p1.z, p2.z, bary_coor);
				if (z > gZbuffer[i][j])
				{
					gZbuffer[i][j] = z;
					if (intensity > 0.f)
					{
						Vec2i color_coor;
						color_coor.x = interpolate(vt0.x, vt1.x, vt2.x, bary_coor);
						color_coor.y = interpolate(vt0.y, vt1.y, vt2.y, bary_coor);

						Color point_color = GetColor(color_coor);

						Point pt = { Vec2i(i, j), Color{(unsigned char)(point_color.r * intensity), (unsigned char)(point_color.g * intensity), (unsigned char)(point_color.b * intensity), point_color.a} };
						point(pt);
					}
				}
			}
		}
	}
}

void Model::triangle(Vec3i p0, Vec3i p1, Vec3i p2, Vec2i vt0, Vec2i vt1, Vec2i vt2, float intensity)
{
	Vec2i top_left, bot_right;

	top_left.x = std::max(std::min(p0.x, std::min(p1.x, p2.x)), 0);
	top_left.y = std::max(std::min(p0.y, std::min(p1.y, p2.y)), 0);

	bot_right.x = std::min(std::max(p0.x, std::max(p1.x, p2.x)), RIGHT);
	bot_right.y = std::min(std::max(p0.y, std::max(p1.y, p2.y)), RIGHT);


	for (int i = top_left.x; i < bot_right.x; i++)
	{
		for (int j = top_left.y; j < bot_right.y; j++)
		{
			Vec3f bary_coor = barycentric(Vec2i(p0.x, p0.y), Vec2i(p1.x, p1.y), Vec2i(p2.x, p2.y), Vec2i(i, j));
			if (is_inside(bary_coor))
			{
				int z = interpolate(p0.z, p1.z, p2.z, bary_coor);
				if (z > gZbuffer[i][j])
				{
					gZbuffer[i][j] = z;
					if (intensity > 0.f)
					{
						Vec2i color_coor;
						color_coor.x = interpolate(vt0.x, vt1.x, vt2.x, bary_coor);
						color_coor.y = interpolate(vt0.y, vt1.y, vt2.y, bary_coor);

						Color point_color = GetColor(color_coor);

						Point pt = { Vec2i(i, j), Color{ (unsigned char)(point_color.r * intensity), (unsigned char)(point_color.g * intensity), (unsigned char)(point_color.b * intensity), point_color.a } };
						point(pt);
					}
				}
			}
		}
	}
}

void Model::triangle(Vec3i p0, Vec3i p1, Vec3i p2, float intensity0, float intensity1, float intensity2)
{
	Vec2i top_left, bot_right;

	top_left.x = std::max(std::min(p0.x, std::min(p1.x, p2.x)), 0);
	top_left.y = std::max(std::min(p0.y, std::min(p1.y, p2.y)), 0);

	bot_right.x = std::min(std::max(p0.x, std::max(p1.x, p2.x)), RIGHT);
	bot_right.y = std::min(std::max(p0.y, std::max(p1.y, p2.y)), RIGHT);


	for (int i = top_left.x; i < bot_right.x; i++)
	{
		for (int j = top_left.y; j < bot_right.y; j++)
		{
			Vec3f bary_coor = barycentric(Vec2i(p0.x, p0.y), Vec2i(p1.x, p1.y), Vec2i(p2.x, p2.y), Vec2i(i, j));
			if (is_inside(bary_coor))
			{
				//int z = compute_z(p0, p1, p2, Vec2i(i, j));
				int z = interpolate(p0.z, p1.z, p2.z, bary_coor);
				float intensity = interpolate_f(intensity0, intensity1, intensity2, bary_coor);
				if (z >= gZbuffer[i][j])
				{
					gZbuffer[i][j] = z;
					if (intensity >= 0.000000000000f)
					{
						Color c = { WHITE.r * intensity, WHITE.g * intensity, WHITE.b * intensity, 255 };

						Point pt = { Vec2i(i, j), c };
						point(pt);
					}
					else
					{
						Color c = { WHITE.r * intensity, WHITE.g * intensity, WHITE.b * intensity, 255 };

						Point pt = { Vec2i(i, j), c };
						point(pt);
					}
				}
			}
		}
	}
}