#pragma once

#include "Vector2.h"

/************************************************************************/
/* Шаблонный класс "Трехмерный вектор"                                  */
/************************************************************************/
template <class T>
class Vector3
{
public:
	T x, y, z;

	/*
	Конструктор по умолчанию
	*/
	Vector3(void) noexcept
		:x(0), y(0), z(0)
	{
	}

	/*
	Инициализация вектора значениями из массива
	*/
	explicit Vector3(const T * p) noexcept
		:x(p[0]), y(p[1]), z(p[2])
	{
	}

	/*
	Непосредственная инициализация элементов вектора
	*/
	Vector3(T const& x0, T const& y0, T const& z0) noexcept
		:x(x0), y(y0), z(z0)
	{
	}

	/*
	Преобразование 2-х компонентного вектора в 3-х компонентный
	*/
	Vector3(Vector2<T> const& v, T const& z0 = 0) noexcept
		:x(v.x), y(v.y), z(z0)
	{
	}

	/*
	Получение адреса массива значений компонентов вектора
	*/
	operator T const*()const noexcept
	{
		return &x;
	}

	/*
	Масштабирование вектора
	*/
	Vector3 & operator *= (T const& scale) noexcept
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	/*
	Деление вектора на скаляр
	*/
	Vector3 & operator /= (T const& scalar) noexcept
	{
		return *this *= (1 / scalar);
	}

	/*
	Деление вектора на скаляр
	*/
	Vector3 const operator / (T const& scalar)const noexcept
	{
		return *this * (1 / scalar);
	}

	/*
	Разность векторов
	*/
	Vector3 const operator-(Vector3 const& v)const noexcept
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	/*
	Унарный минус
	*/
	Vector3 const operator-()const noexcept
	{
		return Vector3(-x, -y, -z);
	}

	/*
	Унарный плюс
	*/
	Vector3 const operator+()const noexcept
	{
		return *this;
	}

	/*
	Покомпонентное произведение векторов
	*/
	Vector3 const operator*(Vector3 const& v)const noexcept
	{
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	/*
	Покомпонентное деление векторов
	*/
	Vector3 const operator/(Vector3 const& v)const noexcept
	{
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	/*
	Сумма векторов
	*/
	Vector3 const operator+(Vector3 const& v)const noexcept
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	/*
	Сложение векторов
	*/
	Vector3 & operator+=(Vector3 const& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	/*
	Покомпонентное умножение векторов
	*/
	Vector3 & operator*=(Vector3 const& v) noexcept
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	/*
	Покомпонентное деление векторов
	*/
	Vector3 & operator/=(Vector3 const& v) noexcept
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	/*
	Вычитание векторов
	*/
	Vector3 & operator-=(Vector3 const& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	/*
	Умножение вектора на скаляр
	*/
	Vector3 const operator*(T const & scale)const noexcept
	{
		return Vector3(x * scale, y * scale, z * scale);
	}

	/*
	Вычисление длины вектора
	*/
	T GetLength()const noexcept
	{
		return static_cast<T>(sqrt(x * x + y * y + z * z));
	}

	// Нормализация вектора
	void Normalize() noexcept
	{
		T invLength = 1 / GetLength();
		x *= invLength;
		y *= invLength;
		z *= invLength;
	}
};

template<class T>
Vector3<T> const operator*(T const& scalar, Vector3<T> const& v) noexcept
{
	return v * scalar;
}


typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
