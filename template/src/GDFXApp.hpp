//-----------------------------------------------------------------------------
// Copyright (c) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_APP_HPP
#define GDFX_APP_HPP

#include <gdfx/game/Game.hpp>
#include <gdfx/graphics/Graphics.hpp>

using namespace gdfx;

class GDFXApp : public Game {
public:
	GDFXApp();
	virtual ~GDFXApp();

	virtual void update(float delta) override;
	virtual void draw(Graphics& g) override;

private:
	std::shared_ptr<Texture> logo;
	std::shared_ptr<Font> font;
    int x, y;
    int bx, by;
    int xdir, ydir;
};

#endif // GDFX_APP_HPP
