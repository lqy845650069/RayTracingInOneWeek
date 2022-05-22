#pragma once
#include<memory>
#include<vector>
#include"Hitable.h"
using std::shared_ptr;
using std::make_shared;

class HitableList: public Hitable{
public:
	HitableList(){}
	HitableList(shared_ptr<Hitable> object) { add(object); }

	void clear(){}
	void add(shared_ptr<Hitable> object) { objects.push_back(object); }

	inline virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const;
	
	std::vector<shared_ptr<Hitable>> objects;

};

inline bool HitableList::Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const
{
	HitRecord TempRec;
	bool HitAimageWidththing = false;
	float ClosetSoFar = TMax;
	for (const auto& object: objects) {
		if (object->Hit(R, TMin, ClosetSoFar, TempRec)) {
			HitAimageWidththing = true;
			ClosetSoFar = TempRec.T;
			Rec = TempRec;
		}
	}
	return HitAimageWidththing;
}