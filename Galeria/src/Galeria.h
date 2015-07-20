/*
	Galería de imágenes
	Simple Slider
*/
#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Xml.h"

class Foto
{
public:
	Foto(ci::Rectf rect, ci::gl::TextureRef texture);
	
	
	void draw();
	void asginarRect(ci::Rectf rect);
	void mover(float x);

	//Un rectangulo es donde se pintará, y el otro es donde estará contenido
	ci::Rectf mRect,
			  mRectContenedor;
	ci::gl::TextureRef mTexture;

	float nH, nW, xPos, yPos;
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
	void render();
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

	//Para controlar el cambio
	//Indice de la foto activa
	//std::size_t sería lo correcto pues es el tipo que se 
	//usa para contenedores std
	//http://googleprojectzero.blogspot.de/2015/07/when-int-is-new-short.html
	//std::size_t		fotoActiva;
	//Pero con int funciona
	int					fotoActiva;
	float				indice;

	//Para aplicar una "máscara" y que se vea sólo el área del rectangulo contenedor
	//usaremos un FBO http://libcinder.org/docs/v0.8.4/guide__gl___fbo.html
	ci::gl::FboRef			mFbo;

	//Variables para los botones
	//Imágenes 
	ci::gl::TextureRef		mBtnSiguiente,
							mBtnPrevio;
	//Rectángulos en dónde se vana pintar
	ci::Rectf				mRectSiguiente,
							mRectPrevio;
	
};

