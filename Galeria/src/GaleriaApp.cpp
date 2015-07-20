#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Galeria.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static void prepareSettings(App::Settings *settings)
{
	settings->setFrameRate(30);
	settings->setFullScreen(true);
	settings->setBorderless(true);

	settings->setWindowPos(0, 0);
	settings->setFrameRate(60);
	//settings->setWindowSize(1366, 768);
	//settings->setAlwaysOnTop(true);
	settings->setTitle("Telemática");


}


class GaleriaApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void keyUp( KeyEvent event) override;
	void update() override;
	void draw() override;

	Galeria		mGaleria;
};

void GaleriaApp::setup()
{
	getWindowBounds();
	mGaleria = Galeria(getWindowBounds() );
	mGaleria.cargarXml("galeria.xml");
	mGaleria.setup();
}

void GaleriaApp::mouseDown( MouseEvent event )
{
	mGaleria.mouseDown(event);
}

void GaleriaApp::keyUp(KeyEvent event)
{
	if (event.getCode() == KeyEvent::KEY_ESCAPE)
	{
		quit();
	}
}

void GaleriaApp::update()
{
	mGaleria.update();
}

void GaleriaApp::draw()
{
	gl::clear(Color(0, 0, 0));
	mGaleria.draw();;
}

CINDER_APP(GaleriaApp, RendererGl, prepareSettings)
