#pragma once

#include "Vector3.h"

/************************************************************************/
/* Шаблонный класс "Четырехмерный вектор"                               */
/************************************************************************/
template <class T>
class Vector4
{
public:
	T x;
	T y;
	T z;
	T w;

	/*
	Конструктор по умолчанию
	*/
	Vector4(void) noexcept
		:x(0), y(0), z(0), w(0)
	{
	}

	/*
	Инициализация вектора значениями из массива
	*/
	explicit Vector4(const T * p) noexcept
		:x(p[0]), y(p[1]), z(p[2]), w(p[3])
	{
	}

	/*
	Непосредственная инициализация элементов вектора
	*/
	Vector4(T const& x0, T const& y0, T const& z0, T const& w0 = 0) noexcept
		:x(x0), y(y0), z(z0), w(w0)
	{
	}

	/*
	Преобразование 3-х компонентного вектора в 4-х компонентный
	*/
	Vector4(Vector3<T> const& v, T const& w0 = 0) noexcept
		:x(v.x), y(v.y), z(v.z), w(w0)
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
	Vector4 & operator *= (T scale) noexcept
	{
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;
		return *this;
	}


	/*
	Деление вектора на скаляр
	*/
	Vector4 & operator /= (T const& scalar) noexcept
	{
		return *this *= (1/scalar);
	}

	/*
	Деление вектора на скаляр
	*/
	Vector4 const operator / (T const& scalar)const noexcept
	{
		return *this * (1/scalar);
	}

	/*
	Разность векторов
	*/
	Vector4 const operator-(Vector4 const& v)const noexcept
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}


	/*
	Унарный минус
	*/
	Vector4 const operator-()const noexcept
	{
		return Vector4(-x, -y, -z, -w);
	}

	/*
	Унарный плюс
	*/
	Vector4 const operator+()const noexcept
	{
		return *this;
	}

	/*
	Покомпонентное произведение векторов
	*/
	Vector4 const operator*(Vector4 const& v)const noexcept
	{
		return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	/*
	Покомпонентное деление векторов
	*/
	Vector4 const operator/(Vector4 const& v)const noexcept
	{
		return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
	}

	/*
	Сумма векторов
	*/
	Vector4 const operator+(Vector4 const& v)const noexcept
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	/*
	Сложение векторов
	*/
	Vector4 & operator+=(Vector4 const& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	/*
	Покомпонентное умножение векторов
	*/
	Vector4 & operator*=(Vector4 const& v) noexcept
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	/*
	Покомпонентное деление векторов
	*/
	Vector4 & operator/=(Vector4 const& v) noexcept
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	/*
	Вычитание векторов
	*/
	Vector4 & operator-=(Vector4 const& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	/*
	Умножение вектора на скаляр
	*/
	Vector4 const operator*(T scale)const noexcept
	{
		return Vector4(x * scale, y * scale, z * scale, w * scale);
	}

	/*
	Вычисление длины вектора
	*/
	T GetLength()const noexcept
	{
		return static_cast<T>(sqrt(x * x + y * y + z * z + w * w));
	}

	/*
	Преобразование к трехмерному вектору путем отбрасывания координаты w
	*/
	operator Vector3<T> ()const noexcept
	{
		return Vector3<T>(x, y, z);
	}

	/*
	Преобразование к трехмерному вектору путем выполнения перспективного деления
	*/
	Vector3<T> Project()const noexcept
	{
		T invW = 1 / w;
		return Vector3<T>(x * invW, y * invW, z * invW);
	}

	// Нормализация вектора
	void Normalize() noexcept
	{
		T invLength = 1 / GetLength();
		x *= invLength;
		y *= invLength;
		z *= invLength;
		w *= invLength;
	}
};


template<class T>
Vector4<T> const operator*(T const& scalar, Vector4<T> const& v) noexcept
{
	return v * scalar;
}

typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
