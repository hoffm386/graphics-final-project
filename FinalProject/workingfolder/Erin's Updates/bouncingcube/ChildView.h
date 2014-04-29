
// ChildView.h : interface of the CChildView class
//


#pragma once


#include "ShaderWnd/ShaderWnd.h"

#include "Cube.h"
#include "GrTexture.h"
// CChildView window

class CChildView : public CShaderWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	//CCube *m_cube, *m_cube1, *m_cube4, *m_cube2, *m_cube3, *m_cube5, *m_cube6,  *m_cube7,  *m_cube8,  *m_cube9;
	//CCube *m_cube10, *m_cube11, *m_cube14, *m_cube12, *m_cube13, *m_cube15, *m_cube16,  *m_cube17,  *m_cube18,  *m_cube19;
	//CCube *m_cube20, *m_cube21, *m_cube24, *m_cube22, *m_cube23;
	CCube *m_user;
	CCube *cubes[31];
	double *checkpoints[5];
	CCube  *m_wall1, *m_wall2, *m_wall3, *m_wall4, *m_wall5, *m_wall6;
	CCube *m_wall,*m_floor, *m_cp, *m_cp1, *m_cp2;
	bool m_bTimer,camfpv;
	int m_nTimer,m_corner;
	float m_fT;
	vec4 bounds[31];

// Operations
public:

// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


// Implementation
public:
	virtual ~CChildView();
	virtual void InitGL();
	virtual void RenderGL();
	virtual void CleanGL();
	void createMaze();
	void CubeTranslate(double a, double b, double c);
	vec3 GenerateNewLocation();
	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	 
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void ToggleDragSetting();
	void Motion(vec3& vVec);
	void resetCamera(bool camfpv);
private:
	CGrTexture m_wood;
	CGrTexture m_sand;
	CGrTexture m_lego;
	CGrTexture m_forest;
	CGrTexture m_checkpoint;
	bool m_dragSetting;
	double x;
	double y;
	mat4 m_translation;
	double z;

};

