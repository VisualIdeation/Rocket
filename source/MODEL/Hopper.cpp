/*
 * Hopper.cpp - Methods for Hopper facility.
 *
 * Author: Patrick O'Leary
 * Created: June 3, 2010
 * Copyright: 2010
 * Requirements: Delta3D 2.4.0 (Modified) and OSG 2.8.2
 */

/* System headers */
#include <iostream>

/* Application headers */
#include <SYNC/Guard.h>

/* Delta3D headers */
#include <dtCore/object.h>
#include <dtCore/camera.h>
#include <dtCore/scene.h>
#include <dtCore/system.h>
#include <dtCore/environment.h>

/* ODE headers */
#include <ode/ode.h>

/* Vrui Headers */
#include <Vrui/Vrui.h>

#include "Hopper.h"

static bool dophysics = false;
static double lastTime = 0.0f;
static double *simulationTime;

using namespace std;
using namespace dtCore;
using namespace dtABC;
using namespace dtUtil;

/*****************************************
 Methods of class Hopper::DataItem:
 *****************************************/

/****************************************************
 Constructors and Destructors of class DataItem:
 ****************************************************/
/*
 * DataItem constructor
 */
Hopper::DataItem::DataItem(void) {
} // end DataItem()

/*
 * ~DataItem destructor
 */
Hopper::DataItem::~DataItem(void) {
} // end ~DataItem()

/****************************************************
 Constructors and Destructors of class Hopper:
 ****************************************************/
/*
 * Hopper constructor
 */
Hopper::Hopper(void) :
		Application(true), drawMode(true), frameNumber(0) {

	hopper = this;

	// Tell osg::Referenced to use thread-safe reference counting. We do
	// this here for all applications and all configurations though it is
	// only strictly necessary when Vrui is configured to use
	// multi-threaded rendering.
	osg::Referenced::setThreadSafeReferenceCounting(false);

	/* Initialize update visitor */
	updateVisitor = new osgUtil::UpdateVisitor();
	frameStamp = new ::osg::FrameStamp();
	updateVisitor->setFrameStamp(frameStamp.get());
} // end Hopper()

/*
 * ~Hopper - destructor
 */
Hopper::~Hopper(void) {
} // end ~Hopper()

/*******************************
 Methods of class Hopper:
 *******************************/

/*
 * addObjects
 */
void Hopper::addObjects(void) {
	GetScene()->AddDrawable(europa.get());
} // end addObjects()

/*
 * createHopper
 */
void Hopper::createHopper(void) {
	europa = new Object("Hopper");
	europa->LoadFile("../data/rocket/europa.3ds");
} // end createHopper

/*
 * config
 */
void Hopper::config(void) {
	/* Load the model files */
	createHopper();

	addObjects();
} // end config()

/*
 * display
 *
 * parameter glContextData - GLContextData &
 */
void Hopper::display(GLContextData & glContextData) const {

	/* Get context data item: */
	DataItem* dataItem = glContextData.retrieveDataItem<DataItem> (this);

	dataItem->viewer->advance(lastFrameTime);
	if (!dataItem->viewer->done()) {
		dataItem->viewer->updateTraversal();
	}

	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	GLdouble mv[16], p[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, p);

	dataItem->viewer->getCamera()->setViewport(vp[0], vp[1], vp[2], vp[3]);
	dataItem->viewer->getCamera()->setProjectionMatrix(osg::Matrix(p));
	dataItem->viewer->getCamera()->setViewMatrix(osg::Matrix(mv));

	/* Render all opaque surfaces: */
	dataItem->viewer->renderingTraversals();

} // end display()

/*
 * frame
 */
void Hopper::frame(void) {
	/* Get the current application time: */
	double newFrameTime = Vrui::getApplicationTime();

	++frameNumber;

	// Update the frame stamp with information from this frame.
	frameStamp->setFrameNumber(frameNumber);
	frameStamp->setReferenceTime(newFrameTime);
	frameStamp->setSimulationTime(newFrameTime);

	// Set up the time and frame number so time-dependent things (animations,
	// particle system) function correctly.
	updateVisitor->setTraversalNumber(frameNumber);

	lastFrameTime = newFrameTime;
} // end frame()

/*
 * initContext
 *
 * parameter glContextData - GLContextData &
 */
void Hopper::initContext(GLContextData & glContextData) const {
	/* Create a new context data item: */
	DataItem* dataItem = new DataItem();

	// Create the osgViewer instance
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	// Set up osgViewer::GraphicsWindowEmbedded for this context
	osg::ref_ptr<osg::GraphicsContext::Traits> traits =
			new ::osg::GraphicsContext::Traits;
	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphicsWindow =
			new osgViewer::GraphicsWindowEmbedded(traits.get());
	viewer->getCamera()->setGraphicsContext(graphicsWindow.get());
	viewer->getCamera()->getGraphicsContext()->getState()->setContextID(
			osg::GraphicsContext::createNewContextID());

	viewer->getCamera()->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);

	viewer->getCamera()->setClearColor(::osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	viewer->getCamera()->setClearMask(0);

	osg::Group * root = new osg::Group();
	dataItem->root = root;
	root->setName("Root");
	root->addChild(hopper->GetRootNode());

	// Add the tree to the viewer and set properties
	Guard<MutexPosix> viewerGuard(dataItem->viewerLock);
	viewer->setSceneData(root);

	dataItem->viewer = viewer;

	glContextData.addDataItem(this, dataItem);
} // end initContext()

/*
 * toggleHopper
 */
void Hopper::toggleHopper(void) {
	europa.get()->DeltaDrawable::SetActive(
			!europa.get()->DeltaDrawable::GetActive());
} // end toggleHopper()

/*
 * toggleWireframe
 */
void Hopper::toggleWireframe(void) {
	if (drawMode) {
		GetScene()->SetRenderState(dtCore::Scene::FRONT_AND_BACK, dtCore::Scene::LINE);
		drawMode = false;
	} else {
		GetScene()->SetRenderState(dtCore::Scene::FRONT, dtCore::Scene::FILL);
		drawMode = true;
	}
} // end toggleWireframe()
