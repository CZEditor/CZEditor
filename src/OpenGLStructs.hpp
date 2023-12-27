#include <qopenglfunctions.h>
#include <qopenglextrafunctions.h>
#include <QString>

struct VertexShader
{
	int shader;
	QString declaration;
	QString call;
};

enum FRAGMENT_SHADER_TYPE
{
	FST_Position,
	FST_Color
};

struct FragmentShader
{
	int shader;
	QString declaration;
	QString call;
	FRAGMENT_SHADER_TYPE type;
};