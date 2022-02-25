#include "main.h"

size_t 
    number_wands;

PixelIterator
    *pixel_iterator;

PixelWand
    **pixel_wand_row = NULL,
    *background_pixel_image,
    *stroke;

MagickBooleanType
    status;

MagickWand
    *magick_wand;

DrawingWand
    *drawing_wand;

int 
    h,
    w;

double
    sx,
    sy,
    ex,
    ey,
    stroke_width;

char
    *buff_double;

int main(int argc, const char **argv)
{
    if (argc != 11)
    {
        (void)fprintf(stdout, "Usage: %s file_name.avs width height background_color line_color sx sy ex ey stroke_width\n", argv[0]);
        exit(0);
    }


    // Conversiones
    w = atoi(argv[2]);
    h = atoi(argv[3]);
    sx = strtod(argv[6], &buff_double);
    sy = strtod(argv[7], &buff_double);
    ex = strtod(argv[8], &buff_double);
    ey = strtod(argv[9], &buff_double);
    stroke_width = strtod(argv[10], &buff_double);

    //Inicializaciones
    MagickWandGenesis();
    magick_wand = NewMagickWand();
    drawing_wand = NewDrawingWand();
    background_pixel_image = NewPixelWand();
    stroke = NewPixelWand();

    //definicion de colores
    PixelSetColor(background_pixel_image, (const char *)argv[4]);
    PixelSetColor(stroke, (const char *)argv[5]);

    //definicion de la brocha
    DrawSetStrokeColor(drawing_wand, stroke);
    DrawSetStrokeWidth(drawing_wand, stroke_width);

    //creacion de imagen
    status = MagickNewImage(magick_wand, w, h, background_pixel_image);
    if (status == MagickFalse)
        ThrowWandException(magick_wand);


    MagickResetIterator(magick_wand);
    MagickNextImage(magick_wand);

    MagickAffineTransformImage(magick_wand, drawing_wand);

    DrawLine(drawing_wand, sx, sy, ex, ey);


    // escribir dibujo en la imagen
    MagickDrawImage(magick_wand, drawing_wand);

    // escribir imagen
    status = MagickWriteImage(magick_wand, argv[1]);
    if (status == MagickFalse)
        ThrowWandException(magick_wand);
    magick_wand = DestroyMagickWand(magick_wand);
    MagickWandTerminus();
    return (0);
}
