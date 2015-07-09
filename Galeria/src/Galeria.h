/*
	Galería de imágenes
*/
#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Xml.h"

class Foto
{
public:
	Foto(ci::Rectf rect, ci::gl::TextureRef texture) :mRect(rect), mTexture(texture){};
	void draw();
	void asginarRect(ci::Rectf rect);
	void mover(float x);
	ci::Rectf mRect;
	ci::gl::TextureRef mTexture;
};

typedef	std::shared_ptr<Foto> FotoRef;

class Galeria
{
public:
	//Por default se crea un constructor que no hace nada
	Galeria();
	//Se construye con una rectángulo que es el área donde se pintará la galería
	Galeria(ci::Rectf rect);

	void setup();
	//Dos funciones para cargar la galería, la primera útil si sólo se carga un xml y la segunda por si el xml contiene varias galerias.
	void cargarXml( std::string path );//path dentro de la carpeta assets
	void cargarXml( ci::XmlTree nodo );
	void mouseDown( ci::app::MouseEvent &event );

	void update();
	void draw();

private:
	//Contiene las imágenes
	std::vector< ci::gl::TextureRef >	mImagenes;
	std::vector< FotoRef >				mFotos;

	//Dimensiones
	ci::Rectf		mRectDimensiones;
	int				ancho,
					alto,
					xPos,
					yPos;

};

