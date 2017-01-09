#include "GObject.h"
#include "GExceptions.h"

/**/

/**/

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

using namespace std;

glm::mat4 GObject::lightView;
glm::mat4 GObject::lightViewProj;
#ifdef _DEBUG
	glm::mat4 GObject::debug_view;
	glm::mat4 GObject::debug_viewProj;
#endif

GLuint GObject::sunShadowMap;

GLuint GObject::GProgram[(int)RenderMode::SIZE];

GLuint GObject::uniform_buffers[UNIFORM_BUFFER_SIZE];
GLuint GObject::nums_uniform_buffers[UNIFORM_BUFFER_SIZE];

RenderMode GObject::renderMode;

unsigned int GObject::nextAvaibleTextureUnit;

const vector<shared_ptr<DirecLight>> *GObject::direc_lights;
const vector<shared_ptr<PointLight>> *GObject::point_lights;
const vector<shared_ptr<SpotLight>> *GObject::spot_lights;

GLuint	GObject::compileShader(const char * file_dir, GLenum type)
{
	GLuint shader = glCreateShader(type);

	fstream file{ file_dir , ios::in };

#if (DEBUG_LVL>=1)
	cout<<"file direction:"<< file_dir << "[end]" << endl;
#endif // DEBUG

	if (!file.is_open())
		throw GException("Shader file is missing.(Put them in the program`s file directory.)");

	string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	const char * source = contents.c_str();

	file.close();

#if (DEBUG_LVL>=1)
	cout << source<<"[end]"<<endl;
#endif // DEBUG

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint  compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);


	if (!compiled) {

		int logSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetShaderInfoLog(shader, logSize, NULL, logMsg);
		string msg("Shader Not Compiled:\n"); msg += logMsg;
		delete[] logMsg;

		throw GException( msg );
	}

	return shader;
}
#ifdef _DEBUG

void GObject::setWireframeColor(const glm::vec4 & _color)
{
	wireframeColor = _color;
}

glm::vec4 GObject::getWireframeColor()
{
	return wireframeColor;
}

void GObject::setDebugViewMatrix(const glm::mat4 & mat)
{
	debug_view = mat;
}

void GObject::setDebugViewProjMatrix(const glm::mat4 & view, const glm::mat4 & proj)
{
	debug_view = view;
	debug_viewProj = proj*view;
}
#endif 

GObject::GObject()
{

}

