#pragma once

#include "Vertex2D.h"
#include "Size.h"
#include <vector>

class Model2D
{
public:
	Model2D() = default;
	Model2D( const std::vector<Vertex2D> &Vertices );
	Model2D( std::vector<Vertex2D> &&Vertices );

	static Model2D CreateTriangle( const Vec2f& Center, const SizeF& Size, const ColorF &C );
	static Model2D CreateSquare(   const Vec2f& Center, const SizeF& Size, const ColorF &C );

	const std::vector<Vertex2D> &GetVertices()const;
private:
	std::vector<Vertex2D> m_vertices;
};

