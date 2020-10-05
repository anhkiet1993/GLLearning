#include <cstdlib>
#include <glut.h>
#include <vector>
#include "Matrix.h"
#include "geometry.h"
#include "GLDrawing.h"
#include "Model.h"
#include <ostream>
#include "tgaimage.h"


//std::ostream log(".\\output.log");

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, RIGHT, 0, TOP, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw
    glColor3ub(255, 255, 255);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_INT, sizeof(Point), &points[0].coor.x);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Point), &points[0].color.r);
    glPointSize(3.0);
    glDrawArrays(GL_POINTS, 0, points.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glFlush();
    glutSwapBuffers();
}

void Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(RIGHT, TOP);
	glutCreateWindow("Random Points");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	InitZbuffer();

	//line(Vec2i(0, 0), Vec2i(400, 400), WHITE);

	Model object("obj/african_head.obj");
	//object.Draw(WHITE);
    LookAt(Vec3f(1000, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 0));
    object.LoadTexture("obj/african_head_diffuse.tga");
    object.RenderFromLook();
    //object.RenderShading();

	////Matrix<int> m1(2, 2);
	//int m1a[] = { 1,2,3,4,5,6 };
	//Matrix<int> m1(3, 2, m1a);

	//int m2a[] = { 7,8,9,10,11,12 };
	//Matrix<int> m2(2, 3, m2a);

	//Matrix<int> m3 = m1*m2;

	//int a = m3[0][0];


    glutMainLoop();
    return 0;
}