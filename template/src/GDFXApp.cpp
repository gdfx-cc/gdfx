//-----------------------------------------------------------------------------
// Copyright (c) 2026 GDFX Authors
//-----------------------------------------------------------------------------

#include "GDFXApp.hpp"

CREATE_GAME(GDFXApp)

GDFXApp::GDFXApp() :
	Game("GDFXApp", "cc.gdfx.GDFXApp", "1.0.0", 320, 200)
{
	logo = content.load<Texture>("data/logo.png");
}

GDFXApp::~GDFXApp()
{
}

void GDFXApp::update(float delta)
{
}

void GDFXApp::draw(Graphics& g)
{
	g.clear();
	g.drawTexture(*logo, 0, 0);
}
