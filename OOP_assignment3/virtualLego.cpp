#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>

using namespace std;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01

#define ballCnt 36
#define wallCnt 4
int ramainBallCnt = ballCnt;
int lifeCnt = 5;
bool isStart = false;

IDirect3DDevice9* Device = NULL;

D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

const int Width = 1024;
const int Height = 768;

const float spherePos[ballCnt][2] = { {-2, 0}, {-1, 0}, {0 , 0}, {1,0}, {2, 0}, {0, 1}, {0, 2}, {0, -1}, {0, -2}, {4, 0}, {-4, 0},
									  {-3, 0}, {-2.5f, 1}, {-2.0f, 2}, {-1.5f , 3}, {-1.0f,4}, {-0.5f, 5}, {0, 6}, {3, 0}, {2.5f, 1},
									  {2.0f, 2}, {1.5f , 3}, {1.0f,4}, {0.5f, 5} ,{-3, 0}, {-2.5f, -1}, {-2.0f, -2}, {2.5f, -1},
									  {2.0f, -2}, {1.5f, -3}, {-1.5f, -3} , {-1.0f, -3}, {-0.5f, -3}, {0, -3}, {0.5f, -3}, {1.0f, -3} };

const D3DXCOLOR sphereColor[ballCnt] = { d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW
										,d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW
										,d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW
										,d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW, d3d::YELLOW };


// -----------------------------------------------------------------------------
// CSphere class definition
// -----------------------------------------------------------------------------

class CSphere {
private:
	bool isRemove = false;
	float center_x, center_y, center_z;
	float m_radius;
	float m_velocity_x;
	float m_velocity_z;

	D3DXMATRIX m_mLocal;
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pSphereMesh;

public:
	// constructor
	CSphere(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_radius = 0;
		m_velocity_x = 0;
		m_velocity_z = 0;
		m_pSphereMesh = NULL;
	}
	~CSphere(void) {}

	// function
	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
			return false;
		return true;
	}

	void destroy(void)
	{
		if (m_pSphereMesh != NULL) {
			m_pSphereMesh->Release();
			m_pSphereMesh = NULL;
		}
	}

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pSphereMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball)
	{
		float distance, dx, dz;
		dx = (this->center_x - ball.center_x);
		dz = (this->center_z - ball.center_z);
		distance = sqrt(dx * dx + dz * dz);

		if (distance - 0.01f <= this->getRadius() + ball.getRadius())
			return true;
		else
			return false;
	}

	void hitBy(CSphere& ball)
	{
		if (hasIntersected(ball))
		{
			ball.m_velocity_z = -(ball.m_velocity_z);
			if (this->isRemove == false)
			{
				ramainBallCnt--;
				this->isRemove = true;
				this->setCenter(-50, 0, -50);
			}
		}
	}

	void ballUpdate(float timeDiff)
	{
		const float TIME_SCALE = 3.3;
		D3DXVECTOR3 cord = this->getCenter();
		double vx = abs(this->getVelocity_X());
		double vz = abs(this->getVelocity_Z());

		if (vx > 0.01 || vz > 0.01)
		{
			float tX = cord.x + TIME_SCALE * timeDiff * m_velocity_x;
			float tZ = cord.z + TIME_SCALE * timeDiff * m_velocity_z;
			this->setCenter(tX, cord.y, tZ);

			if(tX >= (4.5 - M_RADIUS))
				tX = 4.5 - M_RADIUS;
			else if(tX <=(-4.5 + M_RADIUS))
				tX = -4.5 + M_RADIUS;
			else if(tZ <= (-7 + M_RADIUS))
				tZ = -7 + M_RADIUS;
			else if(tZ >= (7 - M_RADIUS))
				tZ = 7 - M_RADIUS;
		}
		else { this->setPower(0, 0); }
		this->setPower(getVelocity_X(), getVelocity_Z());
	}

	// getter
	double getVelocity_X() { return this->m_velocity_x; }
	double getVelocity_Z() { return this->m_velocity_z; }
	float getRadius(void)  const { return (float)(M_RADIUS); }
	const D3DXMATRIX& getLocalTransform(void) const { return m_mLocal; }
	D3DXVECTOR3 getCenter(void) const
	{
		D3DXVECTOR3 org(center_x, center_y, center_z);
		return org;
	}

	// setter
	void setRemove()
	{
		this->isRemove = true;
	}
	void setPower(double vx, double vz)
	{
		this->m_velocity_x = vx;
		this->m_velocity_z = vz;
	}
	void setCenter(float x, float y, float z)
	{
		D3DXMATRIX m;
		center_x = x;	center_y = y;	center_z = z;
		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
};