GLuint GObject::createProgram(const char * vshader, const char * fshader)
{
	GLuint vertex_shader = compileShader(vshader, GL_VERTEX_SHADER);
	GLuint fragment_shader = compileShader(fshader, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);

	GLint  linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {

		string msg("Program Not Linked:\n");
		GLint  logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		msg += logMsg;
		delete[] logMsg;

		throw GException(msg);

	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return program;
}

Texture GObject::createTexture(string file)
{
	gli::texture tex = gli::load(file);
	if (tex.empty())
		throw GException{ "Texture is empty."+file+"\n" };

#if (DEBUG_LVL>=1)
	cout << "Texture SRGBA"<<"("<< file<<"):[" << gli::is_srgb(tex.format()) <<"]"<< endl;
#endif

	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::gl::format const Format = GL.translate(tex.format(), tex.swizzles());
	GLenum Target = GL.translate(tex.target());

	GLuint TextureName;
	glGenTextures(1, &TextureName);
	glBindTexture(Target, TextureName);
	
#if (DEBUG_LVL>=1)
	cout<<"Texture SRGBA:"<<gli::is_srgb(tex.format())<<"||"<< tex.format() << endl;
#endif


	glm::tvec3<GLsizei> const Extent(tex.extent());
	GLsizei const FaceTotal = static_cast<GLsizei>(tex.layers() * tex.faces());

	switch (tex.target())
	{
	case gli::TARGET_1D:
		glTexStorage1D(
			Target, static_cast<GLint>(tex.levels()), Format.Internal, Extent.x);
		break;
	case gli::TARGET_1D_ARRAY:
	case gli::TARGET_2D:
	case gli::TARGET_CUBE:
		glTexStorage2D(
			Target, static_cast<GLint>(tex.levels()), Format.Internal,
			Extent.x, tex.target() == gli::TARGET_2D ? Extent.y : FaceTotal);
		break;
	case gli::TARGET_2D_ARRAY:
	case gli::TARGET_3D:
	case gli::TARGET_CUBE_ARRAY:
		glTexStorage3D(
			Target, static_cast<GLint>(tex.levels()), Format.Internal,
			Extent.x, Extent.y,
			tex.target() == gli::TARGET_3D ? Extent.z : FaceTotal);
		break;
	default:
		assert(0);
		break;
	}

	for (std::size_t Layer = 0; Layer < tex.layers(); ++Layer)
		for (std::size_t Face = 0; Face < tex.faces(); ++Face)
			for (std::size_t Level = 0; Level < tex.levels(); ++Level)
			{
				GLsizei const LayerGL = static_cast<GLsizei>(Layer);
				glm::tvec3<GLsizei> Extent(tex.extent(Level));
				Target = gli::is_target_cube(tex.target())
					? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
					: Target;

				switch (tex.target())
				{
				case gli::TARGET_1D:
					if (gli::is_compressed(tex.format()))
						glCompressedTexSubImage1D(
							Target, static_cast<GLint>(Level), 0, Extent.x,
							Format.Internal, static_cast<GLsizei>(tex.size(Level)),
							tex.data(Layer, Face, Level));
					else
						glTexSubImage1D(
							Target, static_cast<GLint>(Level), 0, Extent.x,
							Format.External, Format.Type,
							tex.data(Layer, Face, Level));
					break;
				case gli::TARGET_1D_ARRAY:
				case gli::TARGET_2D:
				case gli::TARGET_CUBE:
					if (gli::is_compressed(tex.format()))
						glCompressedTexSubImage2D(
							Target, static_cast<GLint>(Level),
							0, 0,
							Extent.x,
							tex.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
							Format.Internal, static_cast<GLsizei>(tex.size(Level)),
							tex.data(Layer, Face, Level));
					else
						glTexSubImage2D(
							Target, static_cast<GLint>(Level),
							0, 0,
							Extent.x,
							tex.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
							Format.External, Format.Type,
							tex.data(Layer, Face, Level));
					break;
				case gli::TARGET_2D_ARRAY:
				case gli::TARGET_3D:
				case gli::TARGET_CUBE_ARRAY:
					if (gli::is_compressed(tex.format()))
						glCompressedTexSubImage3D(
							Target, static_cast<GLint>(Level),
							0, 0, 0,
							Extent.x, Extent.y,
							tex.target() == gli::TARGET_3D ? Extent.z : LayerGL,
							Format.Internal, static_cast<GLsizei>(tex.size(Level)),
							tex.data(Layer, Face, Level));
					else
						glTexSubImage3D(
							Target, static_cast<GLint>(Level),
							0, 0, 0,
							Extent.x, Extent.y,
							tex.target() == gli::TARGET_3D ? Extent.z : LayerGL,
							Format.External, Format.Type,
							tex.data(Layer, Face, Level));
					break;
				default: assert(0); break;
				}
			}
	
	glBindTexture(Target, 0);

#ifdef _DEBUG
	throwError("GObject::createTexture():\n");
#endif

	return Texture{ Target, TextureName };

}

std::string GObject::returnErrorString(GLuint err)
{

	switch (err) {
	case GL_INVALID_ENUM:
		return string{ "ERROR:GL_INVALID_ENUM\n" };
	case GL_INVALID_VALUE:
		return string{ "ERROR:GL_INVALID_VALUE\n" };
	case GL_INVALID_OPERATION:

		return string{ "ERROR:GL_INVALID_OPERATION\n" };
	case GL_STACK_OVERFLOW:

		return string{ "ERROR:GL_STACK_OVERFLOW\n" };
	case GL_STACK_UNDERFLOW:

		return string{ "ERROR:GL_STACK_UNDERFLOW\n" };
	case GL_OUT_OF_MEMORY:

		return string{ "ERROR:GL_OUT_OF_MEMORY\n" };
	case GL_INVALID_FRAMEBUFFER_OPERATION:

		return string{ "ERROR:GL_INVALID_FRAMEBUFFER_OPERATION\n" };
	case GL_CONTEXT_LOST:
		
		return string{ "ERROR:GL_CONTEXT_LOST\n" };
	default:
		return string{ "ERROR:UNKOWN " } +to_string(err) + "\n";
	};

}


void GObject::throwError(string s)
{
	GLuint err = glGetError();
	stringstream stream;	stream << s;
	if (err != GL_NO_ERROR ) {

	 do{
		stream << returnErrorString(err);
		err = glGetError();
	 } while (err != GL_NO_ERROR );

		throw GException{ string{ std::istreambuf_iterator<char>(stream) , std::istreambuf_iterator<char>() } };
	}
	
}

void GObject::throwError()
{
	GLuint err = glGetError();
	stringstream stream;
	if (err != GL_NONE) {

		do {
			stream << returnErrorString(err);
			err = glGetError();
		} while (err != GL_NONE);

		throw GException{ stream.str() };
	}
}

void GObject::initialize( const vector<shared_ptr<PointLight>>* p_lights, const vector<shared_ptr<DirecLight>>* d_lights, const vector<shared_ptr<SpotLight>>* s_lights)
{
	GProgram[(int)RenderMode::WIREFRAME] = createProgram("shaders\\gobject\\basic.vert.glsl", "shaders\\gobject\\basic.frag.glsl");
	GProgram[(int)RenderMode::BASIC_TEXTURE] = createProgram("shaders\\gobject\\basictexture.vert.glsl", "shaders\\gobject\\basictexture.frag.glsl");

	GProgram[(int)RenderMode::SHADOW_CALC] = createProgram("shaders\\gobject\\onlydepth.vert.glsl", "shaders\\gobject\\onlydepth.frag.glsl");

	glGenBuffers(UNIFORM_BUFFER_SIZE, GObject::uniform_buffers);

	glBindBuffer(GL_UNIFORM_BUFFER, GObject::uniform_buffers[DIRECTLIGHT]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(DirecLightData) * 5, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, GObject::uniform_buffers[DIRECTLIGHT]);

	glBindBuffer(GL_UNIFORM_BUFFER, GObject::uniform_buffers[POINTLIGHT]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLightData) * 10, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, GObject::uniform_buffers[POINTLIGHT]);

	glBindBuffer(GL_UNIFORM_BUFFER, GObject::uniform_buffers[SPOTLIGHT]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SpotLightData) * 10, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, GObject::uniform_buffers[SPOTLIGHT]);

	GObject::point_lights = p_lights;
	GObject::direc_lights = d_lights;
	GObject::spot_lights = s_lights;

	memset((GLuint *)nums_uniform_buffers, 0, sizeof(nums_uniform_buffers));

	renderMode = RenderMode::DEFAULT;


	sunShadowMap = 0;
	nextAvaibleTextureUnit = 1;
}

