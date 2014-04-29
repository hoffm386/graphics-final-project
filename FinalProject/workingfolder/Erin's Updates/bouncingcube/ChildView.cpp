
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "BouncingCube.h"
#include "ChildView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	x = -12;
	y = 1;
	z = 5.5;
	m_bTimer = false;
	m_nTimer = -1;
	m_fT = 0.f;
	createMaze();
	m_user = new CCube(.5,2,.5, false);
	m_user->CubeTranslate(x, y, z);
	m_lego.LoadFile(L"textures/lego.jpg");
	m_forest.LoadFile(L"textures/forest.jpg");
}

CChildView::~CChildView()
{
	delete m_user,m_wall,m_floor,cubes[30];
}


BEGIN_MESSAGE_MAP(CChildView, CShaderWnd)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CShaderWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}


const int  TextureSize  = 64;

// Texture objects and storage for texture image
GLuint textures[5];
GLubyte image[TextureSize][TextureSize][3];
GLubyte image2[TextureSize][TextureSize][3];
//vec2   tex_coords[NumVertices];
typedef  vec4  point4;typedef  vec4  color4;

void CChildView::InitGL()
{
	// Create a checkerboard pattern
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			GLubyte c;
			c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
			image[i][j][0]  = c;
			image[i][j][1]  = c;
			image[i][j][2]  = c;
		}
	}
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			GLubyte c;
			c = (((i & 0x16) == 0) | ((j & 0x16) == 0)) * 255;
			image2[i][j][0] = c;
			image2[i][j][1] = 0;
			image2[i][j][2] = c;
		}
	}

	m_program = LoadShaders( "ShaderWnd/vertex.glsl" , "ShaderWnd/fragment.glsl" );

	glUseProgram(m_program);

	glGenTextures(4, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_wood.Width(),m_wood.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_wood.ImageBits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_wood.Width(),m_wood.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_wood.ImageBits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_lego.Width(),m_lego.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_lego.ImageBits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_forest.Width(),m_forest.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_forest.ImageBits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_checkpoint.Width(),m_checkpoint.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_checkpoint.ImageBits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, textures[0]);

	for(int i=0;i<30;i++)
	{
		cubes[i]->InitGL(m_program);
	}

	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, textures[1]);
	m_wall->InitGL(m_program);

	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, textures[2]);
	m_user->InitGL(m_program);

	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_2D, textures[3]);
	m_floor->InitGL(m_program);

	glActiveTexture( GL_TEXTURE4 );
	glBindTexture( GL_TEXTURE_2D, textures[4]);
	m_cp->InitGL(m_program);
	m_cp1->InitGL(m_program);
	m_cp2->InitGL(m_program);

	point4 light_position (-5.f, 5.f, -5.f, 0.f);
	color4 light_ambient (0.2f, 0.2f, 0.2f, 1.f);
	color4 light_diffuse (1.f, 1.f, 1.f, 1.f);
	color4 light_specular (1.f, 1.f, 1.f, 1.f);

	color4 material_ambient(.3f, .6f, .3f, 1.f);
	color4 material_diffuse (0.9f, .9f, 0.7f, 1.f);
	color4 material_specular (0.4f, 0.4f, 0.4f, 1.f);
	float material_shininess = 100.0f;

	color4 ambient_product = light_ambient*material_ambient;
	color4 diffuse_product = light_diffuse*material_diffuse;
	color4 specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	glUniform4fv(glGetUniformLocation(m_program, "DiffuseProduct"), 1, value_ptr(diffuse_product));
	glUniform4fv(glGetUniformLocation(m_program, "SpecularProduct"), 1, value_ptr(specular_product));
	glUniform4fv(glGetUniformLocation(m_program, "LightPosition"), 1, value_ptr(light_position));
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), material_shininess);


	glUniform1i( glGetUniformLocation(m_program, "diffuse_mat"), 0);


	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glClearColor(1.f,1.f,1.f,1.f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CChildView::RenderGL()
{

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(m_mPVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(m_mVM));

	glUniform1f(glGetUniformLocation(m_program,"t"), m_fT);

	color4 light_ambient (0.4f, 0.4f, 0.4f, 1.f);
	color4 material_ambient(.6f, .6f, .6f, 1.f);
	color4 material_transpartent(.3f, .3f, .3f, 1.f);

	color4 ambient_product = light_ambient*material_ambient;

	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	glUniform1i( glGetUniformLocation(m_program, "diffuse_mat"), 0);

	for(int i=0;i<24;i++)
	{
		cubes[i]->RenderGL(m_program);
	}
	

	ambient_product = light_ambient*material_transpartent;
	glUniform1i( glGetUniformLocation(m_program, "diffuse_mat"), 1);
	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	m_wall->RenderGL(m_program);
	for(int i=24;i<30;i++)
	{
		cubes[i]->RenderGL(m_program);
	}

	glUniform1i( glGetUniformLocation(m_program, "diffuse_mat"), 2);
	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 2, value_ptr(ambient_product));
	m_user->RenderGL(m_program);
	
	glUniform1i( glGetUniformLocation(m_program, "diffuse_mat"), 3);
	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 3, value_ptr(ambient_product));
	m_floor->RenderGL(m_program);

	glUniform1i( glGetUniformLocation(m_program, "diffuse_mat"), 4);
	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 4, value_ptr(ambient_product));
	m_cp->RenderGL(m_program);
	m_cp1->RenderGL(m_program);
	m_cp2->RenderGL(m_program);
}