// -----------------------------------------------------------------------------
// CWall class definition
// -----------------------------------------------------------------------------

class CWall {

private:

	float m_x;
	float m_z;
	float m_width;
	float m_depth;
	float m_height;

	D3DXMATRIX m_mLocal;
	D3DMATERIAL9 m_mtrl;
	ID3DXMesh* m_pBoundMesh;

	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }

public:
	// constructor
	CWall(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_width = 0;
		m_depth = 0;
		m_pBoundMesh = NULL;
	}
	~CWall(void) {}

	// function
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		m_width = iwidth;
		m_depth = idepth;

		if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
			return false;
		return true;
	}

	void destroy(void)
	{
		if (m_pBoundMesh != NULL) {
			m_pBoundMesh->Release();
			m_pBoundMesh = NULL;
		}
	}

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pBoundMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball)
	{
		if (this->m_x == 0.0f && this->m_z > 0.0f) // 위
			if (ball.getCenter().z + ball.getRadius() + 0.01f >= this->m_z - this->m_depth / 2)
				return true;
		if (this->m_x == 0.0f && this->m_z < 0.0f) // 아래
			if (ball.getCenter().z - ball.getRadius() - 0.01f <= this->m_z + this->m_depth / 2)
				return true;
		if (this->m_x < 0.0f && this->m_z == 0.0f) // 왼쪽
			if (ball.getCenter().x - ball.getRadius() - 0.01f <= this->m_x + this->m_width / 2)
				return true;
		if (this->m_x > 0.0f && this->m_z == 0.0f) // 오른쪽
			if (ball.getCenter().x + ball.getRadius() + 0.01f >= this->m_x - this->m_width / 2)
				return true;
		return false;
	}

	void hitBy(CSphere& ball)
	{
		if (hasIntersected(ball))
		{
			if (this->m_z == 0.0f) // left, right
				ball.setPower(-ball.getVelocity_X(), ball.getVelocity_Z());
			else if (this->m_x == 0.0f && this->m_z > 0) // up
				ball.setPower(ball.getVelocity_X(), -ball.getVelocity_Z());
			else if (this->m_x == 0.0f && this->m_z < 0) // down
			{
				ball.setPower(0, 0);
				ball.setCenter(0, ball.getCenter().y, -6.5f + 2 * M_RADIUS + 0.01f);
				isStart = false;
			}
		}
	}

	// getter
	float getHeight(void) const { return M_HEIGHT; }

	// setter
	void setPosition(float x, float y, float z)
	{
		D3DXMATRIX m;
		this->m_x = x;
		this->m_z = z;

		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}
};

// -----------------------------------------------------------------------------
// CLight class definition
// -----------------------------------------------------------------------------

class CLight {
private:
	DWORD               m_index;
	D3DXMATRIX          m_mLocal;
	D3DLIGHT9           m_lit;
	ID3DXMesh* m_pMesh;
	d3d::BoundingSphere m_bound;

public:
	// constructor
	CLight(void)
	{
		static DWORD i = 0;
		m_index = i++;
		D3DXMatrixIdentity(&m_mLocal);
		::ZeroMemory(&m_lit, sizeof(m_lit));
		m_pMesh = NULL;
		m_bound._center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_bound._radius = 0.0f;
	}
	~CLight(void) {}
	
