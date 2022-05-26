#pragma once
#include"Hitable.h"

struct HitRecord;
class Material {
public:
	virtual bool Scatter(const Ray& RIn, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const = 0;
};

class Lambertian :public Material {
public:
	Lambertian(const Vec3& A) : Albedo(A) {}

	virtual bool Scatter(const Ray& RIn, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const {
		Vec3 Target = Rec.P + Rec.Normal + random_in_unit_sphere();

		Scattered = Ray(Rec.P, Target - Rec.P, RIn.Time());
		//Attenuation = Albedo;
		//return true;
		if (random_double() < PR) {
			Attenuation = Albedo / PR;
			return true;
		}
		else return false;
	}

	Vec3 Albedo;//·´ÉäÂÊ
private:
	static double PR;
};

double Lambertian::PR = 0.8;


class Metal : public Material {
public:
	Metal(const Vec3& A, float F) : Albedo(A) { Fuzz = F > 1 ? 1: F < 0 ? 0 : F; }
	virtual bool Scatter(const Ray& RIn, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const {
		Vec3 Reflected = Reflect(UnitVector(RIn.Direction()), Rec.Normal);
		Scattered = Ray(Rec.P, Reflected  + Fuzz * random_in_unit_sphere());
		Attenuation = Albedo;
		return (Dot(Scattered.Direction(), Rec.Normal) > 0);
	}

	Vec3 Albedo;
	float Fuzz;
};


inline float Schlick(float Cosine, float Ref)
{
	float R0 = (1 - Ref) / (1 + Ref);
	R0 = R0 * R0;
	return R0 + (1 - R0) * pow((1 - Cosine), 5);
}

class Dielectric : public Material {
public:
	Dielectric(float Ri) :RefIdx(Ri){}
	inline friend float Schlick(float Cosine, float Ref);

	virtual bool Scatter(const Ray& RIn, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const {
		Vec3 OutwardNormal;
		Vec3 Reflected = Reflect(RIn.Direction(), Rec.Normal);
		float NiOverNt;
		Attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 Refracted;
		float ReflectProb;
		float Cosine;
		if (Dot(RIn.Direction(), Rec.Normal) > 0) {
			OutwardNormal = -Rec.Normal;
			NiOverNt = RefIdx;
			Cosine = Dot(RIn.Direction(), Rec.Normal) / RIn.Direction().Vec3Length();
		}
		else {
			OutwardNormal = Rec.Normal;
			NiOverNt = 1.0 / RefIdx;
			Cosine = -Dot(RIn.Direction(), Rec.Normal) / RIn.Direction().Vec3Length();
		}

		if (Refract(RIn.Direction(), OutwardNormal, NiOverNt, Refracted)) {

			ReflectProb = Schlick(Cosine, RefIdx);
		}
		else {
			ReflectProb = 1.0;
			Scattered = Ray(Rec.P, Reflected);
		}
		if (random_double(0.0, 1.0) < ReflectProb) {
			Scattered = Ray(Rec.P, Reflected);
		}
		else {
			Scattered = Ray(Rec.P, Refracted);
		}
		return true;
	}



	float RefIdx;
};
