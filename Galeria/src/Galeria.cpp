#include "Galeria.h"


Galeria::Galeria()
{
	Galeria( ci::Rectf(0, 0, 100, 100) );
}

Galeria::Galeria(ci::Rectf rect)
{
	mRectDimensiones	= rect;
	ancho				= mRectDimensiones.getWidth();
	alto				= mRectDimensiones.getHeight();
	xPos				= mRectDimensiones.getX1();
	yPos				= mRectDimensiones.getY1();

	fotoActiva			= -1;

	ci::gl::Fbo::Format csaaFormat;
	csaaFormat.setSamples(4);
	csaaFormat.setCoverageSamples(8);
	mFbo = ci::gl::Fbo::create( ancho, alto , csaaFormat );

	//Cargamos las imágenes para los botones
	mBtnSiguiente	= ci::gl::Texture::create( ci::loadImage( ci::app::loadAsset("siguiente.png")) );
	mBtnPrevio		= ci::gl::Texture::create( ci::loadImage( ci::app::loadAsset("previo.png")));

	mRectSiguiente.set(
		xPos + ancho - mBtnSiguiente->getWidth(),
		yPos + ( alto - mBtnSiguiente->getHeight() ) * 0.5,
		xPos + ancho,
		yPos + (alto - mBtnSiguiente->getHeight() ) * 0.5 + mBtnSiguiente->getHeight()
					  );

	mRectPrevio.set(
		xPos,
		yPos + (alto - mBtnPrevio->getHeight() ) * 0.5,
		xPos + mBtnPrevio->getActualWidth(),
		yPos + (alto - mBtnPrevio->getHeight()) * 0.5 + mBtnPrevio->getHeight()
		);
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

			//ci::Rectf rect = ci::Rectf(xPos + deltaX, yPos, xPos + deltaX + ancho, yPos + alto);
			ci::Rectf rect = ci::Rectf( deltaX , 0 , deltaX + ancho , alto );
			ci::gl::TextureRef texture = ci::gl::Texture::create( ci::loadImage( ci::app::loadAsset( path ) ) );
			FotoRef nuevaFoto = FotoRef( new Foto( rect, texture ) );
			mFotos.push_back( nuevaFoto );
			deltaX += ancho;
		}
	}

	if ( mFotos.size() > 0 )
		fotoActiva = indice = 0;

	render();


};

void Galeria::mouseDown(ci::app::MouseEvent &event)
{
	//Necesitamos ver si el mouse toca un botón

	//Para ver si toca previo
	if ( mRectPrevio.contains(event.getPos()))
	{
		fotoActiva = fotoActiva > 0 ? fotoActiva - 1 : mFotos.size() - 1;
		ci::app::console() << "previo" << fotoActiva << std::endl;
	}

	//Para ver si toca a siguiente
	if ( mRectSiguiente.contains(event.getPos()))
	{
		fotoActiva = fotoActiva < mFotos.size() - 1 ? fotoActiva + 1 : 0;
		ci::app::console() << "siguiente" << fotoActiva << std::endl;
	}

};

void Galeria::update()
{
	//Si cambia de foto activa a mover el indice -> que mueve todas las fotos
	if ( indice != fotoActiva )
	{
		//Técnica muy comun para anima de un valor a otro
		//depende del framerate
		indice += (fotoActiva - indice) * 0.15;
		float deltaX = -indice * ancho;

		for ( auto& foto : mFotos )
		{
			foto->mover( deltaX );
			deltaX += ancho;
		}

		//Si no de hace esto nunca sale del loop, en parte porque son diferentes tipos
		//Ademas acortamos el tiempo en el se sigue calculando
		if ( abs(fotoActiva - indice) < 0.0005)
			indice = fotoActiva;

		render();
		ci::app::console() << "moviendo " << fotoActiva << " : " << indice << std::endl;;
	}
};

