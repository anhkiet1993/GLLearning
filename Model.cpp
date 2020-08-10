#include "Model.h"
#include <fstream>
#include <sstream>
#include <assert.h>
#include "GLDrawing.h"



Model::Model(std::string file_name)
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

}