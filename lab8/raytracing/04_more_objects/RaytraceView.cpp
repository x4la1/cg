// 01_raytrace_baseView.cpp : implementation of the CRaytraceView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrameBuffer.h"
#include "resource.h"

#include "CheckerShader.h"
#include "ConicCylinder.h"
#include "Intersection.h"
#include "OmniLightSource.h"
#include "Plane.h"
#include "Ray.h"
#include "RaytraceView.h"
#include "SceneObject.h"
#include "SimpleDiffuseShader.h"
#include "SimpleMaterial.h"
#include "Sphere.h"
#include "Torus.h"

CRaytraceView::CRaytraceView()
	: m_pFrameBuffer(std::make_unique<FrameBuffer>(800, 600))
{
	/*
	Задаем цвет заднего фона сцены
	*/
	m_scene.SetBackdropColor(Vector4f(1, 0, 1, 1));

	AddSomePlane();

	AddSomeTorusPyramid();

	AddSomeLight();

	/*
	Задаем параметры видового порта и матрицы проецирования в контексте визуализации
	*/
	m_context.SetViewPort(ViewPort(0, 0, 800, 600));
	Matrix4d proj;
	proj.LoadPerspective(60, 800.0 / 600.0, 0.1, 10);
	m_context.SetProjectionMatrix(proj);

	// Задаем матрицу камеры
	Matrix4d modelView;
	modelView.LoadLookAtRH(
		0, 3, 7,
		0, 0, 0,
		0, 1, 0);
	m_context.SetModelViewMatrix(modelView);
}

// Добавляем бесконечную шахматную плоскость y = 0
void CRaytraceView::AddSomePlane()
{
	// Задаем смещение текстурных координат в 1/2 размера шахматного кубика для того чтобы избежать
	// визуальных артефактов при определении цвета клетки, связанных с погрешностями вычислений
	Matrix4d checkerShaderTransform;
	checkerShaderTransform.Scale(0.2, 0.2, 0.2);
	checkerShaderTransform.Translate(0.25, 0.25, 0.25);
	AddPlane(std::make_shared<CheckerShader>(checkerShaderTransform), 0, 1, 0, 0);
}

// Добавляем несколько сфер
void CRaytraceView::AddSomeSpheres()
{
	SimpleMaterial yellow;
	yellow.SetDiffuseColor(Vector4f(1, 1, 0, 1));
	auto shader = std::make_shared<SimpleDiffuseShader>(yellow);
	AddSphere(shader, 1, Vector3d(0, 1, 0));
	AddSphere(std::move(shader), 0.5, Vector3d(2, 0, 0));
}

// Создаем и добавляем в сцену точечный источник света
void CRaytraceView::AddSomeLight()
{
	COmniLightPtr pLight(new OmniLightSource(Vector3d(-5, 5, 10)));
	pLight->SetDiffuseIntensity(Vector4f(1, 1, 1, 1));
	pLight->SetAttenuation(1, 0, 0.0005);
	m_scene.AddLightSource(pLight);
}

// Добавляем несколько конических цилиндров
void CRaytraceView::AddSomeConicCylinders()
{
	SimpleMaterial white;
	white.SetDiffuseColor(Vector4f(1, 1, 1, 1));

	Matrix4d transform;
	transform.Translate(-2.5, 0, 0);
	transform.Rotate(-90, 1, 0, 0);

	AddConicCylinder(std::make_shared<SimpleDiffuseShader>(white), 2, 1, 1, transform);

	SimpleMaterial red;
	red.SetDiffuseColor(Vector4f(1, 0, 0, 1));
	Matrix4d coneTransform;
	coneTransform.Translate(0, 0, 2);
	coneTransform.Rotate(-90, 1, 0, 0);

	AddConicCylinder(std::make_shared<SimpleDiffuseShader>(red), 1, 1, 0, coneTransform);

	Matrix4d conicFrustumTransform;
	conicFrustumTransform.Translate(4.0, 0.0, 0.0);
	conicFrustumTransform.Rotate(-90, 1, 0, 0);
	SimpleMaterial green;
	green.SetDiffuseColor(Vector4f(0, 1, 0, 1));
	AddConicCylinder(std::make_shared<SimpleDiffuseShader>(green), 1, 0.5, 0.3, conicFrustumTransform);
}

