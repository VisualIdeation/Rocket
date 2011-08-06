/*
 * BaseLocator
 *
 * Author: Patrick O'Leary
 * Created: June 3, 2010
 * Copyright 2010. All rights reserved.
 */
#ifndef BASELOCATOR_H_
#define BASELOCATOR_H_

/* Vrui includes */
#include <Vrui/LocatorToolAdapter.h>

// begin Forward Declarations
namespace Vrui {
class LocatorTool;
}
class Rocket;
// end Forward Declarations
class BaseLocator : public Vrui::LocatorToolAdapter {
public:
	BaseLocator(Vrui::LocatorTool* _locatorTool, Rocket* _rocket);
	~BaseLocator();
	virtual void highlightLocator(GLContextData& contextData) const;
	virtual void glRenderAction(GLContextData& contextData) const;
	virtual void glRenderActionTransparent(GLContextData& contextData) const;
private:
	Rocket* rocket;
};

#endif /*BASELOCATOR_H_*/