void CChildView::CleanGL()
{

}

vec3 CChildView::GenerateNewLocation()
{
	double loc = 0;
	loc = rand()%5;
	loc = floor(loc);
	if (loc == 0)
	{
		x=-13, z=13;
		return vec3(x,1,z);
	}
	else if (loc == 1)
	{
		x=0, z=0;
		return vec3(x,1,z);
	}
	else if (loc == 2)
	{
		x=-13, z=-13;
		return vec3(x,1,z);
	}
	else if (loc == 3)
	{
		x=13.5, z=13.5;
		return vec3(x,1,z);
	}
	else if (loc == 4)
	{
		x=13, z=-13;
		return vec3(x,1,z);
	}
	else
	{
		x=-12, z=5.5;
		return vec3(x,1,z);
	}
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	bool goodMove=true;
	bool checkPoint = true;
	double loc = 0;
	switch (nChar) {
	case 'r':
	case 'R':
		x=-12;
		z=5.5;
		ResetMatrix();
		m_user->ResetCube();
		m_user->CubeTranslate(x,1,z);
		Invalidate();
		break;
	case VK_SPACE:
		if (m_nTimer==-1) {
		m_nTimer = SetTimer(1, 40, NULL);
		} else {
		KillTimer(m_nTimer);
		m_nTimer = -1;
		}
		break;
	case VK_RIGHT:
		m_user->CubeRotate(1);
		if (m_user->checkPoint())
		{
			exit(0);
			vec3 trans = GenerateNewLocation();
			m_user->CubeTranslate(trans[0], trans[1], trans[2]);
		}
		Invalidate();
		for(int i=0;i<31;i++)
		{
			if(goodMove)
			{
				goodMove=!(m_user->checkCollide(bounds[i],i));
			}
		}
		if(!goodMove){
			m_user->CubeRotate(-1);
			Invalidate();
		}
		break;
	case VK_LEFT:
		m_user->CubeRotate(-1);
		if (m_user->checkPoint())
		{
			vec3 trans = GenerateNewLocation();
			m_user->CubeTranslate(trans[0], trans[1], trans[2]);
		}
		Invalidate();
		for(int i=0;i<31;i++)
		{
			if(goodMove)
			{
				goodMove=!(m_user->checkCollide(bounds[i],i));
			}
		}
		if(!goodMove)
		{
		m_user->CubeRotate(1);
		Invalidate();
		}

		break;
	case VK_UP:
		m_user->CubeMove(1);
		if (m_user->checkPoint())
		{
			vec3 trans = GenerateNewLocation();
			m_user->CubeTranslate(trans[0], trans[1], trans[2]);
		}
		Invalidate();
		for(int i=0;i<31;i++)
		{
			if(goodMove)
			{
				goodMove=!(m_user->checkCollide(bounds[i],i));
			}
		}
		if(!goodMove)
		{
		m_user->CubeMove(-1);
		Invalidate();
		}

		break;
	case VK_DOWN:
		m_user->CubeMove(-1);
		if (m_user->checkPoint())
		{
			vec3 trans = GenerateNewLocation();
			m_user->CubeTranslate(trans[0], trans[1], trans[2]);
		}
		Invalidate();
		for(int i=0;i<31;i++)
		{
			if(goodMove)
			{
				goodMove=!(m_user->checkCollide(bounds[i],i));
			}
		}
		if(!goodMove)
		{
			m_user->CubeMove(1);
			Invalidate();
		}

		break;
	case '0':
		m_corner=0;
		break;
	case '1':
		m_corner=1;
		break;
	case '2':
		m_corner=2;
		break;
	case '3':
		m_corner=3;
		break;
	case '4':
		m_corner=4;
		break;
	case '5':
		m_corner=5;
		break;
	case '6':
		m_corner=6;
		break;
	case '7':
		m_corner=7;
		break;
	}

	CShaderWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	Invalidate();

	CShaderWnd::OnTimer(nIDEvent);
}

void CChildView::ToggleDragSetting()
{
	m_dragSetting = !m_dragSetting;
}

