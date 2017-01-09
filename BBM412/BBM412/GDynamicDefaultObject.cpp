#include "GDynamicDefaultObject.h"

#include <sstream>
#include <iostream>



GDynamicDefaultObject::GDynamicDefaultObject(const string & obj,const GLuint & _prog ) : 
									GDynamicDefaultObject(obj , obj , glm::vec3(.0f), glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{
}

GDynamicDefaultObject::GDynamicDefaultObject(const string & sub, const string & obj, const GLuint & _prog) :
										GDynamicDefaultObject(sub, obj, glm::vec3(.0f), glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{
}

GDynamicDefaultObject::GDynamicDefaultObject(const string & obj, const glm::vec3 & dis,const GLuint & _prog ) : 
									GDynamicDefaultObject(obj, dis, glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{	
}

GDynamicDefaultObject::GDynamicDefaultObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular, const GLfloat & shininess, const GLuint & _prog):
	GDynamicDefaultObject(obj, obj, dis, _m_specular, shininess, _prog)

{
}

GDynamicDefaultObject::GDynamicDefaultObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog ) : 
									GDynamicDefaultObject(sub, obj, dis, glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{
}

GDynamicDefaultObject::GDynamicDefaultObject(const string & sub, const string & obj, const glm::vec3 & dis,
	const glm::vec3 & _m_specular, const GLfloat & shininess, const GLuint & _prog ) 
	: GDefaultObject{ sub, obj ,glm::translate(dis),_m_specular,shininess,_prog }, Moveable{dis} 
{
#ifdef _DEBUG
	GObject::throwError("World::GDynamicDefaultObject():\n");
#endif
}



GDynamicDefaultObject::GDynamicDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const GLuint & _prog ) :
	GDynamicDefaultObject(vbo, vao, _points_size, glm::mat4{ 1.0f }, glm::vec3{ 1.0f }, 32.0f, _prog)
{
}

GDynamicDefaultObject::GDynamicDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::mat4 & _model, const GLuint & _prog ) :
	GDynamicDefaultObject(vbo, vao, _points_size, _model, glm::vec3(0.5,0.5,0.5), 32.0f, _prog)
{
}

GDynamicDefaultObject::GDynamicDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
	const glm::mat4 & _model, const glm::vec3 & _m_specular, const GLfloat & _shininess, const GLuint & _prog ) :
	GDefaultObject(vbo,vao, points_size , model, _m_specular, _shininess, _prog)
{
#ifdef _DEBUG
	GObject::throwError("World::GDynamicDefaultObject():\n");
#endif
}

void GDynamicDefaultObject::toggleToggleMode()
{
	if (toggle)
	{


		toggle = false;
	}
	else
	{
		toggle = true;
	}
}

bool GDynamicDefaultObject::isToggleMode()
{
	return toggle;
}


inline void GDynamicDefaultObject::toggleDefaultRender()
{
	glUseProgram(program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffers[0]);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 1, uniform_buffers[1]);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 2, uniform_buffers[2]);

	glUniform1i(120, GObject::nums_uniform_buffers[0]);
	glUniform1i(121, GObject::nums_uniform_buffers[1]);
	glUniform1i(122, GObject::nums_uniform_buffers[2]);


	glUniform3f(101, 1.0, 1.0, 1.0);
	glUniform1f(102, 64);


	glm::mat4 mvp;
	glm::mat4 mv;
	glm::mat3 normal_mv;


	mvp = GObject::camera->getProjMatrix()*model;
	mv = model;

	normal_mv = glm::mat3(glm::inverseTranspose(mv));

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(normal_mv));

	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(GObject::camera->getViewMatrix()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, 0);



	glDrawArrays(GL_TRIANGLES, 0, points_size);

	throwError("GDynamicObject::render():\n");
}

void GDynamicDefaultObject::render()
{
	switch (renderMode) {
	case RenderMode::DEFAULT:
		if (toggle)
		{
			toggleDefaultRender();
		}
		else
		{
			defaultRender();
		}
		break;

	}


}


void GDynamicDefaultObject::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	model = calculateModelMatrix();
}

void GDynamicDefaultObject::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	model = calculateModelMatrix();
}

void GDynamicDefaultObject::translateOn(float x, float y, float z, const Directions & _dir)
{
	Moveable::translateOn(x, y, z, _dir);
	model = calculateModelMatrix();
}

void GDynamicDefaultObject::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	model = calculateModelMatrix();
}

void GDynamicDefaultObject::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	model = calculateModelMatrix();
}


GDynamicDefaultObject::~GDynamicDefaultObject()
{
}