void GObject::setLights()
{
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffers[DIRECTLIGHT]);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(DirecLightData) * 5, NULL, GL_DYNAMIC_DRAW);
	nums_uniform_buffers[0] = 0;
	for (auto light : *GObject::direc_lights)
	{

		if (light->isActive())
		{
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(DirecLightData)*nums_uniform_buffers[DIRECTLIGHT], sizeof(DirecLightData), &light->getDirecLight());
			nums_uniform_buffers[0]++;
		}
	}


	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffers[POINTLIGHT]);
	nums_uniform_buffers[1] = 0;
	for (auto light : *GObject::point_lights)
	{
		if (light->isActive())
		{
			//cout << numLights[0] << "Alignof DirectLight:" << alignof(PointLightData) << ", Sizeof:" << sizeof(PointLightData) << endl;
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(PointLightData)*nums_uniform_buffers[POINTLIGHT], sizeof(PointLightData), &light->getPointLight());
			nums_uniform_buffers[1]++;
		}
	}

	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffers[SPOTLIGHT]);
	nums_uniform_buffers[2] = 0;
	for (auto light : *GObject::spot_lights)
	{

		if (light->isActive())
		{
			//cout << numLights[2] << "Alignof SpotLightData:" << alignof(SpotLightData) << ", Sizeof:" << sizeof(SpotLightData) << endl;
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(SpotLightData)*nums_uniform_buffers[SPOTLIGHT], sizeof(SpotLightData), &light->getSpotLight());
			nums_uniform_buffers[2]++;
		}
	}

	throwError("GObject::setLights():\n");

}