void CChildView::Motion(vec3& vVec)
{
	if (!m_bDragging)
		return;

	if (m_dragSetting)
	{
		CShaderWnd::Motion(vVec);
	}
	else
	{
		m_user->AddImpulse(normalize(vVec),m_corner);
	}
}

void CChildView::createMaze()
{
	cubes[0] = new CCube(5,3,.3, false);
	cubes[0]->CubeTranslate(-6,0,-7);
	cubes[1] = new CCube(.3,3,2, false);
	cubes[1]->CubeTranslate(-10.7,0,-9);
	cubes[2] = new CCube(.3,3,2, false);
	cubes[2]->CubeTranslate(-1.3,0,-9);
	cubes[3] = new CCube(.3,3,4, false);
	cubes[3]->CubeTranslate(-6,0,-11);
	cubes[4] = new CCube(1,3,.3, false);
	cubes[4]->CubeTranslate(-2.5,0,-10.7);
	cubes[5] = new CCube(.3,3,2, false);
	cubes[5]->CubeTranslate(4,0,-13);
	cubes[6] = new CCube(3.5,3,.3, false);
	cubes[6]->CubeTranslate(7.2,0,-11);
	cubes[7] = new CCube(.3,3,2, false);
	cubes[7]->CubeTranslate(10.4,0,-8.7);
	cubes[8] = new CCube(1.5,3,.3, false);
	cubes[8]->CubeTranslate(8.7,0,-7);
	cubes[9] = new CCube(.3,3,8, false);
	cubes[9]->CubeTranslate(4,0,.7);
	cubes[10] = new CCube(4,3,.3, false);
	cubes[10]->CubeTranslate(7.7,0,8.7);
	cubes[11] = new CCube(3.8,3,.3, false);
	cubes[11]->CubeTranslate(11.2,0,-2.5);
	cubes[12] = new CCube(.3,3,4, false);
	cubes[12]->CubeTranslate(7.7,0,1.2);
	cubes[13] = new CCube(.3,3,4, false);
	cubes[13]->CubeTranslate(12,0,5);
	cubes[14] = new CCube(1.5,3,.3, false);
	cubes[14]->CubeTranslate(13.5,0,1.3);
	cubes[15] = new CCube(4,3,.3, false);
	cubes[15]->CubeTranslate(8.2,0,12);
	cubes[16] = new CCube(4,3,.3, false);
	cubes[16]->CubeTranslate(.2,0,-2.2);
	cubes[17] = new CCube(.3,3,7, false);
	cubes[17]->CubeTranslate(-3.5,0,4.5);
	cubes[18] = new CCube(.3,3,6, false);
	cubes[18]->CubeTranslate(0,0,9);
	cubes[19] = new CCube(3.5,3,.3, false);
	cubes[19]->CubeTranslate(-6.7,0,11.5);
	cubes[20] = new CCube(3.5,3,.3, false);
	cubes[20]->CubeTranslate(-6.7,0,3);
	cubes[21] = new CCube(1.5,3,.3, false);
	cubes[21]->CubeTranslate(-8.3,0,-2.4);
	cubes[22] = new CCube(.3,3,3, false);
	cubes[22]->CubeTranslate(-10,0,.3);
	cubes[23] = new CCube(4,3,.3, false);
	cubes[23]->CubeTranslate(-11,0,7);
	cubes[24] =  new CCube(15,3,.3, false);
	cubes[24]->CubeTranslate(0,0,15.3);
	cubes[25] =  new CCube(15,3,.3, false);
	cubes[25]->CubeTranslate(0,0,-15.3);
	cubes[26] =  new CCube(.3,3,4.45, false);
	cubes[26]->CubeTranslate(-15.3,0,11.15);
	cubes[27] =  new CCube(.3,3,9.3, false);
	cubes[27]->CubeTranslate(-15.3,0,-6.3);
	cubes[28] =  new CCube(.3,3,6.7, false);
	cubes[28]->CubeTranslate(15.3,0,-8.9);
	cubes[29] =  new CCube(.3,3,7.3, false);
	cubes[29]->CubeTranslate(15.3,0,8.3);

	m_floor = new CCube(15,0,15, false);
	m_floor->CubeTranslate(0,-2,1);
	m_wall=new CCube(20,3,20,true);
	for(int i=0;i<30;i++)
	{
		bounds[i]= (cubes[i]->getBnds());

	}
	m_cp = new CCube(1.5,.1,1.5, false);
	m_cp->CubeTranslate(13.5,-2,3);
	m_cp1 = new CCube(1.5,.1,1.5, false);
	m_cp1->CubeTranslate(1.5,-2,-13.5);
	m_cp2 = new CCube(1.5,.1,1.5, false);
	m_cp2->CubeTranslate(1.5,-2, 13.5);

	m_wood.LoadFile(L"textures/brick.jpg");
	m_sand.LoadFile(L"textures/sand.jpg");
	m_checkpoint.LoadFile(L"textures/cp.jpg");


}