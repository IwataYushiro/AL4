#pragma once
#include "CollisionPrimitive.h"

//�����蔻��w���p�[
class Collision
{
public:
	/// <summary>
	/// ���ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="plane">����</param>
	/// <param name="inter">��_(���ʏ�̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool ChackSphere2Plane(const Sphere& sphere, const Plane& plane,
		DirectX::XMVECTOR* inter = nullptr);

private:

};
