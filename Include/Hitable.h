#pragma once
#include<memory>
#include"Ray.h"
#include"AABB.h"
using std::shared_ptr;

class Material;
class HitRecord {
public:
	float T; //Time
	Vec3 P; // Point Position
	Vec3 Normal; // Normal Vector
	shared_ptr<Material> MatPtr;
};

class Hitable {
public:
	virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const = 0;
	virtual bool BoundingBox(double t0, double t1, AABB& outputBox) const = 0;
};
