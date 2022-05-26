#pragma once
#include"Hitable.h"
#include"Material.h"

class Sphere: public Hitable
{
public:
	Sphere(){}
	Sphere(Vec3 _Center, float R, shared_ptr<Material> _MatPtr) : Center(_Center), Radius(R), MatPtr(_MatPtr) {};
	inline virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const;
	inline virtual bool BoundingBox(double t0, double t1, AABB& outputBox) const override;
	Vec3 Center;
	double Radius;
	shared_ptr<Material> MatPtr;
};

bool Sphere::Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const {
	Vec3 OC = R.Origin() - Center;
	float A = Dot(R.Direction(), R.Direction());
	float B = 2.0 * Dot(OC, R.Direction());
	float C = Dot(OC, OC) - Radius * Radius;
	float Dis = B * B - 4 * A * C;
	if (Dis > 0.0) {
		Rec.MatPtr = MatPtr;
		float Temp = ((-B - sqrt(Dis)) / (2.0 * A));
		if (!(Temp < TMax && Temp > TMin)) Temp = ((-B + sqrt(Dis)) / (2.0 * A));
		if (!(Temp < TMax && Temp > TMin)) return false;
		Rec.T = Temp;
		Rec.P = R.PointAtParameter(Rec.T);
		Rec.Normal = (Rec.P - Center) / Radius;
		return true;
	}
	else return false;
}

inline bool Sphere::BoundingBox(double t0, double t1, AABB& outputBox) const
{
	outputBox = AABB(Center - Vec3(Radius, Radius, Radius), Center + Vec3(Radius, Radius, Radius));
	return true;
}


class MovingSphere : public Hitable {
public:
	MovingSphere(){}
	MovingSphere(Vec3 Cen0, Vec3 Cen1, double t0, double t1, double r, shared_ptr<Material> m) :Center0(Cen0), Center1(Cen1), time0(t0), time1(t1), Radius(r), MatPtr(m) {};
	inline virtual bool Hit(const Ray& R, double tmin, double tmax, HitRecord& Rec) const override;
	inline virtual bool BoundingBox(double t0, double t1, AABB& outputBox) const override;
	Vec3 GetCenter(double time) const;
	Vec3 Center0, Center1;
	double time0, time1;
	double Radius;
	shared_ptr<Material> MatPtr;
};

Vec3 MovingSphere::GetCenter(double time) const {
	return Center0 + ((time - time0) / (time1 - time0) * (Center1 - Center0));
}

bool MovingSphere::Hit(const Ray& R, double tmin, double tmax, HitRecord& Rec) const
{
	Vec3 OC = R.Origin() - GetCenter(R.Time());
	auto A = Dot(R.Direction(), R.Direction());
	auto HalfB = Dot(OC, R.Direction());
	auto C = Dot(OC, OC) - Radius * Radius;
	auto Dis = HalfB * HalfB -  A * C;
	if (Dis > 0) {
		auto Temp = (-HalfB - sqrt(Dis)) / A;

		if (Temp < tmax && Temp > tmin) {
			Rec.T = Temp;
			Rec.P = R.PointAtParameter(Temp);
			Vec3 OutwardNormal = (Rec.P - GetCenter(R.Time())) / Radius;
			Rec.Normal = OutwardNormal;
			Rec.MatPtr = MatPtr;
			return true;
		}
		Temp = (-HalfB + sqrt(Dis)) / A;
		if (Temp < tmax && Temp > tmin) {
			Rec.T = Temp;
			Rec.P = R.PointAtParameter(Temp);
			Vec3 OutwardNormal = (Rec.P - GetCenter(R.Time())) / Radius;
			Rec.Normal = OutwardNormal;
			Rec.MatPtr = MatPtr;
			return true;
		}
	}
	return false;
}

inline bool MovingSphere::BoundingBox(double t0, double t1, AABB& outputBox) const
{
	AABB Box0(GetCenter(t0) - Vec3(Radius, Radius, Radius),
		GetCenter(t0) + Vec3(Radius, Radius, Radius));
	AABB Box1(GetCenter(t1) - Vec3(Radius, Radius, Radius),
		GetCenter(t1) + Vec3(Radius, Radius, Radius));
	outputBox = SurroundingBox(Box0, Box1);
	return true;
}