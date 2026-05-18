#pragma once
#include "pch.h"
#include "ModelRenderer.h"
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "BaseTexture.h"

const float CAR_SPEED = 20.0;
const std::wstring FLOOT_TEXTURE_PATH = L"textures/floor_texture.jpg";
const std::wstring ROAD_TEXTURE_PATH = L"textures/road_texture.jpg";

const std::string CAR_MODEL_PATH = "models/car/car.3ds";
const std::string GIRL_MODEL_PATH = "models/girl/girl.3ds";
const std::string HOUSE_MODEL_PATH = "models/house/house.3ds";
const std::string PLAYGROUND_MODEL_PATH = "models/slide/slide.3ds";
const std::string TREE_MODEL_PATH = "models/tree/tree.3ds";



class Scene
{
public:
	Scene()
		:m_deltaTime(0),
		m_carPositionX(-45)
	{
		LoadModels();
	}

	void Display()
	{
		DrawFloor();
		DrawCar();
		DrawHouse();
		DrawGirl();
		DrawPlayground();
		DrawTrees();
		DrawRoad();
	}

	void Animate(double deltaTime)
	{
		m_deltaTime = deltaTime;
		double deltaSeconds = deltaTime / 1000.0;

		m_carPositionX += CAR_SPEED * deltaSeconds;

		if (m_carPositionX > 40.0f)
		{
			m_carPositionX = -40.0f;
		}
	}

	BoundingBox GetBoundingBox()
	{
		return BoundingBox({ -30.0, 0.0, -30.0 }, { 30.0, 25.0, 30.0 });
	}

private:
	void LoadModels()
	{
		TextureLoader texLoader;
		m_floorTexture = texLoader.LoadTexture2D(FLOOT_TEXTURE_PATH);
		m_roadTexture = texLoader.LoadTexture2D(ROAD_TEXTURE_PATH);

		ModelLoader modelLoader;
		modelLoader.Load3dsFile(CAR_MODEL_PATH.c_str(), m_car);
		modelLoader.Load3dsFile(GIRL_MODEL_PATH.c_str(), m_girl);
		modelLoader.Load3dsFile(HOUSE_MODEL_PATH.c_str(), m_house);
		modelLoader.Load3dsFile(PLAYGROUND_MODEL_PATH.c_str(), m_playground);
		modelLoader.Load3dsFile(TREE_MODEL_PATH.c_str(), m_tree);
	}

	void DrawFloor()
	{
		glEnable(GL_TEXTURE_2D);
		Texture2DHandle(m_floorTexture).Bind();
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		float size = 50.0f;
		float repeat = 10.0f;

		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);           glVertex3f(-size, 0.0f, size);
		glTexCoord2f(repeat, 0.0f);      glVertex3f(size, 0.0f, size);
		glTexCoord2f(repeat, repeat); glVertex3f(size, 0.0f, -size);
		glTexCoord2f(0.0f, repeat);      glVertex3f(-size, 0.0f, -size);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}

	void DrawCar()
	{
		glPushMatrix();
		glTranslatef(m_carPositionX, 0.0f, 6.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.04f, 0.04f, 0.04f);
		m_renderer.RenderModel(m_car);
		glPopMatrix();
	}

	void DrawHouse()
	{
		glPushMatrix();
		glTranslatef(25.0f, 0.0f, -25.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glScalef(0.003f, 0.003f, 0.003f);
		m_renderer.RenderModel(m_house);
		glPopMatrix();
	}

	void DrawGirl()
	{
		glPushMatrix();
		glTranslatef(2.0f, 0.0f, -15.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glScalef(4.0f, 4.0f, 4.0f);
		m_renderer.RenderModel(m_girl);
		glPopMatrix();
	}

	void DrawPlayground()
	{
		glPushMatrix();
		glTranslatef(-30.0f, 7.0f, -30.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.01f, 0.01f, 0.01f);
		m_renderer.RenderModel(m_playground);
		glPopMatrix();
	}
	void DrawTrees()
	{
		DrawSingleTree(-37.0f, 19.0f, 45.0f, 32.0f);
		DrawSingleTree(-31.0f, 19.0f, 45.0f, 32.0f);
		DrawSingleTree(-27.0f, 19.0f, 41.0f, 44.0f);
		DrawSingleTree(-17.0f, 19.0f, 42.0f, 52.0f);
		DrawSingleTree(-10.0f, 19.0f, 40.0f, 76.0f);
		DrawSingleTree(0.0f, 19.0f, 43.0f, 23.0f);
		DrawSingleTree(17.0f, 19.0f, 40.0f, 114.0f);
		DrawSingleTree(24.0f, 19.0f, 41.0f, 234.0f);
		DrawSingleTree(30.0f, 19.0f, 41.0f, 234.0f);
		DrawSingleTree(30.0f, 19.0f, 44.0f, 22.0f);
		DrawSingleTree(38.0f, 19.0f, 45.0f, 123.0f);
	}

	void DrawRoad()
	{
		glEnable(GL_TEXTURE_2D);
		Texture2DHandle(m_roadTexture).Bind();

		float roadLength = 50.0f;
		float roadZMin = 2.0f;
		float roadZMax = 20.0f;
		float roadY = 0.02f;

		float texZ = 5.0f;
		float texX = 1.0f;

		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);       glVertex3f(-roadLength, roadY, roadZMax);
		glTexCoord2f(0.0f, texZ);    glVertex3f(roadLength, roadY, roadZMax);
		glTexCoord2f(texX, texZ); glVertex3f(roadLength, roadY, roadZMin);
		glTexCoord2f(texX, 0.0f);    glVertex3f(-roadLength, roadY, roadZMin);

		glEnd();

		glDisable(GL_TEXTURE_2D);
	}

	void DrawSingleTree(float x, float y, float z, float rotateAngle)
	{
		glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(rotateAngle, 0.0f, 0.0f, 1.0f);
		glScalef(0.01f, 0.01f, 0.01f);
		m_renderer.RenderModel(m_tree);
		glPopMatrix();
	}

private:
	ModelRenderer m_renderer;

	Model m_tree;
	Model m_girl;
	Model m_house;
	Model m_car;
	Model m_playground;

	GLuint m_floorTexture;
	GLuint m_roadTexture;

	float m_carPositionX;
	float m_deltaTime;
};