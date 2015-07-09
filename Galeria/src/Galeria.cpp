#include "Galeria.h"


Galeria::Galeria()
{
	/*mRectDimensiones = ci::Rectf( 0 , 0 , 100 , 100 );
	ancho = mRectDimensiones.getWidth();
	alto = mRectDimensiones.getHeight();
	xPos = mRectDimensiones.getX1();
	yPos = mRectDimensiones.getY1();*/
	Galeria( ci::Rectf(0, 0, 100, 100) );
}

Galeria::Galeria(ci::Rectf rect)
{
	mRectDimensiones	= rect;
	ancho				= mRectDimensiones.getWidth();
	alto				= mRectDimensiones.getHeight();
	xPos				= mRectDimensiones.getX1();
	yPos				= mRectDimensiones.getY1();
}


void Galeria::setup()
{

};

void Galeria::cargarXml(std::string path)
{
	//Si no exsite el archivo en assets no cargamos
	if (ci::app::getAssetPath(path).empty())
	{
		ci::app::console() << "No se pudeo cargar archivo: " << path << std::endl;
		return;
	}

	ci::XmlTree doc( ci::app::loadAsset(path) );
	//No aseguramos que exista el nodo <galeria>
	if (doc.hasChild("galeria"))
	{
		//Se crea el nodo xml con el contenido de <galeria>
		ci::XmlTree galeriaXml = doc.getChild("galeria");
		
		//CArgamos con el otro método, ya que tenemos el nodo <galeria>
		cargarXml(galeriaXml);
		
	}
};

void Galeria::cargarXml(ci::XmlTree nodo)
{
	//El nodo en este punto debe corresponde a <galeria> y o alguno otro que contenga hijos del tipo <foto>

	//VAriables para asignar posiciones y dimensiones iniciales a las fotos
	int deltaX = 0;

	//Iteramos sobre los hijos de galeria usando auto
	for (auto& foto : nodo.getChildren())
	{
		//Hay que saegurarse de que es un noto <foto>
		if ( foto->getTag() == "foto")
		{
			std::string path = foto->getChild("path").getValue();

			ci::Rectf rect = ci::Rectf(xPos + deltaX, yPos, xPos + deltaX + ancho, yPos + alto);
			ci::gl::TextureRef texture = ci::gl::Texture::create( ci::loadImage( ci::app::loadAsset( path ) ) );
			FotoRef nuevaFoto = FotoRef( new Foto( rect, texture ) );
			mFotos.push_back( nuevaFoto );
			deltaX += ancho;
		}
	}
};

void Galeria::mouseDown(ci::app::MouseEvent &event)
{

};

void Galeria::update()
{

};

void Galeria::draw()
{
	//Pintar todas las fotos
	for (auto& foto : mFotos)
	{
		foto->draw();
	}
};

void Foto::draw()
{
	ci::gl::draw(mTexture, mRect);
};

void Foto::asginarRect(ci::Rectf rect)
{
	mRect = rect;
};

void Foto::mover(float x)
{
	auto ancho = mRect.getWidth();
	mRect.x1 = x;
	mRect.x2 = x + ancho;
};