void Galeria::render()
{
	//Pintar todas las fotos
	mFbo->bindFramebuffer();

	//Primero ponemos el color del fondo de este buffer
	//Puedes tener canal alfa
	ci::gl::clear( ci::ColorA(0.6, 0.3,0.3,0) );
	
	//Ahora necesitamos establecer en OpenGL la condiciones correctas
	//para hacer el render, de otra manera su mueve el sistema 
	//de coordenadas y cambia relaciones de aspecto
	ci::gl::pushViewport();
	ci::gl::viewport(mFbo->getSize());
	ci::gl::pushMatrices();
	ci::gl::setMatricesWindowPersp(mFbo->getSize());
	//Se pintan las fotos
	for (auto& foto : mFotos)
	{
		foto->draw();
	}
	//Un borde cuadrado como con color 
	ci::gl::color( ci::Color( 0.9 , 0.9 , 0.9 ) );
	ci::gl::drawStrokedRect( mFbo->getBounds() , 2 );
	//Regresamos el color de OpenGL a blanco
	ci::gl::color( ci::Color::white() );

	//Se regreasn los estados de opengl a como estaban
	ci::gl::popMatrices();
	ci::gl::popViewport();
	
	//Terminar de usar el FBO
	mFbo->unbindFramebuffer();
}

void Galeria::draw()
{
	//Quiero que tenga transparencia
	ci::gl::enableAlphaBlending();
	ci::gl::draw(mFbo->getColorTexture() , mRectDimensiones);

	ci::gl::draw( mBtnSiguiente, mRectSiguiente );
	ci::gl::draw( mBtnPrevio, mRectPrevio );
	ci::gl::disableAlphaBlending();
};

Foto::Foto(ci::Rectf rect, ci::gl::TextureRef texture) :mRectContenedor(rect), mTexture(texture)
{
	float anchoImagen	= mTexture->getWidth();
	float altoImagen	= mTexture->getHeight();
	
	float altoDestino	= mRectContenedor.getHeight();
	float anchoDestino	= mRectContenedor.getWidth();
	mRect = ci::Rectf( 0,0,30,30 );
	/*
		Vamos a tener 4 casos dependiendo de las dimensiones:
		2 para el contenedor y 2 para la imagen
		El contenedor es verticalu horizontal
		La imagen es horizontal o vertical.

		La idea es que la imagen se ajuste al área del contenedor si deformarse.
	*/
	
	
	ci::app::console() << "anchoImagen: " << anchoImagen
		<< " altoImagen: " << altoImagen
		<< std::endl;

	//Imagen horizontal
	if (anchoImagen > altoImagen)
	{
		//ajustamos al ancho del contenedor
		nH = anchoDestino * ( altoImagen / anchoImagen );
		nW = anchoDestino;
			
		xPos = 0;
		yPos = (altoDestino - nH) * 0.5;

		//Si se desborda ajustar a los alto
		if (nH > altoDestino)
		{
			nH = altoDestino;
			nW = altoDestino * (anchoImagen / altoImagen );

			yPos = 0;
			xPos = (anchoDestino - nW) * 0.5;
		}
	}
	else
	{
		//Se ajusta a lo alto
		nH = altoDestino;
		nW = altoDestino * (anchoImagen / altoImagen);

		yPos = mRectContenedor.y1;
		xPos = (anchoDestino - nW) * 0.5;
		
		//Si se desborda ajustar a lo alto
		if (nW > anchoDestino)
		{
			nH = anchoDestino * (altoImagen / anchoImagen);
			nW = anchoDestino;

			xPos = 0;
			yPos = (altoDestino - nH) * 0.5;
		}
	}

	mRect.set(
		xPos + mRectContenedor.x1,
		yPos + mRectContenedor.y1,
		xPos + mRectContenedor.x1 + nW,
		yPos + mRectContenedor.y1 + nH
		);
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
	auto ancho = mRectContenedor.getWidth();
	auto ofsetX = mRectContenedor.x1 - mRect.x1;
	mRectContenedor.x1 = x;
	mRectContenedor.x2 = x + ancho;	

	mRect.set(
		xPos + mRectContenedor.x1,
		yPos + mRectContenedor.y1,
		xPos + mRectContenedor.x1 + nW,
		yPos + mRectContenedor.y1 + nH
		);
};
