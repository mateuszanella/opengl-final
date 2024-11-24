#pragma once
#include "SceneBaseClass.h"
#include "CTexture.h"
#include "CTimer.h"
#include "CTexto.h"
#include "CCamera.h"
#include "CModel_3DS.h"

class CScene2 : public CSceneBaseClass
{
public:
	CScene2();
	~CScene2(void);

	virtual void MouseMove(void);					// Tratamento de movimento do mouse
	virtual void KeyPressed(void);					// Tratamento de teclas pressionadas
	virtual void KeyDownPressed(WPARAM	wParam);	// Tratamento de teclas pressionadas
	virtual int DrawGLScene(void);					// Função que desenha a cena

	void Draw3DSGrid(float width, float length);
	void DrawAxis();

	void DrawFloor();
	void DrawHouse(float X, float Y, float Rotation);
	void DrawTree(float X, float Y, float Rotation);
	void DrawForest();
	void DrawBox(float X, float Y, float Z, float Size, float Rotation);

	void SetFog(float fDensity, float fStart, float fEnd);
	void UnsetFog();

	void SetupAmbientLight();
	void SetupHouse1Light();
	void SetupHouse2Light();

	void CreateSkyBox(float x, float y, float z,
		float width, float height, float length,
		CTexture* pTextures);
private:

	bool	keys[256];		// Array usado para rotinas do teclado
	bool	active;			// Window Active Flag Set To TRUE By Default
	bool	fullscreen;		// Exibir janela em modo fullscreem (TRUE) ou em janela (FALSE)


	CCamera* pCamera;		// Gerencia câmera OpenGL
	float fDeltaY;			// Rotação da câmera OpenGL no eixo Y
	float fDeltaX;			// Rotação da câmera OpenGL no eixo X

	CTexto*   pTexto;		// Objeto que gerencia texto
	CTexture* pTextures;	// Objeto que gerencia texturas
	CTimer*   pTimer;		// Objeto que gerencia o timer


	int		iFPS;			// FPS and FPS Counter
	int		iFrames;		// FPS and FPS Counter
	DWORD	ulLastFPS;		// FPS and FPS Counter
	char	szTitle[256];	// FPS and FPS Counter


	bool bIsWireframe;		// Modos Wireframe/Solid
	bool bIsCameraFPS;		// Ativa modo de camera First Person Shooter (true) ou Third Person Shooter (false)

	bool bShowAxis;			// Mostra e esconde os eixos X, Y e Z
	bool bDrawFog;			// Habilita e desabilita o efeito de neblina
	bool bDrawLightObjects; // Habilita e desabilita a visualização dos objetos de luz

	float fRenderPosY;
	float fTimerPosY;

	CModel_3DS* pModel3DS_BaseHouse;

	// Floor Material
	GLfloat FloorAmbient[4];
	GLfloat FloorDiffuse[4];
	GLfloat FloorSpecular[4];
	GLfloat FloorShininess;

	// Ambient Light
	GLfloat AmbianceLightAmbient[4];
	GLfloat AmbianceLightDiffuse[4];
	GLfloat AmbianceLightSpecular[4];
	GLfloat AmbianceLightPosition[4];

	// House 1 Light
	GLfloat House1Ambient[4];
	GLfloat House1Diffuse[4];
	GLfloat House1Specular[4];
	GLfloat House1Position[4];
	GLfloat House1Direction[3];
	GLfloat House1Cutoff;
	GLfloat House1Exponent;

	// House 2 Light
	GLfloat House2Ambient[4];
	GLfloat House2Diffuse[4];
	GLfloat House2Specular[4];
	GLfloat House2Position[4];
	GLfloat House2Direction[3];
	GLfloat House2Cutoff;
	GLfloat House2Exponent;
};

