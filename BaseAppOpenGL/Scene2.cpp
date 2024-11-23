#include "Scene2.h"

CScene2::CScene2()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;
	pModel3DS_BaseHouse = NULL;
	
	bIsWireframe = false;
	bIsCameraFPS = true;

	bShowAxis = false;
	bDrawFog = true;
	bDrawLightObjects = false;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[255] = 0;

	// Cria gerenciador de impress�o de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 1.0f, 20.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	pTextures = new CTexture();
	pTextures->CreateTextureClamp(0, "../Scene2/skybox/ny.png");
	pTextures->CreateTextureClamp(1, "../Scene2/skybox/py.png");
	pTextures->CreateTextureClamp(2, "../Scene2/skybox/pz.png");
	pTextures->CreateTextureClamp(3, "../Scene2/skybox/nz.png");
	pTextures->CreateTextureClamp(4, "../Scene2/skybox/nx.png");
	pTextures->CreateTextureClamp(5, "../Scene2/skybox/px.png");

	pTextures->CreateTextureClamp(6, "../Scene2/BaseFloor.bmp");

	pModel3DS_BaseHouse = new CModel_3DS();
	pModel3DS_BaseHouse->Load("../Scene2/BaseHouse.3DS");

	FloorAmbient[0]  = 0.2f; FloorAmbient[1]  = 0.2f; FloorAmbient[2]  = 0.2f; FloorAmbient[3]  = 1.0f;
	FloorDiffuse[0]  = 0.8f; FloorDiffuse[1]  = 0.8f; FloorDiffuse[2]  = 0.8f; FloorDiffuse[3]  = 1.0f;
	FloorSpecular[0] = 0.0f; FloorSpecular[1] = 0.0f; FloorSpecular[2] = 0.0f; FloorSpecular[3] = 1.0f;
	FloorShininess   = 0.0f;


	AmbianceLightAmbient[0]  = 0.0f; AmbianceLightAmbient[1]  = 0.0f; AmbianceLightAmbient[2]    = 0.0f; AmbianceLightAmbient[3]  = 1.0f;
	AmbianceLightDiffuse[0]  = 0.8f; AmbianceLightDiffuse[1]  = 0.8f; AmbianceLightDiffuse[2]    = 1.0f; AmbianceLightDiffuse[3]  = 1.0f;
	AmbianceLightSpecular[0] = 0.9f; AmbianceLightSpecular[1] = 0.9f; AmbianceLightSpecular[2]   = 1.0f; AmbianceLightSpecular[3] = 1.0f;
	AmbianceLightPosition[0] = 0.0f; AmbianceLightPosition[1] = 100.0f; AmbianceLightPosition[2] = 0.0f; AmbianceLightPosition[3] = 1.0f;

	CampfireAmbient[0]  = 0.2f; CampfireAmbient[1]   = 0.1f; CampfireAmbient[2]  = 0.0f; CampfireAmbient[3]  = 1.0f;
	CampfireDiffuse[0]  = 1.0f; CampfireDiffuse[1]   = 0.5f; CampfireDiffuse[2]  = 0.0f; CampfireDiffuse[3]  = 1.0f;
	CampfireSpecular[0] = 1.0f; CampfireSpecular[1]  = 0.5f; CampfireSpecular[2] = 0.0f; CampfireSpecular[3] = 1.0f;
	CampfirePosition[0] = 20.0f; CampfirePosition[1] = 0.5f; CampfirePosition[2] = 0.0f; CampfirePosition[3] = 1.0f;
}


CScene2::~CScene2(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}	
}




