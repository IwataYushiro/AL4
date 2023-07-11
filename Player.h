#pragma once
#include "Object3d.h"

class Player:public Object3d
{
public:
	//ì¬
	static Player* Create(Model* model = nullptr);
	//‰Šú‰»
	bool Initialize()override;
	//XV
	void Update()override;
	//“–‚½‚è”»’è
	void OnCollision(const CollisionInfo& info)override;
};

