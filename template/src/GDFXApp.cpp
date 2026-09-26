//-----------------------------------------------------------------------------
// Copyright (c) 2026 GDFX Authors
//-----------------------------------------------------------------------------

#include "GDFXApp.hpp"

CREATE_GAME(GDFXApp)

GDFXApp::GDFXApp() :
	Game("GDFXApp", "cc.gdfx.GDFXApp", "1.0.0", 640, 360)
{
	logo = content.load<Texture>("data/logo.png");
	font = content.load<Font>("data/font-8x8.png");
	x = y = 10;
	bx = getWidth() / 2;
	by = getHeight() / 2;
	xdir = -1;
	ydir = -1;
}

GDFXApp::~GDFXApp()
{
}

void GDFXApp::update(float delta)
{
	if (x++ > getWidth())
		x -= getWidth();
	if (y++ > getHeight())
		y -= getHeight();

	bx += xdir;
	by += ydir;

	if (xdir > 0) {
		if (bx > getWidth() - logo->getWidth()) {
			xdir *= -1;
		}
	}
	else {
		if (bx < 0) {
			xdir *= -1;
		}
	}

	if (ydir > 0) {
		if (by > getHeight() - logo->getHeight()) {
			ydir *= -1;
		}
	}
	else {
		if (by < 0) {
			ydir *= -1;
		}
	}
}

void GDFXApp::draw(Graphics& g)
{
	g.setColor(0, 32, 192);
	g.clear();

	g.drawTexture(*logo, bx, by);

	g.setColor(Color::C64::WHITE);
	g.drawText(font.get(), x, y, "Hello World!");

	g.setColor(Color::C64::YELLOW);
	g.drawFilledRect(40, 40, 60, 60);
	g.drawTextRight(font.get(), getWidth(), 10, "GDFX GAME FRAMEWORK");

	g.setColor(Color::C64::CYAN);
	g.drawTextCentered(font.get(), getWidth()/2, getHeight()-font->getSize()*2, "(C) 2026 GDFX Authors");
}