	// function
	bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f)
	{
		if (NULL == pDevice)
			return false;
		if (FAILED(D3DXCreateSphere(pDevice, radius, 10, 10, &m_pMesh, NULL)))
			return false;

		m_bound._center = lit.Position;
		m_bound._radius = radius;

		m_lit.Type = lit.Type;
		m_lit.Diffuse = lit.Diffuse;
		m_lit.Specular = lit.Specular;
		m_lit.Ambient = lit.Ambient;
		m_lit.Position = lit.Position;
		m_lit.Direction = lit.Direction;
		m_lit.Range = lit.Range;
		m_lit.Falloff = lit.Falloff;
		m_lit.Attenuation0 = lit.Attenuation0;
		m_lit.Attenuation1 = lit.Attenuation1;
		m_lit.Attenuation2 = lit.Attenuation2;
		m_lit.Theta = lit.Theta;
		m_lit.Phi = lit.Phi;
		return true;
	}

	void draw(IDirect3DDevice9* pDevice)
	{
		if (NULL == pDevice)
			return;
		D3DXMATRIX m;
		D3DXMatrixTranslation(&m, m_lit.Position.x, m_lit.Position.y, m_lit.Position.z);
		pDevice->SetTransform(D3DTS_WORLD, &m);
		pDevice->SetMaterial(&d3d::WHITE_MTRL);
		m_pMesh->DrawSubset(0);
	}

	void destroy(void)
	{
		if (m_pMesh != NULL) {
			m_pMesh->Release();
			m_pMesh = NULL;
		}
	}

	// getter
	D3DXVECTOR3 getPosition(void) const { return D3DXVECTOR3(m_lit.Position); }

	// setter
	bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return false;

		D3DXVECTOR3 pos(m_bound._center);
		D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
		D3DXVec3TransformCoord(&pos, &pos, &mWorld);
		m_lit.Position = pos;

		pDevice->SetLight(m_index, &m_lit);
		pDevice->LightEnable(m_index, TRUE);
		return true;
	}
};


// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------

CWall	g_legoPlane;
CWall	g_legowall[wallCnt];
CSphere	g_sphere[ballCnt];
CSphere	g_sphereMoving;
CSphere g_sphereUser;
CLight	g_light;

double g_camera_pos[3] = { 0.0, 5.0, -8.0 };

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

bool initBall()
{
	// brick balls
	for (int i = 0; i < ballCnt; i++) {
		if (!g_sphere[i].create(Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS, spherePos[i][1]);
		g_sphere[i].setPower(0, 0);
	}

	// moving ball
	if (!g_sphereMoving.create(Device, d3d::RED)) return false;
	g_sphereMoving.setCenter(0.0f, (float)M_RADIUS, -6.5f + 2 * M_RADIUS + 0.01f);
	g_sphereMoving.setPower(0, 0);
	g_sphereMoving.setRemove(); // 부딫혀도 안사라지게

	// control ball
	if (!g_sphereUser.create(Device, d3d::WHITE)) return false;
	g_sphereUser.setCenter(0.0f, (float)M_RADIUS, -6.5f);
	g_sphereUser.setPower(0, 0);
	g_sphereUser.setRemove(); // 부딫혀도 안사라지게

	ramainBallCnt = ballCnt;
	lifeCnt = 5;

	return true;

}

bool Setup()
{
	D3DXMatrixIdentity(&g_mWorld);
	D3DXMatrixIdentity(&g_mView);
	D3DXMatrixIdentity(&g_mProj);

	// create plane and set the position
	if (!g_legoPlane.create(Device, -1, -1, 9, 0.03f, 14, d3d::PLANECOLOR)) return false;
	g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);

	// create walls and set the position
	// 위
	if (!g_legowall[0].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::BLUE)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 6.94f);
	// 아래
	if (!g_legowall[1].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::RED)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -6.94f);
	// 오른쪽
	if (!g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, 14, d3d::BLUE)) return false;
	g_legowall[2].setPosition(4.56f, 0.12f, 0.0f);
	// 왼쪽
	if (!g_legowall[3].create(Device, -1, -1, 0.12f, 0.3f, 14, d3d::BLUE)) return false;
	g_legowall[3].setPosition(-4.56f, 0.12f, 0.0f);

	// create balls
	if (!initBall()) return false;



	// light setting 
	D3DLIGHT9 lit;
	::ZeroMemory(&lit, sizeof(lit));
	lit.Type = D3DLIGHT_POINT;
	lit.Diffuse = d3d::WHITE;
	lit.Specular = d3d::WHITE * 0.9f;
	lit.Ambient = d3d::WHITE * 0.9f;
	lit.Position = D3DXVECTOR3(0.0f, 6.0f, 5.0f);
	lit.Range = 100.0f;
	lit.Attenuation0 = 0.0f;
	lit.Attenuation1 = 0.3f;
	lit.Attenuation2 = 0.0f;
	if (false == g_light.create(Device, lit))
		return false;

	// Position and aim the camera.
	D3DXVECTOR3 pos(0.0f, 10.0f, -13.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);

	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
		(float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

	// Set render states.
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	g_light.setLight(Device, g_mWorld);
	return true;
}

