#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Galeria.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GaleriaApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	Galeria		mGaleria;
};

void GaleriaApp::setup()
{
	mGaleria = Galeria( Rectf( 100,  100, 500 , 400 ) );
	mGaleria.cargarXml("galeria.xml");

	mGaleria.setup();
	
}

void GaleriaApp::mouseDown( MouseEvent event )
{
	mGaleria.mouseDown( event );

}

void GaleriaApp::update()
{
	mGaleria.update();
}

void GaleriaApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	mGaleria.draw();
}

CINDER_APP( GaleriaApp, RendererGl )
