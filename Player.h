#pragma once
#include "Object3d.h"

class Player:public Object3d
{
public:
	//�쐬
	static Player* Create(Model* model = nullptr);
	//������
	bool Initialize()override;
	//�X�V
	void Update()override;
	//Particle
	void DrawParticle(ID3D12GraphicsCommandList* cmdList);
	//�����蔻��
	void OnCollision(const CollisionInfo& info)override;
};

