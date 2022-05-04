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

};