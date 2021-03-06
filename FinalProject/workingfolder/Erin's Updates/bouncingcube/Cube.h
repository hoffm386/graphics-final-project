#pragma once
#include "ShaderWnd/ShaderWnd.h"
#include "GrTexture.h"

const int NumVertices = 36;
typedef  vec4  point4;typedef  vec4  color4;


class CCube
{
public:
	point4 vertex_positions[8];
	point4  points[NumVertices];
	color4  colors[NumVertices];
	vec3 normals[NumVertices];
	vec2   tex_coords[NumVertices];

	GLuint vao, vbo;

	double m_size;
	mat4 m_translation;
	mat4 m_rotation;
	vec3 c, v, w;
	quat q;
	CGrTexture m_wood;
	double len,hgt;
	vec4 m_dir;
public:
	CCube(double length, double width, double height, bool openWall);
	~CCube(void);

	void quad(int a, int b, int c, int d, point4 *vertex_positions, int& Index);


	void InitGL(GLuint program);
	void RenderGL(GLuint program);

	void Update(double dt);
	void ResetCube();
	void CubeTranslate(double a, double b, double c);

	void AddImpulse(vec3 n, int corner);
	vec4 getBnds();
	bool checkCollide(vec4 bnds,int cnum);
	bool checkPoint();
	void CubeRotate(int dir);
	void CubeMove(int dir);
};

const vec3 operator* (double dt, vec3 v); 
const quat operator+ (quat, quat);
const quat operator* (vec3 v, quat q);
const quat operator* (float s, quat q);

