/*
 * ClippingPlaneLocator.cpp - Class for clipping plane locator.
 *
 * Author: Patrick O'Leary
 * Created: June 3, 2010
 * Copyright: 2010. All rights reserved.
 */
#ifndef CLIPPINGPLANELOCATOR_H_
#define CLIPPINGPLANELOCATOR_H_

#include <ANALYSIS/BaseLocator.h>
#include <Rocket.h>

/* Vrui includes */
#include <Vrui/Tools/LocatorTool.h>

// Begin forward declarations
class ClippingPlane;
// End forward declarations
class ClippingPlaneLocator : public BaseLocator {
public:
	ClippingPlaneLocator(Vrui::LocatorTool* locatorTool,
			Rocket * rocket);
	~ClippingPlaneLocator(void);
	virtual void buttonPressCallback(
			Vrui::LocatorTool::ButtonPressCallbackData* callbackData);
	virtual void buttonReleaseCallback(
			Vrui::LocatorTool::ButtonReleaseCallbackData* callbackData);
	virtual void motionCallback(
			Vrui::LocatorTool::MotionCallbackData* callbackData);
private:
	ClippingPlane * clippingPlane;
};

#endif /*CLIPPINGPLANELOCATOR_H_*/