void GObject::setRenderMode(RenderMode mode)
{
	GObject::renderMode = mode;
}

void GObject::setLightViewMatrix(const glm::mat4 & view, const glm::mat4 & proj)
{
	GObject::lightView = view;
	GObject::lightViewProj = proj*view;
}

unsigned int GObject::getAvaibleTextureUnit()
{
	return nextAvaibleTextureUnit;
}




void GObject::readObjectFile(string file, vector<glm::vec3>& points)
{
#if (DEBUG_LVL>=2)
	int i = 0;
#endif
	ifstream in{ file };
	if (!in.is_open())
		throw GException("File can not open for ReadObject.\n");

	vector<glm::vec3> v;

	string cmd;
	in >> cmd;
	while (!in.eof()) {
		char dlim[6];	GLfloat p[3]; GLuint pi[9];

#if (DEBUG_LVL>=2)
		cerr << "in while -> cmd:" << cmd << endl;
#endif

		if (cmd == "v") {
			in >> p[0] >> p[1] >> p[2];
			glm::vec3 vec{ p[0],p[1],p[2] };
			v.push_back(vec);

#if (DEBUG_LVL>=2)
			cerr << "v:" << endl;
			cerr << p[0] << "," << p[1] << "," << p[2] << endl;
#endif

		}
		else if (cmd == "f") {
			in >> pi[0] >> pi[1] >> pi[2];


#if (DEBUG_LVL>=2)
			cerr << "f:" << endl;
			cerr << pi[0] << "," << pi[1] << "," << pi[2] << endl
				<< pi[3] << "," << pi[4] << "," << pi[5] << endl
				<< pi[6] << "," << pi[7] << "," << pi[8] << endl;

			cerr << "a:" << a << endl;
#endif

			glm::vec3 vecp1{ v[pi[0] - 1] };
			glm::vec3 vecp2{ v[pi[1] - 1] };
			glm::vec3 vecp3{ v[pi[2] - 1] };

			points.push_back(vecp1);			points.push_back(vecp2);			points.push_back(vecp3);

#if (DEBUG_LVL>=2)
			cout << points[i].x << "," << points[i].y << "," << points[i].z << endl;
			cout << points[i + 1].x << "," << points[i + 1].y << "," << points[i + 1].z << endl;
			cout << points[i + 2].x << "," << points[i + 2].y << "," << points[i + 2].z << endl;
			i += 3;
#endif


		}
		in.ignore(1024, '\n');
		if(in.bad())
			throw GException("Something when wrong reading a file.\n");

		in >> cmd;
	}
	cout << "s" << endl;
}

