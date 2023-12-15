#include "CzeViewport.hpp"
#include "CzeButton.hpp"
#include "CzeTextbox.hpp"
#include <QBoxLayout>
#include <QOpenGLFunctions>
#include <qopenglextrafunctions.h>
#include <qopenglwidget.h>
#include <qmatrix4x4.h>
// this is for later

class CzeViewportOpenGL : public QOpenGLWidget
{
public:
	CzeViewportOpenGL(QWidget* parent = nullptr) : QOpenGLWidget(parent)
	{
		
	}

	void initializeGL()
	{
		QOpenGLExtraFunctions extra(context());
		extra.glGenVertexArrays(1, &vao);
		extra.glBindVertexArray(vao);

		extra.glGenBuffers(1, &vbo);
		extra.glBindBuffer(GL_ARRAY_BUFFER, vbo);

		extra.glEnableVertexAttribArray(0);
		extra.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, 0);
		extra.glEnableVertexAttribArray(1);
		extra.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, (void*)12);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LEQUAL);

		glGenTextures(1, &texture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
		vertshader = extra.glCreateShader(GL_VERTEX_SHADER);
		extra.glShaderSource(vertshader, 1, &vertshadersource, 0);
		extra.glCompileShader(vertshader);

		fragshader = extra.glCreateShader(GL_FRAGMENT_SHADER);
		extra.glShaderSource(fragshader, 1, &fragshadersource, 0);
		extra.glCompileShader(fragshader);
		char IDK[512];
		int retlen;
		extra.glGetShaderInfoLog(fragshader, 512, &retlen, IDK);
		qWarning("%s\n",IDK);

		program = extra.glCreateProgram();
		extra.glAttachShader(program, vertshader);
		extra.glAttachShader(program, fragshader);
		extra.glLinkProgram(program);


	}

	void paintGL()
	{
		QOpenGLExtraFunctions extra(context());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.5, 1.0);
		glLoadIdentity();

		extra.glBindBuffer(GL_ARRAY_BUFFER, vbo);
		extra.glBindVertexArray(vao);
		QMatrix4x4* projection = new QMatrix4x4();
		projection->perspective(90, width() / height(), 1, 128);
		static float vertices[3][5] = { {0.0,0.0,0.0, 0.0,0.0}, {1.0,0.0,0.0, 1.0,0.0}, {0.0,1.0,0.0, 0.0,1.0} };
		
		glBindTexture(GL_TEXTURE_2D, texture);
		
		

		extra.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		extra.glUseProgram(program);
		glViewport(0, 0, width(), height());
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	GLuint vao;
	GLuint vbo;
	GLuint texture;
	GLuint texturedata[4] = { 255,128,64,255 };
	GLuint vertshader;
	GLuint fragshader;
	GLuint program;
	const char* vertshadersource = "#version 330 core\n\
		layout (location = 0) in vec3 aPos;\n\
layout (location=1) in vec2 vertexColor; \n\
		void main()\n\
		{\n\
		   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
		}\0";
	const char* fragshadersource = "#version 330 core \n\
		out vec4 FragColor; \n\
 \n\
	void main() \n\
	{  \n\
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n\
	} \0";
};


CzeViewport::CzeViewport(QWidget* parent) : CzeWindow(parent)
{
	SetTitle("Viewport");
	opengl = new CzeViewportOpenGL(this);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(opengl);
	opengl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setLayout(layout);
	
}