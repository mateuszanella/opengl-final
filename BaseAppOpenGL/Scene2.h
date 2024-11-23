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
	virtual int DrawGLScene(void);					// Fun��o que desenha a cena

	void Draw3DSGrid(float width, float length);
	void DrawAxis();

	void DrawFloor();
	void DrawHouse(float X, float Y, float Rotation);

	void SetFog(float fDensity, float fStart, float fEnd);
	void UnsetFog();

	void SetupAmbientLight();

	void CreateSkyBox(float x, float y, float z,
		float width, float height, float length,
		CTexture* pTextures);
private:

	bool	keys[256];		// Array usado para rotinas do teclado
	bool	active;			// Window Active Flag Set To TRUE By Default
	bool	fullscreen;		// Exibir janela em modo fullscreem (TRUE) ou em janela (FALSE)


	CCamera* pCamera;		// Gerencia c�mera OpenGL
	float fDeltaY;			// Rota��o da c�mera OpenGL no eixo Y
	float fDeltaX;			// Rota��o da c�mera OpenGL no eixo X

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

	float fRenderPosY;
	float fTimerPosY;

	CModel_3DS* pModel3DS_BaseHouse;

	// Ambient Light
	GLfloat LightAmbient[4];
	GLfloat LightDiffuse[4];
	GLfloat LightSpecular[4];
	GLfloat LightPosition[4];
};

