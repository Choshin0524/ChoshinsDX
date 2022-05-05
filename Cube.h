#pragma once
#include "IndexedTriangleList.h"

class Cube
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		namespace dx = DirectX;

		constexpr float side = 1.0f;
		std::vector<dx::XMFLOAT3> vertices;
		vertices.emplace_back(side, side, side); // 0
		vertices.emplace_back(side, -side, side); // 1
		vertices.emplace_back(-side, -side, side); // 2
		vertices.emplace_back(-side, side, side); // 3
		vertices.emplace_back(side, -side, -side); // 4
		vertices.emplace_back(-side, -side, -side); // 5
		vertices.emplace_back(-side, side, -side); // 6
		vertices.emplace_back(side, side, -side); // 7

		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
		}
		return{
			std::move(verts),
			{
				6,7,4, 6,4,5,
				0,2,1, 0,3,2,
				6,5,2, 6,2,3,
				7,0,1, 7,1,4,
				6,3,0, 6,0,7,
				5,1,2, 5,4,1
			}
		};
	}
	template<class V>
	static IndexedTriangleList<V> MakeSkinned()
	{
		namespace dx = DirectX;
		constexpr float side = 1.0f;
		std::vector<V> vertices(14);
		dx::XMStoreFloat2(&vertices[0].tex, { 2.0f / 3.0f, 0.0f });
		dx::XMStoreFloat2(&vertices[1].tex, { 1.0f / 3.0f, 0.0f });
		dx::XMStoreFloat2(&vertices[2].tex, { 2.0f / 3.0f, 1.0f / 4.0f });
		dx::XMStoreFloat2(&vertices[3].tex, { 1.0f / 3.0f, 1.0f / 4.0f });
		dx::XMStoreFloat2(&vertices[4].tex, { 2.0f / 3.0f, 3.0f / 4.0f });
		dx::XMStoreFloat2(&vertices[5].tex, { 1.0f / 3.0f, 3.0f / 4.0f });
		dx::XMStoreFloat2(&vertices[6].tex, { 2.0f / 3.0f, 2.0f / 4.0f });
		dx::XMStoreFloat2(&vertices[7].tex, { 1.0f / 3.0f, 2.0f / 4.0f });
		dx::XMStoreFloat2(&vertices[8].tex, { 2.0f / 3.0f, 1.0f });
		dx::XMStoreFloat2(&vertices[9].tex, { 1.0f / 3.0f, 1.0f });
		dx::XMStoreFloat2(&vertices[10].tex, { 3.0f / 3.0f, 1.0f / 4.0f });
		dx::XMStoreFloat2(&vertices[11].tex, { 3.0f / 3.0f, 2.0f / 4.0f });
		dx::XMStoreFloat2(&vertices[12].tex, { 0.0f, 1.0f / 4.0f });
		dx::XMStoreFloat2(&vertices[13].tex, { 0.0f, 2.0f / 4.0f });
		dx::XMStoreFloat3(&vertices[0].pos, { -side, -side, -side });
		dx::XMStoreFloat3(&vertices[1].pos, { side, -side, -side });
		dx::XMStoreFloat3(&vertices[2].pos, { -side, side, -side });
		dx::XMStoreFloat3(&vertices[3].pos, { side, side, -side });
		dx::XMStoreFloat3(&vertices[4].pos, { -side, -side, side });
		dx::XMStoreFloat3(&vertices[5].pos, { side, -side, side });
		dx::XMStoreFloat3(&vertices[6].pos, { -side, side, side });
		dx::XMStoreFloat3(&vertices[7].pos, { side, side, side });
		dx::XMStoreFloat3(&vertices[8].pos, { -side, -side, -side });
		dx::XMStoreFloat3(&vertices[9].pos, { side, -side, -side });
		dx::XMStoreFloat3(&vertices[10].pos, { -side, -side, -side });
		dx::XMStoreFloat3(&vertices[11].pos, { -side, -side, side });
		dx::XMStoreFloat3(&vertices[12].pos, { side, -side, -side });
		dx::XMStoreFloat3(&vertices[13].pos, { side, -side, side });
		return {
			std::move(vertices),{
				0,2,1, 2,3,1,
				2,6,3, 6,7,3,
				6,4,7, 4,5,7,
				4,8,5, 8,9,5,
				10,11,2, 11,6,2,
				3,7,12, 7,13,12
			}
		};
	}
};