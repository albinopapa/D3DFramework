#pragma once

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	bool KeyPressed( unsigned char Key )const;
	bool KeyReleased( unsigned char Key )const;
	void OnKeyPress( unsigned char Key );
	void OnKeyRelease( unsigned char Key );

private:
	bool m_keys[ 256 ]{};
};

