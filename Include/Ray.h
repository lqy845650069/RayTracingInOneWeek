#pragma once

#include"Vec3.h"
class Ray
{
public:
	Ray() {}
	Ray(const Vec3& Origin, const Vec3 Direction, double time = 0.0): A(Origin), B(Direction), tm(time){}
	Vec3 Origin() const { return A; }
	Vec3 Direction() const { return B; }
	double Time() const { return tm; }

	Vec3 PointAtParameter(float T) const { return A + T * B; }

	friend inline Vec3 Reflect(const Vec3& V, const Vec3& N);
	friend inline bool Refract(const Vec3& V, const Vec3& N, float NiOberNt, Vec3& Refracted);

	Vec3 A;
	Vec3 B;
	double tm;
private:

};

inline Vec3 Reflect(const Vec3& V, const Vec3& N)
{
	return V - 2 * Dot(V, N) * N;
}

inline bool Refract(const Vec3& V, const Vec3& N, float NiOverNt, Vec3& Refracted)
{
	Vec3 UV = UnitVector(V);
	float Dt = Dot(UV, N);
	float Discriminant = 1.0 - NiOverNt * NiOverNt * (1 - Dt * Dt);

	if (Discriminant > 0) {
		Refracted = NiOverNt * (UV - N * Dt) - N * sqrt(Discriminant);
		return true;
	}
	else return false;
}