int CScene2::DrawGLScene(void)	// Fun��o que desenha a cena
{
	// Get FPS
	if (GetTickCount64() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount64();			// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter
	
	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posi��es da c�mera
	pCamera->setView();

	if (bShowAxis) {
		Draw3DSGrid(20.0f, 20.0f);
		DrawAxis();
	}

	// Modo FILL ou WIREFRAME (pressione barra de espa�o)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

	GLfloat fogColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (IN�CIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	SetupAmbientLight();
	SetupCampfireLight();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	CreateSkyBox(0.0f, 100.0f, 0.0f,
		1000.0f, 1000.0f, 1000.0f,
		pTextures);

	DrawFloor();

	if (bDrawFog) 
		SetFog(0.025f, 35.0f, 150.0f);
	else
		UnsetFog();

	DrawHouse(0.0f, 0.0f, 0.0f);
	DrawHouse(25.0f, 20.0f, 90.0f);
	DrawHouse(25.0f, -20.0f, -90.0f);

	glDisable(GL_TEXTURE_2D);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

	// Impress�o de texto na tela...
	// Muda para modo de proje��o ortogonal 2D
	// OBS: Desabilite Texturas e Ilumina��o antes de entrar nesse modo de proje��o
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, -30.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}

	glRasterPos2f(10.0f, -20.0f);
	if (!bShowAxis) {
		pTexto->glPrint("[F]    Mostrar Eixos");
	}
	else {
		pTexto->glPrint("[F]    Esconder Eixos");
	}

	glRasterPos2f(10.0f, -10.0f);
	if (!bDrawFog) {
		pTexto->glPrint("[G]    Habilitar Fog");
	}
	else {
		pTexto->glPrint("[G]    Desabilitar Fog");
	}

	glRasterPos2f(10.0f, 0.0f);
	if (!bDrawLightObjects) {
		pTexto->glPrint("[L]    Habilitar Objetos de Luz");
	}
	else {
		pTexto->glPrint("[L]    Desabilitar Objetos de Luz");
	}

	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posi��o do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplica��o e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime()/1000));


	glPopMatrix();

	// Muda para modo de proje��o perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene2::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os c�lculos de rota��o da vis�o do Player (atrav�s das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a c�mera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene2::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Sen�o, interrompe movimento do Player
	else
	{
	}



}

void CScene2::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		pTimer->Init();
		break;

	case 'F':
		bShowAxis = !bShowAxis;
		break;

	case 'G':
		bDrawFog = !bDrawFog;
		break;

	case 'L':
		bDrawLightObjects = !bDrawLightObjects;
		break;

	case VK_RETURN:
		break;

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene2::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void CScene2::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}

void CScene2::DrawFloor()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);

	glMaterialfv(GL_FRONT, GL_AMBIENT, FloorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, FloorDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, FloorSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, FloorShininess);

	pTextures->ApplyTexture(6);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0f, 1.0f, 1.0f);

	for (float i = -50.0f; i < 50.0f; i += 1.0f)
	{
		for (float j = -50.0f; j < 50.0f; j += 1.0f)
		{
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}

	glPopMatrix();
}
void CScene2::DrawHouse(float X, float Y, float Rotation)
{
	glPushMatrix();

	glTranslatef(X, 0.0f, Y);
	glRotatef(Rotation, 0.0f, 1.0f, 0.0f);
	pModel3DS_BaseHouse->Draw();

	glPopMatrix();
}

void CScene2::SetFog(float fDensity, float fStart, float fEnd)
{
	GLfloat fogColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glEnable(GL_FOG);

	glFogfv(GL_FOG_COLOR, fogColor);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, fStart);
	glFogf(GL_FOG_END, fEnd);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, fDensity);
}

void CScene2::UnsetFog()
{
	glDisable(GL_FOG);
}

void CScene2::SetupAmbientLight()
{
	glLightfv(GL_LIGHT0, GL_AMBIENT,  AmbianceLightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  AmbianceLightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, AmbianceLightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, AmbianceLightPosition);
	glEnable(GL_LIGHT0);
}

void CScene2::SetupCampfireLight()
{
	//if (bDrawLightObjects) {
	//	glDisable(GL_LIGHTING);

	//	glPushMatrix();
	//	glTranslatef(CampfirePosition[0], 1.0f, CampfirePosition[2]);
	//	glColor3f(CampfireDiffuse[0], CampfireDiffuse[1], CampfireDiffuse[2]);
	//	glutSolidSphere(0.5f, 10, 10);
	//	glPopMatrix();

	//	glEnable(GL_LIGHTING);
	//}

	glLightfv(GL_LIGHT1, GL_AMBIENT,  CampfireAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  CampfireDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, CampfireSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, CampfirePosition);
	glEnable(GL_LIGHT1);
}


void CScene2::CreateSkyBox(float x, float y, float z,
	float width, float height, float length,
	CTexture* pTextures)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();

	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	pTextures->ApplyTexture(0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();


	pTextures->ApplyTexture(1);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();


	pTextures->ApplyTexture(2);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();


	pTextures->ApplyTexture(3);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glEnd();


	pTextures->ApplyTexture(4);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glEnd();

	pTextures->ApplyTexture(5);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();
}
