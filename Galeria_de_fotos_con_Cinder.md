Galeria de fotos con Cinder
===========================

En la época dorada de Flash los primeros tutoriales que se leía eran para aprender a hacer galerías de fotos. Incluso los primeros scripts y fla's que se vendieron fueron este tipo de scripts. Ahora tengo que hacer una galeria sencilla para el estudio y pensé en documentar el proceso para armar un tutorial, por los viejos tiempos.

Para entender mejor el uso de imágenes en Cinder se puede leer el  tutorial de [Joshua Noble] (http://thefactoryfactory.com) [Images in Cinder](http://www.creativeapplications.net/tutorials/images-in-cinder-tutorials-cinder/). En este caso las imágenes irán directo al GPU para que OpenGL las pinte, y no les haremos nada pixel por pixel de tal forma que se usarán objetos del tipo `ci::gl::Texture`, o mejor dicho referencias a este tipo de objetos `ci::gl::TextureRef`.

Usaremos la versión glNext de Cinder, que ya está en la rama master del [github](https://github.com/cinder/Cinder).

##Creando los objetos 
Vamos a crear una clase 'Foto' para contener la textura y el rectángulo a dónde se va a pintar.

Luego escribimos la clase `Galeria` donde incluimos la cabeceras de cinder que vamos a ocupar.

	#pragma once
	#include "cinder/app/App.h"
	#include "cinder/app/RendererGl.h"
	#include "cinder/gl/gl.h"

Luego declaramos la clase:

	class Galeria{
	public:
	...
	};
No necesitamos constructor, usaremos una función setup() que és como acostumbro.