void CRaytraceView::AddSomeTorusPyramid()
{
	struct Ring
	{
		double majorRadius;
		double tubeRadius;
		Vector4f color;
	};

	std::vector<Ring> rings = {
		{ 1.25, 0.18, Vector4f(1.0f, 0.0f, 0.0f, 1.0f) },
		{ 1.00, 0.18, Vector4f(1.0f, 0.5f, 0.0f, 1.0f) },
		{ 0.78, 0.18, Vector4f(1.0f, 1.0f, 0.0f, 1.0f) },
		{ 0.56, 0.18, Vector4f(0.0f, 0.8f, 0.0f, 1.0f) },
		{ 0.36, 0.18, Vector4f(0.0f, 0.3f, 1.0f, 1.0f) },
	};

	for (size_t i = 0; i < rings.size(); ++i)
	{
		SimpleMaterial material;
		material.SetDiffuseColor(rings[i].color);

		Matrix4d transform;

		transform.Scale(2, 2, 2);
		transform.Translate(0, rings[i].tubeRadius + i * rings[i].tubeRadius * 1.75, 0);
		transform.Rotate(-90, 1, 0, 0);

		AddTorus(
			std::make_shared<SimpleDiffuseShader>(material),
			rings[i].majorRadius,
			rings[i].tubeRadius,
			Vector3d(),
			transform);
	}
}

CRaytraceView::~CRaytraceView()
{
	// Необходимо остановить фоновую работу объекта CRenderer до разрушения
	// данных класса CRaytraceView, т.к. CRenderer использует для своей работы
	// другие объекты, в частности, буфер кадра, разрушать которые можно только
	// после остановки CRenderer
	m_renderer.Stop();
}

BOOL CRaytraceView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CRaytraceView::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Сообщаем системе, что дальнейшая очистка буфера не требуется
	return 1;
}

LRESULT CRaytraceView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	// Получаем размеры клиентской области окна
	CRect clientRect;
	GetClientRect(clientRect);
	int clientWidth = clientRect.Width();
	int clientHeight = clientRect.Height();

	// Кисть, используемая для очистки окна
	CBrush whiteBrush;
	whiteBrush.CreateSolidBrush(0xffffff);

	// Проверка на случай отсутствия буфера кадра
	if (m_pFrameBuffer.get())
	{
		int w = m_pFrameBuffer->GetWidth();
		int h = m_pFrameBuffer->GetHeight();
		// Рисуем буфер кадра в левом верхнем углу
		// клиентской области окна
		DrawFrameBuffer(dc, 0, 0);

		// Т.к. мы отключили очистку заднего фона экрана,
		// область справа и снизу от изображения придется
		// очистить вручную

		// Очищаем правую часть клиентской области
		if (w < clientWidth)
		{
			dc.FillRect(CRect(w, 0, clientWidth, h), whiteBrush);
		}

		// Очищаем нижную часть клиентской области
		if (h < clientHeight)
		{
			dc.FillRect(CRect(0, h, clientWidth, clientHeight), whiteBrush);
		}
	}
	else // Буфер кадра по каким-то причинам не определен
	{
		ATLASSERT(!"Something bad with the frame buffer");
	}

	return 0;
}

