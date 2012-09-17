/*
Copyright © 2011-2012 Clint Bellanger
Copyright © 2012 Igor Paliychuk

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

#include "MenuConfirm.h"
#include "Settings.h"
#include "SharedResources.h"

using namespace std;


MenuConfirm::MenuConfirm(const string& _buttonMsg, const string& _boxMsg) : Menu() {
	confirmClicked = false;
	hasConfirmButton = true;
	if (_buttonMsg == "") hasConfirmButton = false;
	// Text to display in confirmation box
	boxMsg = _boxMsg;

	if (hasConfirmButton) {
		buttonConfirm = new WidgetButton(mods->locate("images/menus/buttons/button_default.png"));
		buttonConfirm->label = _buttonMsg;
	}

	buttonClose = new WidgetButton(mods->locate("images/menus/buttons/button_x.png"));

	loadGraphics();
}

void MenuConfirm::loadGraphics() {
	background = IMG_Load(mods->locate("images/menus/confirm_bg.png").c_str());
	if(!background) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
		exit(1);
	}
	
	// optimize
	SDL_Surface *cleanup = background;
	background = SDL_DisplayFormatAlpha(background);
	SDL_FreeSurface(cleanup);
}

void MenuConfirm::update() {
	if (hasConfirmButton) {
		buttonConfirm->pos.x = VIEW_W_HALF - buttonConfirm->pos.w/2;
		buttonConfirm->pos.y = VIEW_H/2;
		buttonConfirm->refresh();
		label.set(window_area.x + window_area.w/2, window_area.y + window_area.h - (buttonConfirm->pos.h * 2), JUSTIFY_CENTER, VALIGN_TOP, boxMsg, font->getColor("menu_normal"));
	} else {
		label.set(window_area.x + window_area.w/2, window_area.y + (window_area.h / 4), JUSTIFY_CENTER, VALIGN_TOP, boxMsg, font->getColor("menu_normal"));
	}

	buttonClose->pos.x = window_area.x + window_area.w;
	buttonClose->pos.y = window_area.y;
}

void MenuConfirm::logic() {
	if (visible && hasConfirmButton) {
		if(buttonConfirm->checkClick() || (inpt->pressing[ACCEPT] && !inpt->lock[ACCEPT])) {
			inpt->lock[ACCEPT] = true;
			confirmClicked = true;
		}
		if(buttonClose->checkClick() || (inpt->pressing[CANCEL] && !inpt->lock[CANCEL])) {
			inpt->lock[CANCEL] = true;
			visible = false;
		}
	} else if (visible && !hasConfirmButton) {
		if(buttonClose->checkClick() || (inpt->pressing[CANCEL] && !inpt->lock[CANCEL])) {
			inpt->lock[CANCEL] = true;
			visible = false;
		}
	}
}

void MenuConfirm::render() {
	SDL_Rect src;

	// background
	src.x = 0;
	src.y = 0;
	src.w = window_area.w;
	src.h = window_area.h;
	SDL_BlitSurface(background, &src, screen, &window_area);

	label.render();

	if (hasConfirmButton) buttonConfirm->render();
	buttonClose->render();
}

MenuConfirm::~MenuConfirm() {
	if (hasConfirmButton) delete buttonConfirm;
	delete buttonClose;
	SDL_FreeSurface(background);
}