void GObject::readObjectFile(string file, vector<glm::vec3>& points, vector<glm::vec2>& text_cords)
{
#if _DEBUG
	int i = 0;
#endif

	ifstream in{ file };
	if (!in.is_open())
		throw GException("File can not open for ReadObject.\n");

	vector<glm::vec3> v;
	vector<glm::vec2> vt;
	vector<glm::vec3> vn;

	string cmd;
	in >> cmd;
	while (!in.eof()) {
		char dlim[6];	GLfloat p[3]; GLuint pi[9];

#if (DEBUG_LVL>=2)
		cerr << "in while -> cmd:" << cmd << endl;
#endif

		if (cmd == "v") {
			in >> p[0] >> p[1] >> p[2];
			glm::vec3 vec{ p[0],p[1],p[2] };
			v.push_back(vec);

#if (DEBUG_LVL>=2)
			cerr << "v:" << endl;
			cerr << p[0] << "," << p[1] << "," << p[2] << endl;
#endif

		}
		else if (cmd == "vt") {
			in >> p[0] >> p[1];
			glm::vec2 vec{ p[0],p[1] };
			vt.push_back(vec);
#if (DEBUG_LVL>=2)
			cerr << "vt:" << endl;
			cerr << p[0] << "," << p[1] << endl;
#endif
		}
		else if (cmd == "vn") {
			in >> p[0] >> p[1] >> p[2];
			glm::vec3 vec{ p[0],p[1],p[2] };
			vn.push_back(vec);
#if (DEBUG_LVL>=2)
			cerr << "vn:" << endl;
			cerr << p[0] << "," << p[1] << "," << p[2] << endl;
#endif
		}
		else if (cmd == "f") {
			in >> pi[0] >> dlim[0] >> pi[1]
				>> pi[2] >> dlim[1] >> pi[3]
				>> pi[4] >> dlim[2] >> pi[5];
			char a = dlim[0] | dlim[1] | dlim[2];

#if (DEBUG_LVL>=2)
			cerr << "f:" << endl;
			cerr << pi[0] << "," << pi[1] << "," << pi[2] << endl
				<< pi[3] << "," << pi[4] << "," << pi[5] << endl
				<< pi[6] << "," << pi[7] << "," << pi[8] << endl;

			cerr << "a:" << a << endl;
#endif

			if (a != '/')
				throw GException("Seperator is wrong when reading object.\n");

			glm::vec3 vecp1{ v[pi[0] - 1] };
			glm::vec3 vecp2{ v[pi[2] - 1] };
			glm::vec3 vecp3{ v[pi[4] - 1] };

			glm::vec2 vect1{ vt[pi[1] - 1] };
			glm::vec2 vect2{ vt[pi[3] - 1] };
			glm::vec2 vect3{ vt[pi[5] - 1] };



			points.push_back(vecp1);			points.push_back(vecp2);			points.push_back(vecp3);
			text_cords.push_back(vect1);			text_cords.push_back(vect2);			text_cords.push_back(vect3);


#if (DEBUG_LVL>=2)
			std::cout << points[i] << "||" << points[i + 1] << "||" << points[i + 2] << endl;
			i += 3;
#endif

#if (DEBUG_LVL>=2)
			cout << text_cords[i].x << "," << text_cords[i].y << endl;
			cout << text_cords[i + 1].x << "," << text_cords[i + 1].y << endl;
			cout << text_cords[i + 2].x << "," << text_cords[i + 2].y << endl;
			i += 3;
#endif

		}
		in.ignore(1024, '\n');

		in >> cmd;
	}
}


