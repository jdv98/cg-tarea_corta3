#include "main.h"

void iniciarMatriz();
void escrituraAVS();
void lecturaAVS();


size_t
    number_wands;

PixelIterator
    *pixel_iterator;

PixelWand
    **pixel_wand_row = NULL,
    *background_pixel_image;

MagickBooleanType
    status;

MagickWand
    *magick_wand;

int
    h,
    w;

COLOR **buffer;

int main(int argc, const char **argv)
{
    int escribir=0;
    if (argc >4 || argc <2)
    {
        (void)fprintf(stdout, "Usage: %s input.avs\n     : %s input.avs height width\n", argv[0],argv[0]);
        exit(0);
    }
    else if(argc==4){
        escribir=1;
        h = atoi(argv[2]);
        w = atoi(argv[3]);
        iniciarMatriz();
    }

    // nombre del archivo .avs
    char * nombre=malloc(sizeof(char)*(strlen(argv[1])+4));
    sprintf(nombre, "%s.avs", argv[1]);

    // Inicializaciones
    MagickWandGenesis();
    magick_wand = NewMagickWand();
    background_pixel_image = NewPixelWand();
    number_wands = 0;

    
    if (escribir)
    {
        // definicion del fondo
        PixelSetColor(background_pixel_image, "black");

        //Crea imagen de fondo negro
        status = MagickNewImage(magick_wand, w, h, background_pixel_image);
        if (status == MagickFalse)
            ThrowWandException(magick_wand);
        MagickResetIterator(magick_wand);
        MagickNextImage(magick_wand);

        escrituraAVS();
        DestroyPixelIterator(pixel_iterator);

        status = MagickWriteImage(magick_wand, nombre);
        if (status == MagickFalse)
            ThrowWandException(magick_wand);
        /**/
    }
    else
    {
        //lee la imagen
        status = MagickReadImage(magick_wand, nombre);
        if (status == MagickFalse)
            ThrowWandException(magick_wand);
        MagickResetIterator(magick_wand);
        MagickNextImage(magick_wand);

        //define la altura y anchura de la matriz
        h=MagickGetImageHeight(magick_wand);
        w=MagickGetImageWidth(magick_wand);
        iniciarMatriz();

        //
        lecturaAVS();
        DestroyPixelIterator(pixel_iterator);
        
    }

    // escribir imagen

    magick_wand = DestroyMagickWand(magick_wand);
    MagickWandTerminus();
    return (0);
}

void lecturaAVS() //Graba los pixeles que estan en el iterador de magickwand en la matriz de colores y luego los imprime como comprobacion
{
    pixel_iterator = NewPixelIterator(magick_wand);
    PixelResetIterator(pixel_iterator);
    pixel_wand_row = PixelGetCurrentIteratorRow(pixel_iterator, &number_wands);
    
    int column = 0;
    do
    {
        for (size_t i = 0; i < number_wands; i++)
        {
            buffer[column][i].r=(double)PixelGetRed(pixel_wand_row[i]);
            buffer[column][i].g=(double)PixelGetGreen(pixel_wand_row[i]);
            buffer[column][i].b=(double)PixelGetBlue(pixel_wand_row[i]);
        }
        pixel_wand_row = PixelGetNextIteratorRow(pixel_iterator, &number_wands);
        column++;
    } while (number_wands > 0);


    printf("Impresion de la matriz guardada:\n");

    for (size_t i = 0; i < h; i++)
    {
        for (size_t j = 0; j < w; j++)
        {
            printf("(%f,%f,%f)  ",buffer[i][j].r,buffer[i][j].g,buffer[i][j].b);
        }
        printf("\n\n");
    }
    
}

void escrituraAVS() //Escribi colores aleatorios en la matriz de colores y luego los escribe en el iterador de las filas.
{
    double r, g, b;
    srand(time(NULL));
    for (int i = 0; i < h; i++)
    {
        r = (double)rand() / RAND_MAX;
        g = (double)rand() / RAND_MAX;
        b = (double)rand() / RAND_MAX;
        for (int j = 0; j < w; j++)
        {
            buffer[i][j].r = r;
            buffer[i][j].g = g;
            buffer[i][j].b = b;
        }
    }

    pixel_iterator = NewPixelIterator(magick_wand);
    PixelResetIterator(pixel_iterator);
    pixel_wand_row = PixelGetCurrentIteratorRow(pixel_iterator, &number_wands);
    int column = 0;
    do
    {

        for (size_t i = 0; i < number_wands; i++)
        {
            PixelSetRed(pixel_wand_row[i], buffer[column][i].r);
            PixelSetGreen(pixel_wand_row[i], buffer[column][i].g);
            PixelSetBlue(pixel_wand_row[i], buffer[column][i].b);
            PixelSyncIterator(pixel_iterator);
        }
        pixel_wand_row = PixelGetNextIteratorRow(pixel_iterator, &number_wands);
        column++;
    } while (number_wands > 0);
}

void iniciarMatriz(){//Inicializa la matriz de colores

    buffer = (COLOR **)malloc(h * sizeof(COLOR *));
    for (int i = 0; i < h; i++)
    {
        buffer[i] = (COLOR *)malloc(w * sizeof(COLOR));
    }
}