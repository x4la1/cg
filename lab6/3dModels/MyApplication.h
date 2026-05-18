#pragma once
#include "pch.h"
#include "GLApplication.h"
#include "Buffer.h"
#include "RotationContoller.h"
#include "ModelLoader.h"
#include "ModelRenderer.h"
#include "DirectLight.h"
#include "AnimationContoller.h"
#include "Scene.h"
#include "Lib3dsHelpers.h"

const double FIELD_OF_VIEW = 60;

class MyApplication : public GLApplication
{
public:
	MyApplication() :
		GLApplication("", 800, 600, true, false),
		m_rotationController(800, 600)
	{
	}

protected:
	void OnInit() override
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(1, 1, 1, 1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);

		DirectLight light;
		light.SetAmbientIntensity(0.2f, 0.2f, 0.2f);
		light.SetDiffuseIntensity(0.5f, 0.5f, 0.5f);
		light.SetSpecularIntensity(0.1f, 0.1f, 0.1f);
		light.SetLight(GL_LIGHT0);

		// Определяем ограничивающий блой данной модели
		BoundingBox const& modelBoundingBox = m_scene.GetBoundingBox();

		if (modelBoundingBox.IsEmpty())
		{
			throw std::runtime_error("Model is empty. Nothing to render");
		}

		// Центр bounding box-а модели
		glm::dvec3 modelCenter = modelBoundingBox.GetCenter();
		// Длина диагонали bounding box-а
		float modelDiagonal = glm::length(modelBoundingBox.GetSize());
		// Отодвигаем камеру от центра на расстояние, равное диагонали 
		// bounding box-а
		glm::dvec3 cameraPosition = modelCenter + glm::dvec3(modelDiagonal, 0, 0);

		// Задаем параметры камеры
		glLoadIdentity();
		gluLookAt(
			cameraPosition.x, cameraPosition.y, cameraPosition.z,
			modelCenter.x, modelCenter.y, modelCenter.z,
			0, 1, 0);
	}

	void OnDisplay() override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3ub(0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		DirectLight light({ 1.0f, 1.0f, 0.5f });
		light.SetLight(GL_LIGHT0);

		Material material;
		material.SetSpecular(0.3f, 0.3f, 0.3f);
		material.SetShininess(20);
		material.SetDiffuse(0.8f, 0.8f, 0.8f);
		material.Activate();

		m_scene.Display();
	}

	void OnMouse(int button, int state, int x, int y) override
	{
		m_rotationController.OnMouse(button, state, x, y);
	}

	void OnMotion(int x, int y) override
	{
		if (m_rotationController.LeftButtonIsPressed())
		{
			m_rotationController.OnMotion(x, y);
			PostRedisplay();
		}
	}

	void OnReshape(int width, int height) override
	{
		glViewport(0, 0, width, height);

		double aspect = double(width) / double(height);

		float zFar = glm::length(m_scene.GetBoundingBox().GetSize()) * 2;
		float zNear = zFar * (float)0.01;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(FIELD_OF_VIEW, aspect, zNear, zFar);
		glMatrixMode(GL_MODELVIEW);
	}

	void OnIdle() override
	{
		m_animationController.Tick();
		m_scene.Animate(m_animationController.GetTimeDelta());
		PostRedisplay();
	}

private:

	Scene m_scene;
	ModelRenderer m_renderer;
	RotationController m_rotationController;
	AnimationController m_animationController;
};