void GObject::readObjectFile(string file,
		vector<glm::vec3> &points, vector<glm::vec2> &text_cords, vector<glm::vec3> &normals )
{
#if _DEBUG
	int i=0;
#endif

	ifstream in{ file };
	if (!in.is_open())
		throw GException("File can not open for ReadObject.\n");

	vector<glm::vec3> v;
	vector<glm::vec2> vt;
	vector<glm::vec3> vn;

	string cmd;
	in >> cmd;
	while (!in.eof()) {
		char dlim[6];	GLfloat p[3]; GLuint pi[9];

#if (DEBUG_LVL>=2)
		cerr << "in while -> cmd:" << cmd << endl;
#endif

		if (cmd == "v") {
			in >> p[0] >> p[1] >> p[2];
			glm::vec3 vec{ p[0],p[1],p[2] };
			v.push_back(vec);

#if (DEBUG_LVL>=2)
			cerr << "v:" << endl;
			cerr << p[0] <<","<< p[1] <<","<< p[2] << endl;
#endif

		}
		else if (cmd == "vt") {
			in >> p[0] >> p[1];
			glm::vec2 vec{ p[0],p[1] };
			vt.push_back(vec);
#if (DEBUG_LVL>=2)
			cerr << "vt:" << endl;
			cerr << p[0] << "," << p[1] << endl;
#endif
		}
		else if (cmd == "vn") {
			in >> p[0] >> p[1] >> p[2];
			glm::vec3 vec{ p[0],p[1],p[2] };
			vn.push_back(vec);
#if (DEBUG_LVL>=2)
			cerr << "vn:" << endl;
			cerr << p[0] << "," << p[1] << "," << p[2] << endl;
#endif
		}
		else if (cmd == "f") {
			in >> pi[0] >> dlim[0]>> pi[1] >> dlim[1] >> pi[2]
			>>pi[3]>> dlim[2] >> pi[4]>> dlim[3] >> pi[5]
			>>pi[6]>> dlim[4] >> pi[7]>> dlim[5] >> pi[8];
			char a = dlim[0] | dlim[1] | dlim[2] | dlim[3] | dlim[4] | dlim[5];

#if (DEBUG_LVL>=2)
			cerr << "f:" << endl;
			cerr << pi[0] << "," << pi[1] << "," << pi[2] <<endl
				 << pi[3] << "," << pi[4] << "," << pi[5] <<endl
				 << pi[6] << "," << pi[7] << "," << pi[8] << endl;

			cerr << "a:" << a << endl;
#endif

			if (a != '/')
				throw GException("Seperator is wrong when reading object.\n");

			glm::vec3 vecp1{ v[pi[0] - 1]	};
			glm::vec3 vecp2{ v[pi[3] - 1]	};
			glm::vec3 vecp3{ v[pi[6] - 1]	};

			glm::vec2 vect1{ vt[pi[1] - 1]	};
			glm::vec2 vect2{ vt[pi[4] - 1]	};
			glm::vec2 vect3{ vt[pi[7] - 1]	};

			glm::vec3 vecn1{ vn[pi[2] - 1]	};
			glm::vec3 vecn2{ vn[pi[5] - 1]	};
			glm::vec3 vecn3{ vn[pi[8] - 1]	};


			points.push_back(vecp1);			points.push_back(vecp2);			points.push_back(vecp3);
			text_cords.push_back(vect1);			text_cords.push_back(vect2);			text_cords.push_back(vect3);
			normals.push_back(vecn1);			normals.push_back(vecn2);			normals.push_back(vecn3);


#if (DEBUG_LVL>=2)
			std::cout << points[i] << "||" << points[i+1] << "||" << points[i+2] << endl;
			i+=3;
#endif

#if (DEBUG_LVL>=2)
			cout << text_cords[i].x << "," << text_cords[i].y	<< endl;
			cout << text_cords[i + 1].x << "," << text_cords[i + 1].y << endl;
			cout << text_cords[i + 2].x << "," << text_cords[i + 2].y << endl;
			i += 3;
#endif

		}
		in.ignore(1024, '\n');

		in >> cmd;
	}

}

