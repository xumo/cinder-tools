///////////////////////////////////////////////////////////////
// Este proyecto muestra como filtrar las distancias del sensosor kinect 2
// Usa OpenCV
//
// Rodrigo Torres @xumerio
//
////////////////////////////////////////////////////////////////


#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/params/Params.h"
#include "cinder/Utilities.h"

#include "Kinect2.h"
#include "CinderOpenCv.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class KinectCinder : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;

	Kinect2::DeviceRef			mDevice;
	ci::Channel8uRef			mChannelBodyIndex;
	ci::Channel16uRef			mChannelDepth;
	gl::TextureRef				mDepthTex, 
								mDepthNearFilterTex,
								mDepthFarFilterTex,
								mDepthFilteredTex;
	int							mNearThreshold,
								mFarThreshold,
								mRadiusThreshold;

	params::InterfaceGlRef		mParams;
	std::vector<std::vector<cv::Point> > contours;
	vector<cv::Vec4i>			hierarchy;

	bool findBlobs;
};

void KinectCinder::setup()
{
	findBlobs = false;
	mDevice = Kinect2::Device::create();
	mDevice->start();

	mDevice->connectBodyIndexEventHandler([&](const Kinect2::BodyIndexFrame& frame)
	{
		mChannelBodyIndex = frame.getChannel();
	});
	
	mDevice->connectDepthEventHandler([&](const Kinect2::DepthFrame& frame)
	{
		mChannelDepth = frame.getChannel();
	});	

	mNearThreshold	= 10;
	mFarThreshold	= 200;

	mParams = params::InterfaceGl::create( getWindow(), "Filtro de distancias", toPixels( ivec2( 200, 300 ) ) );
	mParams->addParam("Near Threshold", &mNearThreshold).min(0).max(255).keyIncr("+").keyDecr("-").step(1);
	mParams->addParam("Far Threshold", &mFarThreshold).min(0).max(255).keyIncr("s").keyDecr("a").step(1);
	//mParams->addParam("Cube Color", &mRadiusThreshold).min(0).max(10).keyIncr("x").keyDecr("z").step(1);

}

void KinectCinder::mouseDown( MouseEvent event )
{
}

void KinectCinder::keyDown(KeyEvent event)
{
	if (event.getCode() == event.KEY_ESCAPE){
		quit();
	}
	else if (event.getCode() == event.KEY_SPACE){
		findBlobs = !findBlobs;
	}
}

void KinectCinder::update()
{
	if (mChannelDepth) {
		
		cv::Mat input(toOcv(*Kinect2::channel16To8(mChannelDepth))), outputNear, outputFar, composite;
		cv::threshold(input, outputNear, mNearThreshold, 255, cv::ThresholdTypes::THRESH_TOZERO);
		cv::threshold(input, outputFar, mFarThreshold, 255, cv::ThresholdTypes::THRESH_TOZERO_INV);
		cv::ThresholdTypes;
		mDepthNearFilterTex		= gl::Texture::create(fromOcv(outputNear), gl::Texture::Format().loadTopDown());
		mDepthFarFilterTex		= gl::Texture::create(fromOcv(outputFar), gl::Texture::Format().loadTopDown());
		mDepthTex = gl::Texture::create(*Kinect2::channel16To8(mChannelDepth));

		cv::bitwise_and( outputFar , outputNear , composite );

		cv::threshold(composite, composite, 0, 255, cv::ThresholdTypes::THRESH_BINARY);

		mDepthFilteredTex = gl::Texture::create(fromOcv(composite), gl::Texture::Format().loadTopDown());
		
		
		vector<cv::Vec4i> hierarchy;
		if ( findBlobs)
			cv::findContours(composite, contours , hierarchy , CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	}
}

void KinectCinder::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::color(1, 1, 1);
	if (mDepthTex) {
		gl::draw(mDepthTex, Rectf( 20 , 20 , 400 , 300 ) );
	}

	if( mDepthNearFilterTex ){
		gl::draw( mDepthNearFilterTex , Rectf(420, 20, 820, 300) );
	}

	if (mDepthFarFilterTex){
		gl::draw(mDepthFarFilterTex, Rectf(840, 20, 1240, 300) );
	}

	if (mDepthFilteredTex){
		gl::draw(mDepthFilteredTex, Rectf(20, 320, 400, 620) );
	}
	if (findBlobs){
		gl::pushMatrices();
		gl::translate(vec2(420, 320));
		for (auto iter = contours.begin(); iter != contours.end(); ++iter)
		{
			//glBegin(GL_LINE_LOOP);
			gl::begin(GL_LINE_LOOP);
			for (auto edge = iter->begin(); edge != iter->end(); ++edge)
			{
				gl::color(Color(CM_HSV, 0.2f, 0.3f, Rand::randFloat(1.0f)));
				gl::vertex(fromOcv(*edge));
			}
			gl::end();
			//glEnd();
		}
		gl::popMatrices();
	}
	//gl::drawString("FPS: "+toStrig( getFramerate()))
	toString( getFrameRate() );
	gl::drawString( "Fps " + toString( getFrameRate() ), vec2(500, 20 ) );
	mParams->draw();
}

CINDER_APP(KinectCinder, RendererGl, [](App::Settings* settings)
{
	settings->prepareWindow(Window::Format().size(1280, 640).title("Filtro de distancias"));
	settings->setFrameRate(60.0f);
})
