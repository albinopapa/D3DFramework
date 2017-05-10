#include "Model2D.h"


Model2D::Model2D( const std::vector<Vertex2D> &Vertices )
	:
	m_vertices( Vertices )
{}

Model2D::Model2D( std::vector<Vertex2D> &&Vertices )
	:
	m_vertices( std::move( Vertices ) )
{}

Model2D Model2D::CreateTriangle( const Vec2f& Center, const SizeF& Size, const ColorF & C )
{
	const auto halfSize = Size * .5f;
	Vertex2D top, botL, botR;
	top.position = { Center.x, Center.y - halfSize.height };
	top.texcoord = { .5f, 0.f };
	top.tint = C;

	botR.position = { Center.x + halfSize.width, Center.y + halfSize.height };
	botR.texcoord = { 1.f, 1.f };
	botR.tint = C;

	botL.position = { Center.x - halfSize.width, Center.y + halfSize.height };
	botL.texcoord = { 0.f, 1.f };
	botL.tint = C;
	
	return Model2D( { { top }, { botR }, { botL } } );
}

Model2D Model2D::CreateSquare( const Vec2f& Center, const SizeF& Size, const ColorF & C )
{
	const auto halfSize = Size * .5f;
	Vertex2D topL, topR, botL, botR;
	ColorF color = C;
	topL.position = { Center.x - halfSize.width, Center.y - halfSize.height };
	topL.texcoord = { 0.f, 0.f };
	topL.tint = color;

	topR.position = { Center.x + halfSize.width, Center.y - halfSize.height };
	topR.texcoord = { 1.f, 0.f };
	topR.tint = color;

	botR.position = { Center.x + halfSize.width, Center.y + halfSize.height };
	botR.texcoord = { 1.f, 1.f };
	botR.tint = color;

	botL.position = { Center.x - halfSize.width, Center.y + halfSize.height };
	botL.texcoord = { 0.f, 1.f };
	botL.tint = color;

	return Model2D( { 
		{ topL }, { topR }, { botL },
		{ botL }, { topR }, { botR } 
	} );
}

const std::vector<Vertex2D> &Model2D::GetVertices()const
{
	return m_vertices;
}