#include "global.hpp"
#include <time.h>
#include "Properties.hpp"
static int todayshue;
QFont defaultfont;
int currentframe = 0;
KeyframeList keyframelist;
std::list<Keyframe*> uninitializedKeyframes;
std::list<Keyframe*> updatedKeyframes;

ICZEditor* cze = 0;

void InitGlobals()
{
	todayshue = (time(nullptr)/(60*60*24) * 223) % 360;
	defaultfont = QFont("Segoe UI", 9);
}

QColor GetAccentColor(int saturation, int value, int alpha)
{
	QColor ret(0, 0, 0);
	ret.setHsv(todayshue, saturation, value, alpha);
	return ret;
}

QColor InterpolateQColor(QColor colA, QColor colB, float i)
{
	QRgb B = colB.rgba();
	QRgb A = colA.rgba();
	int interpolation = i * 255;
	int interpolationM = 255 - interpolation;
	return QColor((qRed(B)  * interpolation + qRed(A) * interpolationM) / 255, (qGreen(B) * interpolation + qGreen(A) * interpolationM) / 255, (qBlue(B) * interpolation + qBlue(A) * interpolationM) / 255, (qAlpha(B) * interpolation + qAlpha(A) * interpolationM) / 255);
}


#include "Keyframes.hpp"
void DoKeyframeShit(QOpenGLExtraFunctions extra, GLfloat* projectiondata)
{
	KeyframeList klist;
	for (auto& keyframe : keyframelist.keyframes)
	{
		for (auto& action : keyframe->actions)
		{
			action->actOnKeyframes(&klist, keyframe);
		}
	}
	
	for (auto& keyframe : klist.keyframes)
	{
		std::vector<float> vertices;
		//std::list<FragmentShader> fraglist;
		for (auto& effect : keyframe->effects)
		{
			effect->effectVertices(vertices);
			//effect->addFragmentShaders(fraglist);
		}
		if (vertices.size() == 0)
			continue;
		extra.glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, keyframe->texture);
		extra.glUniformMatrix4fv(extra.glGetUniformLocation(keyframe->programs.back(), "matrix"), 1, GL_FALSE, projectiondata);
		extra.glUniform1i(extra.glGetUniformLocation(keyframe->programs.back(), "image"), 0);
		extra.glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4, vertices.data(), GL_DYNAMIC_DRAW);
		extra.glUseProgram(keyframe->programs.back());
		glDrawArrays(GL_TRIANGLES, 0, (int)(vertices.size() / 5));

		
	}
}
static const char* vertshadersource = "#version 460 core\n\
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
static const char* fragshadersource = "#version 460 core \n\
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
void InitializeKeyframe(Keyframe* keyframe, QOpenGLExtraFunctions extra)
{
	std::list<FragmentShader> fraglist;
	keyframe->programs.push_back(extra.glCreateProgram());
	GLuint vertshader = extra.glCreateShader(GL_VERTEX_SHADER);
	extra.glShaderSource(vertshader, 1, &vertshadersource, 0);
	extra.glCompileShader(vertshader);
	glGenTextures(1, &keyframe->texture);
	glBindTexture(GL_TEXTURE_2D, keyframe->texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	GLubyte texturedata[4] = { 255,128,64,255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
	glBindTexture(GL_TEXTURE_2D, 0);
	char IDK[512];
	int retlen;
	for (auto& effect : keyframe->effects)
	{
		effect->initializeShaders(extra);
		effect->addFragmentShaders(fraglist);
		for (auto& fragmentshader : fraglist)
		{
			extra.glAttachShader(keyframe->programs.back(), fragmentshader.shader);
			if (fragmentshader.type == FST_Color)
			{
				
				QString main;
				main.append("#version 460 core\n\
out vec4 FragColor;\n\
in vec2 fragmentColor;\n\
in vec3 worldPos;\n\
uniform sampler2D image;\n");
				for (auto& fragmentshaderdec : fraglist)
				{
					main.append(fragmentshaderdec.declaration);
					main.append("\n");
				}
				main.append("void main()\n\
				{\n\
					vec2 pos;\n\
					pos = fragmentColor;\n");
				for (auto& fragmentshadercall : fraglist)
				{
					if (fragmentshadercall.type == FST_Position)
					{
						main.append("pos = ");
						main.append(fragmentshadercall.call);
					}
					else
					{
						main.append("FragColor = ");
						main.append(fragmentshadercall.call);
					}
					main.append("\n");
				}
				main.append("}");
				int a = keyframe->programs.back();
				QByteArray ba = main.toLocal8Bit();
				const char* shadersrc = ba.data();
				GLuint finalshader = extra.glCreateShader(GL_FRAGMENT_SHADER);
				extra.glShaderSource(finalshader, 1, &shadersrc, 0);
				extra.glCompileShader(finalshader);
				extra.glAttachShader(keyframe->programs.back(), finalshader);
				extra.glAttachShader(keyframe->programs.back(), vertshader);
				extra.glLinkProgram(a);
				extra.glGetShaderInfoLog(finalshader, 512, &retlen, IDK);
			}
		}
	}
	
}

void UpdateKeyframeTexture(Keyframe* keyframe)
{
	updatedKeyframes.push_back(keyframe);
}