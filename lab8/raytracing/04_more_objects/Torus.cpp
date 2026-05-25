#include "stdafx.h"
#include "Torus.h"
#include "Intersection.h"
#include "Ray.h"

#include <algorithm>
#include <complex>
#include <vector>

static constexpr double EPSILON = 1e-6;

bool IsZero(double x, double eps = EPSILON)
{
	return std::abs(x) < eps;
}

void AddUniqueRoot(std::vector<double>& roots, double root)
{
	for (double existingRoot : roots)
	{
		if (std::abs(existingRoot - root) < EPSILON)
		{
			return;
		}
	}

	roots.push_back(root);
}

void AddRealRoot(std::vector<double>& roots, std::complex<double> const& root)
{
	if (std::abs(root.imag()) < EPSILON)
	{
		AddUniqueRoot(roots, root.real());
	}
}

std::vector<double> SolveQuadratic(double a, double b, double c)
{
	std::vector<double> roots;

	if (IsZero(a))
	{
		if (!IsZero(b))
		{
			roots.push_back(-c / b);
		}
		return roots;
	}

	double discriminant = b * b - 4 * a * c;
	if (discriminant < -EPSILON)
	{
		return roots;
	}

	if (IsZero(discriminant))
	{
		roots.push_back(-b / (2 * a));
		return roots;
	}

	double sqrtDiscriminant = sqrt(std::max(0.0, discriminant));
	roots.push_back((-b - sqrtDiscriminant) / (2 * a));
	roots.push_back((-b + sqrtDiscriminant) / (2 * a));
	return roots;
}

std::vector<double> SolveQuarticFerrari(
	double a4,
	double a3,
	double a2,
	double a1,
	double a0)
{
	std::vector<double> roots;

	if (IsZero(a4))
	{
		return roots;
	}

	double A = a3 / a4;
	double B = a2 / a4;
	double C = a1 / a4;
	double D = a0 / a4;

	double A2 = A * A;
	double A3 = A * A * A;
	double A4 = A * A * A * A;

	double p = B - 3.0 * A2 / 8.0;
	double q = C - A * B / 2.0 + A3 / 8.0;
	double r = D - A * C / 4.0 + A2 * B / 16.0 - 3.0 * A4 / 256.0;

	double shift = -A / 4.0;

	if (IsZero(q))
	{
		std::vector<double> y2Roots = SolveQuadratic(1.0, p, r);
		for (double y2 : y2Roots)
		{
			if (y2 > EPSILON)
			{
				double y = sqrt(y2);
				AddUniqueRoot(roots, shift - y);
				AddUniqueRoot(roots, shift + y);
			}
			else if (IsZero(y2))
			{
				AddUniqueRoot(roots, shift);
			}
		}

		std::sort(roots.begin(), roots.end());
		return roots;
	}

	using Complex = std::complex<double>;

	Complex cp(p, 0); //p + 0i
	Complex cq(q, 0);
	Complex cr(r, 0);

	Complex ferP = -cp * cp / 12.0 - cr;
	Complex ferQ = -cp * cp * cp / 108.0 + cp * cr / 3.0 - cq * cq / 8.0;
	Complex ferR = -ferQ / 2.0 + sqrt(ferQ * ferQ / 4.0 + ferP * ferP * ferP / 27.0);

	Complex ferU = pow(ferR, 1.0 / 3.0);

	Complex y;
	if (std::abs(ferU) < EPSILON)
	{
		y = -5.0 * cp / 6.0 - pow(ferQ, 1.0 / 3.0);
	}
	else
	{
		y = -5.0 * cp / 6.0 + ferU - ferP / (3.0 * ferU);
	}

	Complex W = sqrt(cp + 2.0 * y);

	if (std::abs(W) < EPSILON)
	{
		return roots;
	}

	Complex firstPart = -(3.0 * cp + 2.0 * y + 2.0 * cq / W);
	Complex secondPart = -(3.0 * cp + 2.0 * y - 2.0 * cq / W);

	Complex root1 = shift + (W + sqrt(firstPart)) / 2.0;
	Complex root2 = shift + (W - sqrt(firstPart)) / 2.0;
	Complex root3 = shift + (-W + sqrt(secondPart)) / 2.0;
	Complex root4 = shift + (-W - sqrt(secondPart)) / 2.0;

	AddRealRoot(roots, root1);
	AddRealRoot(roots, root2);
	AddRealRoot(roots, root3);
	AddRealRoot(roots, root4);

	std::sort(roots.begin(), roots.end());
	return roots;
}

Torus::Torus(
	double majorRadius,
	double tubeRadius,
	Vector3d const& center,
	Matrix4d const& transform)
	: GeometryObjectWithInitialTransformImpl(transform)
	, m_majorRadius(majorRadius)
	, m_tubeRadius(tubeRadius)
{
	assert(majorRadius > 0);
	assert(tubeRadius > 0);
	assert(majorRadius > tubeRadius);

	Matrix4d initialTransform;
	initialTransform.Translate(center.x, center.y, center.z);
	SetInitialTransform(initialTransform);
}

bool Torus::Hit(Ray const& ray, Intersection& intersection) const
{
	Ray invRay = Transform(ray, GetInverseTransform());

	Vector3d const& start = invRay.GetStart();
	Vector3d const& dir = invRay.GetDirection();

	double R = m_majorRadius;
	double r = m_tubeRadius;

	double k = R * R - r * r;

	double A2 = Dot(dir, dir);
	double A1 = 2.0 * Dot(start, dir);
	double A0 = Dot(start, start) + k;

	double B2 = dir.x * dir.x + dir.y * dir.y;
	double B1 = 2.0 * (start.x * dir.x + start.y * dir.y);
	double B0 = start.x * start.x + start.y * start.y;

	double c4 = A2 * A2;
	double c3 = 2.0 * A2 * A1;
	double c2 = A1 * A1 + 2.0 * A2 * A0 - 4.0 * R * R * B2;
	double c1 = 2.0 * A1 * A0 - 4.0 * R * R * B1;
	double c0 = A0 * A0 - 4.0 * R * R * B0;

	std::vector<double> roots = SolveQuarticFerrari(c4, c3, c2, c1, c0);

	for (double hitTime : roots)
	{
		if (hitTime <= EPSILON)
		{
			continue;
		}

		Vector3d hitPoint = ray.GetPointAtTime(hitTime);
		Vector3d hitPointInObjectSpace = invRay.GetPointAtTime(hitTime);

		double x = hitPointInObjectSpace.x;
		double y = hitPointInObjectSpace.y;
		double z = hitPointInObjectSpace.z;

		double sum = x * x + y * y + z * z + R * R - r * r;

		Vector3d hitNormalInObjectSpace(
			4.0 * x * (sum - 2.0 * R * R),
			4.0 * y * (sum - 2.0 * R * R),
			4.0 * z * sum);

		Vector3d hitNormal = GetNormalMatrix() * hitNormalInObjectSpace;

		HitInfo hit(
			hitTime,
			*this,
			hitPoint,
			hitPointInObjectSpace,
			hitNormal,
			hitNormalInObjectSpace);

		intersection.AddHit(hit);
	}

	return intersection.GetHitsCount() > 0;
}