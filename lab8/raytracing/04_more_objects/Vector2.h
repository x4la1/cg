#pragma once

/************************************************************************/
/* Шаблонный класс "Двухмерный вектор"                                  */
/************************************************************************/
template <class T>
class Vector2
{
public:

	/*
	Конструктор по умолчанию
	*/
	Vector2(void) noexcept
		:x(0),y(0)
	{
	}

	/*
	Инициализация вектора значениями из массива
	*/
	explicit Vector2(const T * p) noexcept
		:x(p[0]), y(p[1])
	{
	}

	/*
	Непосредственная инициализация элементов вектора
	*/
	Vector2(T const& x0, T const& y0) noexcept
		:x(x0), y(y0)
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
	Vector2 & operator *= (T const& scale) noexcept
	{
		x *= scale;
		y *= scale;
		return *this;
	}


	/*
	Деление вектора на скаляр
	*/
	Vector2 & operator /= (T const& scalar) noexcept
	{
		return *this *= (1/scalar);
	}

	/*
	Деление вектора на скаляр
	*/
	Vector2 const operator / (T const& scalar)const noexcept
	{
		return *this * (1/scalar);
	}


	/*
	Разность векторов
	*/
	Vector2 const operator-(Vector2 const& v)const noexcept
	{
		return Vector2(x - v.x, y - v.y);
	}

	/*
	Унарный минус
	*/
	Vector2 const operator-()const noexcept
	{
		return Vector2(-x, -y);
	}

	/*
	Унарный плюс
	*/
	Vector2 const operator+()const noexcept
	{
		return *this;
	}

	/*
	Покомпонентное произведение векторов
	*/
	Vector2 const operator*(Vector2 const& v)const noexcept
	{
		return Vector2(x * v.x, y * v.y);
	}

	/*
	Покомпонентное деление векторов
	*/
	Vector2 const operator/(Vector2 const& v)const noexcept
	{
		return Vector2(x / v.x, y / v.y);
	}

	/*
	Сумма векторов
	*/
	Vector2 const operator+(Vector2 const& v)const noexcept
	{
		return Vector2(x + v.x, y + v.y);
	}

	/*
	Сложение векторов
	*/
	Vector2 & operator+=(Vector2 const& v) noexcept
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	/*
	Покомпонентное умножение векторов
	*/
	Vector2 & operator*=(Vector2 const& v) noexcept
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	/*
	Покомпонентное деление векторов
	*/
	Vector2 & operator/=(Vector2 const& v) noexcept
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	/*
	Вычитание векторов
	*/
	Vector2 & operator-=(Vector2 const& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	/*
	Умножение вектора на скаляр
	*/
	Vector2 const operator*(T const& scale)const noexcept
	{
		return Vector2(x * scale, y * scale);
	}

	/*
	Вычисление длины вектора
	*/
	T GetLength()const noexcept
	{
		return static_cast<T>(sqrt(x * x + y * y));
	}

	// Нормализация вектора
	void Normalize() noexcept
	{
		T invLength = 1 / GetLength();
		x *= invLength;
		y *= invLength;
	}

	T x;
	T y;
};

template<class T>
Vector2<T> const operator*(T const& scalar, Vector2<T> const& v) noexcept
{
	return v * scalar;
}

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
