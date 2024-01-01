#include "CzeViewport.hpp"
#include "CzeButton.hpp"
#include "CzeTextbox.hpp"
#include <QBoxLayout>
#include <QOpenGLFunctions>
#include <qopenglextrafunctions.h>
#include <qopenglwidget.h>
#include <qmatrix4x4.h>
#include "global.hpp"

class CzeViewportOpenGL : public QOpenGLWidget
{
public:
	CzeViewportOpenGL(QWidget* parent = nullptr) : QOpenGLWidget(parent)
	{
		//QSurfaceFormat f;
		//f.setVersion(4, 6);
		//f.setProfile(QSurfaceFormat::CoreProfile);
		//setFormat(f);
	}

	~CzeViewportOpenGL()
	{
		QOpenGLExtraFunctions extra(context());
		extra.glDeleteBuffers(1, &vbo);
		extra.glDeleteVertexArrays(1, &vao);
		extra.glDeleteTextures(1, &texture);

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
		glBindTexture(GL_TEXTURE_2D, texture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
		glBindTexture(GL_TEXTURE_2D, 0);
		vertshader = extra.glCreateShader(GL_VERTEX_SHADER);
		extra.glShaderSource(vertshader, 1, &vertshadersource, 0);
		extra.glCompileShader(vertshader);

		fragshader = extra.glCreateShader(GL_FRAGMENT_SHADER);
		extra.glShaderSource(fragshader, 1, &fragshadersource, 0);
		extra.glCompileShader(fragshader);
		//char IDK[512];
		//int retlen;
		//extra.glGetShaderInfoLog(fragshader, 512, &retlen, IDK);

		program = extra.glCreateProgram();
		extra.glAttachShader(program, vertshader);
		extra.glAttachShader(program, fragshader);
		extra.glLinkProgram(program);
		//char IDK[512];
		//int retlen;
		//extra.glGetProgramInfoLog(program, 512, &retlen, IDK);

	}

	void paintGL()
	{
		QOpenGLExtraFunctions extra(context());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.5, 1.0);
		glLoadIdentity();

		extra.glBindBuffer(GL_ARRAY_BUFFER, vbo);
		extra.glBindVertexArray(vao);
		QMatrix4x4 projection;
		projection.perspective(90, ((float)width()) / ((float)height()), 1, 128);
		glViewport(0, 0, width(), height());
		

		if (!uninitializedKeyframes.empty())
		{
			for (auto& keyframe : uninitializedKeyframes)
			{
				InitializeKeyframe(keyframe, extra);
			}
			uninitializedKeyframes.clear();
		}

		if (!updatedKeyframes.empty())
		{
			//qWarning("yeah");
			for (auto& keyframe : updatedKeyframes)
			{
				int width, height;
				keyframe->source->getSize(width, height);
				int newsize = width * height * 4;
				unsigned char* pixels = (unsigned char*)malloc(newsize);
				keyframe->source->getImage(pixels, width, height);
				glBindTexture(GL_TEXTURE_2D, keyframe->texture);
				int maxwidth, maxheight;
				extra.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &maxwidth);
				extra.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &maxheight);
				if (newsize > maxwidth * maxheight * 4)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				}
				else
				{
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				free(pixels);
			}
			updatedKeyframes.clear();
		}

		DoKeyframeShit(extra, projection.data());

		
	}

	GLuint vao;
	GLuint vbo;
	GLuint texture;
	GLubyte texturedata[4] = { 255,128,64,255 };
	GLuint vertshader;
	GLuint fragshader;
	GLuint program;
	const char* vertshadersource = "#version 460 core\n\
		layout (location = 0) in vec3 aPos;\n\
layout (location=1) in vec2 vertexColor; \n\
uniform highp mat4 matrix;\n\
out vec2 fragmentColor;\n\
out vec3 worldPos;\n\
		void main()\n\
		{\n\
		   gl_Position = matrix*vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
		fragmentColor = vertexColor;\n\
		}\0";
	const char* fragshadersource = "#version 460 core \n\
		out vec4 FragColor; \n\
in vec2 fragmentColor;\n\
in vec3 worldPos; \n\
uniform sampler2D image;\n\
	void main() \n\
	{  \n\
\n\
\n\
		FragColor = texture(image,fragmentColor); \n\
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