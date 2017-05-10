#include "Sprite2D.h"
#include "Graphics.h"
#include "Mat3.h"
#include "Vec2.h"

Sprite2D::Sprite2D( std::shared_ptr<Model2D> &pMod )
	:
	m_pModel( pMod )
{

}

// Colored Sprite2D class
Sprite2D_Color::Sprite2D_Color( const ColorF &Tint, std::shared_ptr<Model2D> &pMod )
	:
	Sprite2D( pMod )
{

}

void Sprite2D_Color::Draw( const Mat3 &Transform, Graphics &Gfx )
{
	Gfx.DrawModel2D( *m_pModel, Transform );
}

// Textured Sprite2D class
Sprite2D_Texture::Sprite2D_Texture( std::shared_ptr<Texture2D> &Tex, std::shared_ptr<Model2D> &Mod )
	:
	Sprite2D( Mod ),
	m_pTexture( Tex )
{}

void Sprite2D_Texture::Draw( const Mat3 &Transform, Graphics &Gfx )
{
	Gfx.DrawModel2D( *m_pModel, *m_pTexture, Transform );
}