void GObject::readObjectFile(string file, vector<glm::vec3>& points, vector<glm::vec2>& text_cords, 
	vector<glm::vec3>& normals, Mesh & faces, vector<glm::vec3>& v, vector<glm::vec2>& vt, 
	vector<glm::vec3>& vn, glm::vec3 displacement, GLfloat scale)
{
#if (DEBUG_LVL>=2)
	int i = 0;
#endif
	ifstream in{ file };
	if (!in.is_open())
		throw GException("File can not open for ReadObject.\n");

	string cmd;
	in >> cmd;
	while (!in.eof()) {
		char dlim[6];	GLfloat p[3]; GLuint pi[9];

#if (DEBUG_LVL>=2)
		cerr << "in while -> cmd:" << cmd << endl;
#endif

		if (cmd == "v") {
			in >> p[0] >> p[1] >> p[2];
			glm::vec3 vec{ p[0],p[1],p[2] };
			vec = (vec + displacement) * scale;
			v.push_back(vec);

#if (DEBUG_LVL>=2)
			cerr << "v:" << endl;
			cerr << p[0] << "," << p[1] << "," << p[2] << endl;
#endif

		}
		else if (cmd == "vt") {
			in >> p[0] >> p[1];
			glm::vec2 vec{ p[0],p[1] };
			vt.push_back(vec);
#if (DEBUG_LVL>=2)
			cerr << "vt:" << endl;
			cerr << p[0] << "," << p[1] << endl;
#endif
		}
		else if (cmd == "vn") {
			in >> p[0] >> p[1] >> p[2];
			glm::vec3 vec{ p[0],p[1],p[2] };
			vn.push_back(vec);
#if (DEBUG_LVL>=2)
			cerr << "vn:" << endl;
			cerr << p[0] << "," << p[1] << "," << p[2] << endl;
#endif
		}
		else if (cmd == "f") {
			in >> pi[0] >> dlim[0] >> pi[1] >> dlim[1] >> pi[2]
				>> pi[3] >> dlim[2] >> pi[4] >> dlim[3] >> pi[5]
				>> pi[6] >> dlim[4] >> pi[7] >> dlim[5] >> pi[8];
			char a = dlim[0] | dlim[1] | dlim[2] | dlim[3] | dlim[4] | dlim[5];

#if (DEBUG_LVL>=2)
			cerr << "f:" << endl;
			cerr << pi[0] << "," << pi[1] << "," << pi[2] << endl
				<< pi[3] << "," << pi[4] << "," << pi[5] << endl
				<< pi[6] << "," << pi[7] << "," << pi[8] << endl;

			cerr << "a:" << a << endl;
#endif

			if (a != '/')
				throw GException("Seperator is wrong when reading object.\n");
			glm::vec3 vecp[3];
			glm::vec3 vecp1{ v[pi[0] - 1] };
			glm::vec3 vecp2{ v[pi[3] - 1] };
			glm::vec3 vecp3{ v[pi[6] - 1] };

			faces.points.emplace_back( pi[0] - 1, pi[3] - 1, pi[6] - 1 );

			glm::vec2 vect1{ vt[pi[1] - 1] };
			glm::vec2 vect2{ vt[pi[4] - 1] };
			glm::vec2 vect3{ vt[pi[7] - 1] };

			faces.tex_cords.emplace_back( pi[1] - 1, pi[4] - 1, pi[7] - 1);

			glm::vec3 vecn1{ vn[pi[2] - 1] };
			glm::vec3 vecn2{ vn[pi[5] - 1] };
			glm::vec3 vecn3{ vn[pi[8] - 1] };

			faces.normals.emplace_back( pi[2] - 1, pi[5] - 1, pi[8] - 1 );


			points.push_back(vecp1);			points.push_back(vecp2);			points.push_back(vecp3);
			text_cords.push_back(vect1);			text_cords.push_back(vect2);			text_cords.push_back(vect3);
			normals.push_back(vecn1);			normals.push_back(vecn2);			normals.push_back(vecn3);


#if (DEBUG_LVL>=2)
			cout << points[i].x << "," << points[i].y << "," << points[i].z << endl;
			cout << points[i + 1].x << "," << points[i + 1].y << "," << points[i + 1].z << endl;
			cout << points[i + 2].x << "," << points[i + 2].y << "," << points[i + 2].z << endl;
			i += 3;
#endif
#if (DEBUG_LVL>=2)
			cout << text_cords[i].x << "," << text_cords[i].y << endl;
			cout << text_cords[i + 1].x << "," << text_cords[i + 1].y << endl;
			cout << text_cords[i + 2].x << "," << text_cords[i + 2].y << endl;
			i += 3;
#endif

		}
		in.ignore(1024, '\n');

		in >> cmd;
	}
}



GObject::~GObject(){

}