void CRaytraceView::DrawFrameBuffer(CDC& dc, int x, int y)
{
	int w = m_pFrameBuffer->GetWidth();
	int h = m_pFrameBuffer->GetHeight();

	// Заполняем структуру BITMAPINFO (см. MSDN), чтобы
	// перекинуть содержимое буфера кадра на экран без создания дополнительных bitmap-ов
	BITMAPINFO bmpInfo;
	memset(&bmpInfo, 0, sizeof(bmpInfo));
	BITMAPINFOHEADER& hdr = bmpInfo.bmiHeader;
	hdr.biSize = sizeof(hdr);
	hdr.biWidth = w;
	// По умолчанию BMP файл хранится "вверх ногами" (сначала нижний ряд пикселей)
	// Чтобы использовать привычный вариант хранения пикселей высота должна быть отрицательной
	hdr.biHeight = -h;
	hdr.biPlanes = 1; // Количество цветовых плоскостей в изображении
	hdr.biBitCount = sizeof(std::uint32_t) << 3; // Цвет каждого пикселя занимает 32 бита
	hdr.biCompression = BI_RGB;
	hdr.biSizeImage = w * h * static_cast<int>(sizeof(std::uint32_t));

	// Используя функцию SetDIBitsToDevice (см. MSDN) перекидываем массив пикселей
	// из памяти в контекст устройства
	dc.SetDIBitsToDevice(
		x, y, // Координаты вывода внутри контекста устройства
		w, h, // Ширина и высота изображений
		0, 0, // Координаты рисуемой области изображения
		0, h, // Номер начальной строки и количество строк
		m_pFrameBuffer->GetPixels(), // Адрес пикселей
		&bmpInfo, // Адрес информации о пикселях
		DIB_RGB_COLORS); // сигнализируем о том, что значения в таблице
	// bmpInfo.bmiColors интерпретируются как RGB значения,
	// а не индексы логической палитры
}

LRESULT CRaytraceView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Запускаем процесс визуализации
	m_renderer.Render(m_scene, m_context, *m_pFrameBuffer);

	// Инициализируем таймер для отображения построенного изображения
	SetTimer(FRAMEBUFFER_UPDATE_TIMER, 500);
	return 0;
}

bool CRaytraceView::UpdateFrameBuffer()
{
	// Инициируем перерисовку окна
	RedrawWindow();

	unsigned totalChunks = 0;
	unsigned renderedChunks = 0;

	// Возвращаем true, если изображение в буфере кадра полностью построено
	return m_renderer.GetProgress(renderedChunks, totalChunks);
}

SceneObject& CRaytraceView::AddSphere(std::shared_ptr<IShader const> shader, double radius, Vector3d const& center, Matrix4d const& transform)
{
	return AddSceneObject(std::make_shared<Sphere>(radius, center, transform), std::move(shader));
}

SceneObject& CRaytraceView::AddConicCylinder(std::shared_ptr<IShader const> shader, double height, double baseRadius, double capRadius, Matrix4d const& transform)
{
	return AddSceneObject(std::make_shared<ConicCylinder>(height, baseRadius, capRadius, transform), std::move(shader));
}

SceneObject& CRaytraceView::AddPlane(std::shared_ptr<IShader const> shader, double a, double b, double c, double d, Matrix4d const& transform)
{
	return AddSceneObject(std::make_shared<Plane>(a, b, c, d, transform), std::move(shader));
}

SceneObject& CRaytraceView::AddSceneObject(std::shared_ptr<IGeometryObject const> object, std::shared_ptr<IShader const> shader)
{
	auto obj = std::make_shared<SceneObject>(std::move(object), std::move(shader));
	m_scene.AddObject(obj);
	return *obj;
}

SceneObject& CRaytraceView::AddTorus(std::shared_ptr<IShader> shader, double majorRadius, double tubeRadius, Vector3d const& center, Matrix4d const& transform)
{
	return AddSceneObject(
		std::make_shared<Torus>(majorRadius, tubeRadius, center, transform),
		std::move(shader));
}

LRESULT CRaytraceView::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	WPARAM timerId = wParam;
	switch (timerId)
	{
	case FRAMEBUFFER_UPDATE_TIMER:
		// Если выяснилось, что изображение построено, выключаем таймер,
		// перекидывающий буфер кадра в окно
		if (UpdateFrameBuffer())
		{
			KillTimer(FRAMEBUFFER_UPDATE_TIMER);
		}
		break;
	}

	return 0;
}
