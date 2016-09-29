Apuntes sobre Cinder
====================

Cinder es junto a Openframeworks una de la mejores herramientas disponibles para producir interactivos multimedia con C++ y OpenGL. A diferencia de OF, Cinder se ha esmerado en utilizar el paradigma que impulsa la biblioteca standart std, C++ 11 y el uso de boost. De esta manera la curva de aprendizaje es un poco mayor se se es ajeno al tortuoso mundo de C++, pero vale la pena. En mi caso uso C++ cuando es necesario tener procesamiento o manejo de video en tiempo real y por supuesto 3D en sus diferentes facetas.


En esta serie de apuntes planeo hacer un recuento de conceptos básicos con los que me he encontrado. Casi todo está en la página de Cinder.


Apuntadores
-----------
En Cinder usan mucho los shared pointers std::shared_ptr, tienen muchas cualidades que desconozco pero sin duda son muy útiles.

```c++
typedef std::shared_ptr<Objeto> ObjetoRef;
```
Se le añade el sufijo Ref para denotar que es una referencia al objeto. Si le saca de onda, no lo pele.


Settings
--------
Al inicio del programa es muy útil definir ciertas propiedades, como si es fullscreen, pantalla de alta densidad, etc. y para ello hay una función propia de la clase App que se llama `prepareSettings` que puede ser estática y vivir fuera de la clase principal de nuestro programa. 

Así en nuestro .cpp principal, después de los headers se puede definir la función:

```c++

static void prepareSettings(App::Settings *settings)
{
    settings->setFullScreen(false);
	settings->setBorderless(true);
	settings->setWindowPos(20, 20);
	settings->setFrameRate(60);
	settings->setWindowSize(1024, 768);    
    settings->setAlwaysOnTop(true);
	settings->setTitle("El Planeta y su Futuro");
}

```

Aquí definimos que la aplicación viva en una ventana sin bordes y siempre esté sobre cualquier programa, algo de verdad molesto si no es lo que se quiere. Este propiedad puede ser muy útil pues en este punto Cinder a veces no soporta adecuadamente en Mac y a veces en Windows mandar a fullscreen en más de un pantalla, aún si se tiene activado el monitor extendido y en mac desabilidado el uso de spaces por separado.


Para aplicar esta configuración se debe ir al finale del cpp principal y aplicarlos en el macro que inicia el programa:

```c++
CINDER_APP(ApuntesApp, RendererGl, prepareSettings)
```

Cargar Imágenes, Videos y XML
-----------------------------
Si, ya sé. Cargar imágenes es casí lo único que está documentado, igual me tomo un par de intentos y después de un rato seguir viendo referencia así que mejor lo apunto.

Cinder tiene dos maneras para manejar archivos, aunque como estás en c++ puedes usar lo que se antoje ;): 
**Assets** y **Resources**.  

###Assets
Son archivos que viven en la carpeta assets, que Cinder busca en el nivel inmediato y no sé cuantos arriba ./ ../ ../../ etc.  y que se cargan en tiempo de ejecución.

La acción se realiza mediante la el método `ci::app::loadAsset("ruta/dentro/de/assets")` y regresa un referencia del tipo `DataSource`o bien `DataSourceRef`. Esta referencia es lo que se pasa a un cargador de imagen, video o xml.

###Cargar una imagen
Hay dos formas de cargar una imagen en Cinder: directamente a una textura de OpenGL y a un objecto Surface. El primero vivirá en un espacio de la memoria de la tarjeta de video (GPU) y el segundo vive en la memoria RAM a donde nuestro CPU tiene acceso.