void Cleanup(void)
{
	g_legoPlane.destroy();
	for (int i = 0; i < wallCnt; i++) {
		g_legowall[i].destroy();
	}
	g_light.destroy();
}


// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();

		for (int i = 0; i < ballCnt; i++)
			g_sphere[i].ballUpdate(timeDelta);
		g_sphereMoving.ballUpdate(timeDelta);

		for (int i = 0; i < wallCnt; i++) 
			g_legowall[i].hitBy(g_sphereMoving);
		for (int i = 0; i < ballCnt; i++) 
			g_sphere[i].hitBy(g_sphereMoving);
		g_sphereUser.hitBy(g_sphereMoving);
			

		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld);
		for (int i = 0; i < wallCnt; i++) 
			g_legowall[i].draw(Device, g_mWorld);
		for (int i = 0; i < ballCnt; i++)
			g_sphere[i].draw(Device, g_mWorld);
		g_sphereMoving.draw(Device, g_mWorld);
		g_sphereUser.draw(Device, g_mWorld);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);

		if (ramainBallCnt == 0) exit(0); // 공을 전부 깨면 프로그램 종료
		if (lifeCnt == 0) exit(0); // 목숨을 전부 소모하면 프로그램 종료
	}
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool isReset = true;
	static int old_x = 0;
	static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;
	
	switch (msg) {
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			switch (wParam) {
				case VK_ESCAPE:
					::DestroyWindow(hwnd);
					break;
				case VK_SPACE:
					if (!isStart)
					{
						isStart = true;
						g_sphereMoving.setPower(2.0f, 2.0f);
					}
					break;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			int new_x = LOWORD(lParam);
			int new_y = HIWORD(lParam);
			float dx;
			float dy;
			if (LOWORD(wParam)) {
				dx = (old_x - new_x);// * 0.01f;

				D3DXVECTOR3 coord3d = g_sphereUser.getCenter();
				g_sphereUser.setCenter(coord3d.x + dx * (-0.012f), coord3d.y, coord3d.z);
				if (!isStart) g_sphereMoving.setCenter(coord3d.x + dx * (-0.012f), coord3d.y, coord3d.z + 2 * M_RADIUS + 0.01f);
			}
			old_x = new_x;

			move = WORLD_MOVE;
			break;
		}
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE prevInstance,PSTR cmdLine,int showCmd)
{
	srand(static_cast<unsigned int>(time(NULL)));

	if (!d3d::InitD3D(hinstance, Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}