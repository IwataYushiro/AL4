#pragma once
#include "Object3d.h"

class Player:public Object3d
{
public:
	//作成
	static Player* Create(Model* model = nullptr);
	//初期化
	bool Initialize()override;
	//更新
	void Update()override;
	//Particle
	void DrawParticle(ID3D12GraphicsCommandList* cmdList);
	//当たり判定
	void OnCollision(const CollisionInfo& info)override;
};

