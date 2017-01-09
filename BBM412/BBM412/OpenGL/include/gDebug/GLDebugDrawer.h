#ifndef GL_DEBUG_DRAWER_H
#define GL_DEBUG_DRAWER_H

#include "LinearMath/btIDebugDraw.h"

#include "GLDebugFont.h"
#include "GlutStuff.h"

#include <stdio.h>
using namespace std;

class GLDebugDrawer : public btIDebugDraw
{
	int m_debugMode;

	enum {LINE,TRIANGLE,SIZE};
	GLuint vao[SIZE], vbo[SIZE];
	GLuint prog;

public:

	GLDebugDrawer()
		:m_debugMode(0)
	{

	}
	static inline void pushMatrix(glm::mat4 mat) {
		glUseProgram(0);
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(mat));
	}
	static inline void popMatrix(){
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	virtual ~GLDebugDrawer( )
	{

	}

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
	{

		glBegin(GL_LINES);

			glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
			glVertex3f(from.getX(), from.getY(), from.getZ());
			glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
			glVertex3f(to.getX(), to.getY(), to.getZ());

		glEnd();


	}

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		drawLine(from, to, color, color);
	}

	virtual void	drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
	{
		cout << "drawSphere" << endl;
		/*
		glColor4f(color.getX(), color.getY(), color.getZ(), btScalar(1.0f));
		glPushMatrix();
		glTranslatef(p.getX(), p.getY(), p.getZ());

		int lats = 5;
		int longs = 5;

		int i, j;
		for (i = 0; i <= lats; i++) {
			btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar)(i - 1) / lats);
			btScalar z0 = radius*sin(lat0);
			btScalar zr0 = radius*cos(lat0);

			btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar)i / lats);
			btScalar z1 = radius*sin(lat1);
			btScalar zr1 = radius*cos(lat1);

			glBegin(GL_QUAD_STRIP);
			for (j = 0; j <= longs; j++) {
				btScalar lng = 2 * SIMD_PI * (btScalar)(j - 1) / longs;
				btScalar x = cos(lng);
				btScalar y = sin(lng);

				glNormal3f(x * zr0, y * zr0, z0);
				glVertex3f(x * zr0, y * zr0, z0);
				glNormal3f(x * zr1, y * zr1, z1);
				glVertex3f(x * zr1, y * zr1, z1);
			}
			glEnd();
		}

		glPopMatrix();*/
	}

	virtual void	drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& _color, btScalar alpha)
	{
		//	if (m_debugMode > 0)
		{
			//cout << "drawTriangle" << endl;
			glm::vec3 color[3], point[3];


			color[0] = glm::vec3(_color.getX(), _color.getY(), _color.getZ());
			point[0] = glm::vec3(a.getX(), a.getY(), a.getZ());
			color[1] = glm::vec3(_color.getX(), _color.getY(), _color.getZ());
			point[1] = glm::vec3(b.getX(), b.getY(), b.getZ());
			color[2] = glm::vec3(_color.getX(), _color.getY(), _color.getZ());
			point[2] = glm::vec3(c.getX(), c.getY(), c.getZ());

			//const btVector3	n = btCross(b - a, c - a).normalized();

		}
	}

	virtual void	drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{

		{
			cout << "drawContactPoint" << endl;

			btVector3 to = pointOnB + normalOnB * 1;//distance;
			const btVector3&from = pointOnB;
			glColor4f(color.getX(), color.getY(), color.getZ(), 1.f);
			//glColor4f(0,0,0,1.f);
			glBegin(GL_LINES);
			glVertex3d(from.getX(), from.getY(), from.getZ());
			glVertex3d(to.getX(), to.getY(), to.getZ());
			glEnd();


			//		glRasterPos3f(from.x(),  from.y(),  from.z());
			//		char buf[12];
			//		sprintf(buf," %d",lifeTime);
			//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);


		}
	}
	virtual void	reportErrorWarning(const char* warningString)
	{
		printf("%s\n", warningString);
	}

	virtual void	draw3dText(const btVector3& location, const char* textString)
	{
		glRasterPos3f(location.x(), location.y(), location.z());
		//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
	}

	virtual void	setDebugMode(int debugMode)
	{
		m_debugMode = debugMode;

	}


	virtual int		getDebugMode() const { return m_debugMode;}

};

#endif//GL_DEBUG_DRAWER_H


