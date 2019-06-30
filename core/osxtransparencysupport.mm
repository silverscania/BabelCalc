// Copyright (C) 2017 James Clark <james@bitsofbeards.co.uk>
//
// This file is part of BabelCalc, a calculator that operates on
// multiple numerical bases simultaneously.
//
// BabelCalc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// BabelCalc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with BabelCalc.  If not, see <http://www.gnu.org/licenses/>.

#include "osxtransparencysupport.h"

#include <QWindow>
#include <Cocoa/Cocoa.h>
#include <iostream>
//#include <qnsview>

OSXTransparencySupport::OSXTransparencySupport(QWidget *parent)
	:QObject(parent)
{
	NSView *nsview = (NSView *)parent->winId();
	NSWindow *nswindow = [nsview window];

	nswindow.appearance = [NSAppearance appearanceNamed:NSAppearanceNameVibrantDark];
	nswindow.titlebarAppearsTransparent = true;

	nswindow.styleMask |= NSFullSizeContentViewWindowMask |
						 NSClosableWindowMask |
						 NSResizableWindowMask;

	//nswindow.movableByWindowBackground = true;

	static const NSRect frameRect = {
		{ 0.0, 0.0 } ,
		{ static_cast<CGFloat>(parent->width()/2), static_cast<CGFloat>(parent->width()/2) }
	};
	NSVisualEffectView * vibrant  = [[NSVisualEffectView alloc] initWithFrame:frameRect];
	vibrant.material = NSVisualEffectMaterialDark;//Dark,MediumLight,PopOver,UltraDark,AppearanceBased,Titlebar,Menu
	vibrant.blendingMode = NSVisualEffectBlendingModeBehindWindow;//I think if you set this to WithinWindow you get the effect safari has in its TitleBar. It should have an Opaque background behind it or else it will not work well
	vibrant.state = NSVisualEffectStateActive;//FollowsWindowActiveState,Inactive

	NSView* qtView = nswindow.contentView;
	[qtView setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];

	nswindow.contentView = vibrant;
	[vibrant addSubview:qtView positioned:NSWindowBelow relativeTo:nil];
}
