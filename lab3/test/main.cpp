#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <tchar.h>

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGTH = 600;
int g_windowWidth = WINDOW_WIDTH;
int g_windowHeight = WINDOW_HEIGTH;
const char WINDOW_TITLE[] = "My first OpenGL application";

void OnResizeWindow(int width, int height);
void Draw5PointStar(float xCenter, float yCenter, float radius);
void OnDrawScene();

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGTH);
	glutCreateWindow(WINDOW_TITLE);
	glutDisplayFunc(&OnDrawScene);
	glutReshapeFunc(&OnResizeWindow);
	glutMainLoop();
	return 0;
}

void OnDrawScene()
{
	/*
	Задаем цвет очистки буфера кадра в формате RGBA (желтый)
	*/
	glClearColor(1, 1, 0, 0);

	// Очищаем буфер цвета и буфер глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Делаем текущий цвет вершины синим
	// суффикс 3f в названии функции обозначает, что будут указаны
	// 3 составляющих цвета (r, g, b) в формате float (от 0 до 1).
	glColor3f(0, 0, 1);

	// Рисуем пятиконечную звезду
	Draw5PointStar(300, 200, 120);
	glutSwapBuffers();
}

void Draw5PointStar(float xCenter, float yCenter, float radius)
{
	static const float STEP = M_PI * 4 / 5;

	glBegin(GL_LINE_LOOP);
	float angle = -M_PI / 2;

	// Соединяем отрезками прямой линии точки, расположенные на окружности
	// в точках, с углами: -90, 54, 198, 342, 486 (126) градусов
	for (int i = 0; i < 5; ++i, angle += STEP)
	{
		float x = xCenter + radius * cosf(angle);
		float y = yCenter + radius * sinf(angle);
		// функция glVertex2f добавляет в текущую группу примитивов
		// точку, лежащую на плоскости z = 0
		// суффикс 2f в названии функции обозначает, что задаются 2 координаты
		// x и y типа GLfloat
		glVertex2f(x, y);
	}

	// Заканчиваем группу примитивов
	glEnd();
}

void OnResizeWindow(int width, int height)
{
	g_windowWidth = width;
	g_windowHeight = height;

	/*
	Задаем положение и размеры видового порта (порта просмотра) в буфере кадра
	Вывод примитивов не будет осуществляться за его пределы
	*/
	glViewport(0, 0, width, height);

	// Делаем текущей матрицей OpenGL матрицу проецирования
	glMatrixMode(GL_PROJECTION);

	// Загружаем в нее единичную матрицу
	glLoadIdentity();

	/*
	И умножаем ее на матрицу ортографического проецирования такую,
	что левому верхнему углу просмотра будет соответствовать
	точка (0, 0), правому нижнему - (width, height),
	а сцена будет включать в себя объект, расположенные по глубине в диапазоне
	от -1 (сзади) до +1 (спереди)
	*/
	glOrtho(0, width, height, 0, -1, 1);

	// Делаем текущей матрицей матрицу моделирования-вида
	glMatrixMode(GL_MODELVIEW);
}