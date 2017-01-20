#include "Sun.h"


GLuint Sun::VAO, Sun::VBO;
unsigned int Sun::POINTS_SIZE;
DirecLightData Sun::DIRECLIGHT_DATA{ glm::vec3{0,0,0}, glm::vec3{1,1,0} };
Texture Sun::TEXTURE;
Sun::Sun(): GBasicTextureObject("sun"),
	//GBasicLightObject(VAO,VBO,POINTS_SIZE,glm::vec4{1.0f,1.0f,0.0f,1}, glm::vec3{ 0,900,0 }),
	DirecLight(DIRECLIGHT_DATA, glm::vec3{ 0,900,0 }), GObject(glm::translate(glm::vec3{ 0,100,0 })),Moveable(glm::vec3{0,100,0})
{

}

inline void Sun::defaultRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	glm::mat4 mv = camera->getViewMatrix()*GModel;
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++) {
			if (i == j)
				mv[i][j] = 1.0f;
			else
				mv[i][j] = 0.0f;
		}
	mvp = camera->getProjMatrix()*mv;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void Sun::wireframeRender()
{
	glUseProgram(GProgram[(int)RenderMode::WIREFRAME]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;

	glm::mat4 mv = camera->getViewMatrix()*GModel;
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++) {
			if (i == j)
				mv[i][j] = 1.0f;
			else
				mv[i][j] = 0.0f;
		}
	mvp = camera->getProjMatrix()*mv;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &wireframeColor[0]);
	glDrawArrays(GL_LINES, 0, points_size);

}


inline void Sun::reflectionCalculationRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;

	glm::mat4 mv = camera->getViewMatrix()*GModel;
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++) {
			if (i == j)
				mv[i][j] = 1.0f;
			else
				mv[i][j] = 0.0f;
		}
	mvp = camera->getProjMatrix()*mv;

	mvp = mvp*reflectionMatrix;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


inline void Sun::defaultToggleRender(const glm::mat4 & model)
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));


	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void Sun::wireframeToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::WIREFRAME]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &wireframeColor[0]);
	glDrawArrays(GL_LINES, 0, points_size);

}


inline void Sun::reflectionCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model*reflectionMatrix;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}



void Sun::render()
{
	switch (renderMode) {

	case RenderMode::DEFAULT:
	case RenderMode::BASIC_TEXTURE:
		defaultRender();
		break;
	case RenderMode::WIREFRAME:
		wireframeRender();
		break;
	case RenderMode::SHADOW_CALC:
		break;

	case RenderMode::REFLECTION_CALC:
		reflectionCalculationRender();
		break;

	}

#ifdef _DEBUG
	throwError("Sun::render():\n");
#endif // _DEBUG

}


void Sun::toggleRender(const glm::mat4 & model_matrix)
{
	switch (renderMode) {

	case RenderMode::DEFAULT:
	case RenderMode::BASIC_TEXTURE:
		defaultToggleRender(model_matrix);
		break;
	case RenderMode::WIREFRAME:
		wireframeToggleRender(model_matrix);
		break;
	case RenderMode::SHADOW_CALC:
		break;

	}
#ifdef _DEBUG
	throwError("Sun::toggleRender():\n");
#endif // _DEBUG

}

void Sun::calculateLightening()
{

	if (degreeOfDay < 180 && !isActive())
		toggleActive();
	else if (degreeOfDay >= 180 && isActive())
		toggleActive();

	//cout << "DegreeOfDay:" << degreeOfDay << endl;

//	glm::vec3 cameraPosition = camera->getPosition();

	Moveable::position = glm::vec3(
		cos(glm::radians(degreeOfDay))* 800, 
		sin(glm::radians(degreeOfDay))* 800,
		0);

	GModel = glm::translate(position);
	
	//cout << "Position:" << position << endl;

	calculateDirecLightToCenter();

}

void Sun::initialize()
{
	construct("sun", "sun", VAO, VBO, POINTS_SIZE,TEXTURE);
	DIRECLIGHT_DATA = readLightFile("objects\\sun\\sun.light");
}


Sun::~Sun()
{
}
