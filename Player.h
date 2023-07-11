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
	//�����蔻��
	void OnCollision(const CollisionInfo& info)override;
};