Para cargar a textura una imagen localizada en la carpeta *assets*: 
```c++
//Carga simple
gl::TextureRef      texImagen;
 texImagen = gl::Texture::create( loadImage( getAssetPath( "fondo.jpg" ) ) );

//Cargando con opciones
texImagen = gl::Texture::create( loadImage( getAssetPath( "fondo.jpg" ) ) ,ci::gl::Texture::Format().minFilter( GL_LINEAR_MIPMAP_LINEAR ).magFilter( GL_LINEAR ).mipmap().wrap( GL_REPEAT )  );

//Pintarla
gl::draw( texImagen);

//Pintarla cambiando su tamaño y posición
gl::draw( texImagen , Rectf( 20 , 20 , 120 ,120 ) );

```

###Cargar un video
Para cargar un video se tiene que incluir las cabeceras de Quicktime, aunque parece que desde Cinder 9 ya te utiliza el AVFoundation para reproducir video. Después de cargarlo hay que llamar a su método ```play()``` para que pueda iniciar su reproducción.
Finalmente para pintarlo se llama a su método ```getTexture()``` para obtener la textura que generó.

```c++
#include "cinder/qtime/QuickTimeGl.h"
...
qtime::MovieGlRef   video;

video = qtime::MovieGl::create( getAssetPath( "nautilus.mp4" ) );
video->play();

gl::draw( video->getTexture() );

```




###Resources
Estos archivos viven en la carpeta resources, y se embeben en el ejecutable de la aplicación. Es sumamente útil si quiere cargar contenido una vez y estás seguro que no se cambiarán nunca más. 

El método que regresa de nuevo un `DataSourceRef` es `ci::app::loadResource( VARIABLE_DE_PREPROCESADOR )`. Como se vé aquí en vez de ruta hay una variable. Yo sé, que flojera.  Los resources se deben declara en el archivo `Resources.h`, mismo que hay que incluir en tu proyecto.

```c++
#include "Resources.h"
```
que contiene:
```c++
#pragma once
#include "cinder/CinderResources.h"
#define VARIABLE_DE_PREPROCESADOR		CINDER_RESOURCE( ../resources/, image_prueba.png, 1, IMAGE )
```
`CINDER_RESOURCE` acepta 4 parámetros:
- Ruta de la carpeta resource.
- Nombre del archivo.
- Id asociado al archivo, deber ser único.
- Tipo de archivo: IMAGE, VIDEO, XML

En mac se deben añadir los archivos al proyecto de XCode al arrastrarlos a la carpeta resources, o con click derecho en el proyecto y dando a "añadir archivo" o bien "add files".

En windows  está peor, hay que editar el archivo Resources.rc y añadir una línea con la variable:

```c++
#include "..\include\Resources.h"

VARIABLE_DE_PREPROCESADOR
```

###Assets
Existe una carpeta llamada "assets", y cualquier programa de Cinder la va a  buscar primero en su mismo nivel y hasta dos niveles arriba ../../ (me parece). Ahí es un buen lugar para poner los elementos que se van a cargar en tiempo de ejecución.

###Gráficos (Imágenes)
Cuando se pinta con la computadora una imagen, sus datos, pasan por dos partes: la memoria del CPU y la memoria del GPU, a veces son la misma. En Cinder se distinguen por dos tipos de objecto:

- Surface: se encrga de manejar los datos o pixeles de una imagen en el CPU. Con ella podemos iterar sobre el contenido de sus pixeles y leer su valor o asignarles uno.
- Texture: las texturas son los datos de la imagen ya en la memoria de la tartjesta gráfica (GPU). En nuestro caso Cinder usa OpenGL para toda la graficación por GPU.

En general una surface primero tiene que convertirse a texture para ser pintada por OpengGL, pero no es al revés: una textura puede no venir de una Surface.


##Surface

Primero se debe crear un objectos `ci::Surface` con parámetro de dimensiones y el tercero si se quiere que contenga información de alpha o transparencia:

```c++
ci::Surface mSur( ancho , alto, true);

//obien cargando una imagen
ci::Surface mFoto = ci::loadImage( ci::app::loadAsset( "foto.jpg" ) );

```
Ya después se le puede asignar un valor ,aleatorio por ejemplo, a cada pixel: 
```c++
ci::Surface::Iter iter = mSur.getIter();
    while( iter.line() ) {
        while( iter.pixel() ) {
            iter.r() = ci::randInt( 255 );
            iter.g() = ci::randInt( 255 );
            iter.b() = ci::randInt( 255 );
        }
    }
```
Nótese que Cinder hacer de contenedores de la biblioteca stándar para los datos de pixeles, por esa razón es mejor utilizar iteradores. Se recomienda leer acerca de los contenedores e iteradores de std en C++. 





###Texto
Para poner texto en pantalla hay varias maneras que generalmente terminan en *algo* que la *hace* y luego la *renderea* a textura y uno termina con una textura que puede pintar.  
Lo más sencillo es usar una `TextureFontRef` junto con un `Font` que puede usar tipografía instaladas en el sistema o bien cargar una en formato ttf.

Primero se declara las variables que usan:
```c++
ci::Font					debugFont;
ci::gl::TextureFontRef      debugTexture;

```

Luego se carga la fuente y se crea la textura que a pintar el texto:
```c++
debugFont = ci::Font("Arial", 24);
debugTexture = ci::gl::TextureFont::create(debugFont);
``

Finalmente en se puede pintar las string que sean cualquier posición de la pantalla, la posición en x es el lado izquierdo y y es la parte inferior del texto:
```c++
debugTexture->drawString("Hola" , ci::vec2(10, 10));
debugTexture->drawString("Mundo!!" , ci::vec2(10, 30));
```

###Textbox
Hay veces que se requiere un cuadro de texto con ajuste de línea, fondo con color, etc. Para eso exite el Textbox. En el ejemplo samples/TextBox podemos ver que se necesita una textura y una tipografía para usarla.

```c++
gl::TextureRef      mTextTexture;
vec2                mSize;
Font                mFont;
```
Donde `mSize` es el tamaño del cuadro de texto. Después de definir la tipografía como antes se puede crear el cuaro de texto. 

```
string txt = "Here is some text that is larger than can fit naturally inside of 100 pixels.\nAnd here is another line after a hard break.";
    TextBox tbox = TextBox().alignment( TextBox::RIGHT ).font( mFont ).size( ivec2( mSize.x, TextBox::GROW ) ).text( txt );
    tbox.setColor( Color( 1.0f, 0.65f, 0.35f ) );
    tbox.setBackgroundColor( ColorA( 0.5, 0, 0, 1 ) );
    ivec2 sz = tbox.measure();
    console() << "Height: " << sz.y << endl;
    mTextTexture = gl::Texture2d::create( tbox.render() );
```
Está muy chulo que después de crear el cuadro y antes de ponerlo en una textura se pueda tener las nuevas dimensiones del cuadro de texto, pues en este caso `TextBox::GROW` define que el alto se ajuste al largo del texto.


###Audio
Para utilizar audio se pueden utilizar varias bibliotecas, principalmente la interna de Cinder o el bloque que envuelve FMOD. Aquí describiré cómo utilizar audio con la biblioteca interna.

El sistema de audio de Cinder está diseñado a partir de una gráfica en el sentido de nodos y relaciones de tal manera que las fuentes de audio son nodos, sus modificadores como filtros y ganancias también los son.

Se incluye su cabecera.
```c++
#include "cinder/audio/audio.h"
```

Después se declaran los nodos


###Shaders
Cuando se manejan los shaders con Cinder, ya les pasa algunas variables útiles por default.

```
ciModelMatrix
ciModelMatrixInverse
ciModelMatrixInverseTranspose
ciViewMatrix
ciViewMatrixInverse
ciModelView
ciModelViewInverse
ciModelViewInverseTranspose
ciModelViewProjection
ciModelViewProjectionInverse
ciProjectionMatrix
ciProjectionMatrixInverse
ciViewProjection
ciNormalMatrix
ciViewportMatrix
ciWindowSize
ciElapsedSeconds
```

**Para pasar texturas al shader**





