// 01_raytrace_baseView.h : interface of the CMy01_raytrace_baseView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IGeometryObject_fwd.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "Scene.h"

class FrameBuffer;

class IShader;

class CRaytraceView : public CWindowImpl<CRaytraceView>
{
	enum
	{
		FRAMEBUFFER_UPDATE_TIMER = 1
	};

public:
	CRaytraceView();
	~CRaytraceView();
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CRaytraceView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
private:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void AddSomePlane();
	void AddSomeSpheres();
	void AddSomeLight();
	void AddSomeConicCylinders();
	void AddSomeTorusPyramid();

	// Отрисовка содержимого буфера кадра на контексте устройства
	void DrawFrameBuffer(CDC& dc, int x, int y);
	bool UpdateFrameBuffer();

	// Методы, создающие и добавляющие объекты к сцене
	SceneObject& AddSphere(std::shared_ptr<IShader const>, double radius = 1, Vector3d const& center = Vector3d(), Matrix4d const& transform = {});
	SceneObject& AddConicCylinder(std::shared_ptr<IShader const> shader, double height = 1, double baseRadius = 1, double capRadius = 0, Matrix4d const& transform = {});
	SceneObject& AddPlane(std::shared_ptr<IShader const> shader, double a, double b, double c, double d, Matrix4d const& transform = Matrix4d());
	SceneObject& AddSceneObject(std::shared_ptr<IGeometryObject const> object, std::shared_ptr<IShader const> shader);
	SceneObject& AddTorus(std::shared_ptr<IShader> shader, double majorRadius = 1, double tubeRadius = 0.25, Vector3d const& center = Vector3d(), Matrix4d const& transform = {});

private:
	RenderContext m_context;
	Renderer m_renderer;
	Scene m_scene;

	std::unique_ptr<FrameBuffer> m_pFrameBuffer;
};
