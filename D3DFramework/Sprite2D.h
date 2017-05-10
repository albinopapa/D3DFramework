#pragma once

#include "Model2D.h"
#include "Size.h"
#include "Texture.h"
#include <memory>

class Graphics;
class Mat3;

class Sprite2D
{
public:
	Sprite2D( std::shared_ptr<Model2D> &pMod );
	virtual void Draw( const Mat3 &Transform, Graphics &Gfx ) = 0;
protected:
	std::shared_ptr<Model2D> m_pModel;
};

class Sprite2D_Color :public Sprite2D
{
public:
	Sprite2D_Color( const ColorF &Tint, std::shared_ptr<Model2D> &Mod );
	void Draw( const Mat3 &Transform, Graphics &Gfx )override;
};

class Sprite2D_Texture :public Sprite2D
{
public:
	Sprite2D_Texture( std::shared_ptr<Texture2D> &Tex, std::shared_ptr<Model2D> &Mod );
	void Draw( const Mat3 &Transform, Graphics &Gfx )override;

private:
	std::shared_ptr<Texture2D> m_pTexture;
};