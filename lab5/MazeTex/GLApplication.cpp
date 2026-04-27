#pragma once
#include "pch.h"
#include "GLApplication.h"

GLApplication* GLApplication::m_pApplication = nullptr;

GLApplication::GLApplication(
	const char* title,
	int width,
	int height,
	bool needDepth,
	bool needStencil)
{
	// Только один экземпляр приложения разрешено создать
	if (m_pApplication)
	{
		assert(!"Only one instance of the application is permitted");
		return;
	}
	m_pApplication = this;

	// Инициализируем библиотеку GLUT, подсовывая ей
	// фиктивные параметры командой строки
	int argc = 1;
	char const* argv[] = { "" };
	glutInit(&argc, const_cast<char**>(argv));

	// Задаем режим буфера кадра, запрошенный пользователем
	glutInitDisplayMode(
		GLUT_RGBA | GLUT_DOUBLE | (needDepth ? GLUT_DEPTH : 0) | (needStencil ? GLUT_STENCIL : 0));

	// Если пользователь указал размеры окна, сообщаем о них GLUT
	if (width > 0 && height > 0)
	{
		glutInitWindowSize(width, height);
	}

	// Создаем окно приложения
	glutCreateWindow(title);

	// Задаем обработчики событий
	InitEventHandlers();
}

void GLApplication::InitEventHandlers()
{
	// Инициализируем обработчики часто используемых событий
	glutDisplayFunc(&DisplayHandler);
	glutReshapeFunc(&ReshapeHandler);
	glutKeyboardFunc(&KeyboardHandler);
	glutMouseFunc(&MouseHandler);
	glutMotionFunc(&MotionHandler);
	glutIdleFunc(&IdleHandler);

#if (GLUT_API_VERSION >= 2)
	glutSpaceballMotionFunc(&SpaceballMotionHandler);
	glutSpaceballRotateFunc(&SpaceballRotateHandler);
	glutSpaceballButtonFunc(&SpaceballButtonHandler);
#endif
	// При желании можно добавить обработчики остальных
	// событий, поддерживаемых glut
}

void GLApplication::SpaceballMotionHandler(int x, int y, int z)
{
	m_pApplication->OnSpaceballMotion(x, y, z);
}

void GLApplication::SpaceballRotateHandler(int x, int y, int z)
{
	m_pApplication->OnSpaceballRotation(x, y, z);
}

void GLApplication::SpaceballButtonHandler(int button, int state)
{
	m_pApplication->OnSpaceballButton(button, state);
}

void GLApplication::MotionHandler(int x, int y)
{
	// Переадресовываем вызов обработчика события экземпляру класса приложения
	m_pApplication->OnMotion(x, y);
}

void GLApplication::MouseHandler(int button, int state, int x, int y)
{
	// Переадресовываем вызов обработчика события экземпляру класса приложения
	m_pApplication->OnMouse(button, state, x, y);
}

void GLApplication::DisplayHandler()
{
	// Переадресовываем вызов обработчика события экземпляру класса приложения
	m_pApplication->OnDisplay();
	// Обмениваем местами теневой и лицевой буферы кадра
	glutSwapBuffers();
}

void GLApplication::IdleHandler()
{
	Sleep(1);
	// ничего не делаем
	m_pApplication->OnIdle();
}

void GLApplication::ReshapeHandler(int width, int height)
{
	// Переадресовываем вызов обработчика события экземпляру класса приложения
	m_pApplication->OnReshape(width, height);
}

void GLApplication::KeyboardHandler(unsigned char key, int x, int y)
{
	// Переадресовываем вызов обработчика события экземпляру класса приложения
	m_pApplication->OnKeyboard(key, x, y);
}

// Метод может быть перегружен в классе-наследнике
void GLApplication::OnReshape(int width, int height)
{
	// Обработчик по умолчанию всего лишь задает
	// порт просмотра размером с клиентскую область окна
	glViewport(0, 0, width, height);
}

// Метод может быть перегружен в классе-наследнике
void GLApplication::OnKeyboard(unsigned char /*key*/, int /*x*/, int /*y*/)
{
}

// Метод может быть перегружен в классе-наследнике
void GLApplication::OnMouse(int /*button*/, int /*state*/, int /*x*/, int /*y*/)
{
}

// Метод может быть перегружен в классе-наследнике
void GLApplication::OnMotion(int /*x*/, int /*y*/)
{
}

// Метод может быть перегружен в классе-наследнике
void GLApplication::OnIdle()
{
}

// Метод может быть перегружен в классе-наследнике
void GLApplication::OnSpaceballMotion(int /*x*/, int /*y*/, int /*z*/)
{
}

// Метод может быть перегружен в классе-наследнике
void GLApplication::OnSpaceballRotation(int /*x*/, int /*y*/, int /*z*/)
{
}

// Метод может быть перегружен в классе-наследнике
void GLApplication::OnSpaceballButton(int /*button*/, int /*state*/)
{
}

// Метод может быть перегружен в классе-наследнике
void GLApplication::OnInit()
{
}

void GLApplication::PostRedisplay()
{
	// Инициируем перерисовку калпа
	glutPostRedisplay();
}

void GLApplication::MainLoop(void)
{
	OnInit();
	glutMainLoop();
}

GLApplication::~GLApplication()
{
}