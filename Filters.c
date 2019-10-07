#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
 
/*This code uses the keyboard to excute the filters
0 = Brighter Filter
1 = Darkening Filter
2 = Grayscale Filter
3 = Red Hue Filter
4 = Green Hue Filter
5 = Blue Hue Filter
6 = Red Filter
7 = Green Filter
8 = Blue Filter

X = Colour Inversion Filter
T = Thresholding Filter
H = Highlight Filter
G = Glare Filter
C = Contrast Filter
A = Gamma Filter
B = Blur Filter
S = Sharpening Filter


O = Save image

*/

// DECLARING FUNCTIONS HERE

//clamp functions declared here
Uint8 clamp(int colour)
{
    if (colour>255)
        return 255;
    else if(colour<0)
        return 0;
    else
        return colour;
}

unsigned int clampf(float colour)
{
    if (colour>255)
        return 255;
    else if(colour<0)
        return 0;
    else
        return (int) (colour + 0.5);
}

//blur filter struct here
//Source from https://lodev.org/cgtutor/filtering.html starts here.
#define blurFilterWidth 3
#define blurFilterHeight 3

double blurFilter[blurFilterHeight][blurFilterWidth] = 
{
    {0.0, 0.2, 0.0},
    {0.2, 0.2, 0.2},
    {0.0, 0.2, 0.0}
};
double blurFactor = 1.0;

//sharpend filter struct
#define sharpenFilterHeight 3
#define sharpenFilterWidth 3

double sharpenFilter[sharpenFilterHeight][sharpenFilterWidth] =
{
    {-1, -1, -1},
    {-1,  9, -1},
    {-1, -1, -1}
};
double sharpenFactor = 1.0;


int i=0;

int main(int argc, char ** argv)
{
   
    int quit = 0;
    SDL_Event event;
 
    //SDL_Init(SDL_INIT_VIDEO);
    //IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG); 

    SDL_Window * window = SDL_CreateWindow("Filters",    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface * image = IMG_Load(argv[1]); // "imagetest.bmp"
    if (image == NULL)
    {
        printf("there's no such image, please check your argv[1] on the command line is a valid image file\n");
    }

    SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,image->w, image->h);

   // SDL_Surface * originalImage = image;
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    //SDL_FreeSurface(originalImage);
    Uint32 * pixels = (Uint32 *)image->pixels;

    while (quit==0)
    {
        SDL_UpdateTexture(texture, NULL, image->pixels,image->w * sizeof(Uint32));

        SDL_WaitEvent(&event);
 
        switch (event.type)
        {
            case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            //Greyscale Filter
            case SDLK_l:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        //convert pixel to grayscale here
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        Uint8 v = (r + g + b) / 3;

                        pixel = (0xFF << 24) | (v << 16) | (v << 8) | v;
                        pixels[y * image->w + x] = pixel;
                        
                        
                    }
                }
                break;
            //Greyscale Filter (a more accurate greyscale?)
            case SDLK_2:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        //convert pixel to grayscale here
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        Uint8 v = (0.212671*r)+(0.715160*g)+(0.072169*b);

                        pixel = (0xFF << 24) | (v << 16) | (v << 8) | v;
                        pixels[y * image->w + x] = pixel;
                        
                        
                    }
                }
                break;
            //Brighter Filter
            case SDLK_0:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        //convert pixel brighter here
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        r = clamp(r+10);
                        g = clamp(g+10);
                        b = clamp(b+10);

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                        //bright filter
                        
                    }
                }
                break;    
            //Blue Hue Filter
             case SDLK_5:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        // convert pixel more blue here
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        r = clamp(r+0);
                        g = clamp(g+0);
                        b = clamp(b+10);

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                        
                    }
                }
                break;   
            //Red Hue Filter
            case SDLK_3:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        // convert pixel more red here
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        r = clamp(r+10);
                        g = clamp(g+0);
                        b = clamp(b+0);

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                        
                    }
                }
                break;  
            //Green Hue Filter
            case SDLK_4:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        // convert pixel more red here
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        r = clamp(r+0);
                        g = clamp(g+10);
                        b = clamp(b+0);

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                        
                    }
                }
                break;  
            //Blue Filter
             case SDLK_8:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        // convert pixel more blue here
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        r = 0;
                        g = 0;
                        b = b;

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                        
                    }
                }
                break;   
            //Red Filter
            case SDLK_6:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        // convert pixel more red here
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        r = r;
                        g = 0;
                        b = 0;

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                        
                    }
                }
                break;  
            //Green Filter
            case SDLK_7:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        // convert pixel more red here
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        r = 0;
                        g = g;
                        b = 0;

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                        
                    }
                }
                break;  
            //Colour inversion
            case SDLK_x:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        r = 255-r;
                        g = 255-g;
                        b = 255-b;

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                        
                    }
                }
                break;  
            //Thresholding filter
            case SDLK_t:
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        
                        int threshold = 10;
                        Uint32 pixel = pixels[y * image->w + x];
                        
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        int col = (r + g + b)/3;
                        if (col>(128-threshold) && threshold<150)
                        {
                            r = 255;
                            g = 255;
                            b = 255;
                        }
                        else
                        {
                            r = 0;
                            g = 0;
                            b = 0;
                        }

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                        
                    }
                }
                break;  
            
                //Highlights Filter
                case SDLK_h: 
                {
                    char userHighlights[50];
                    printf("Enter highlights value:\n ");
                    scanf("%s", userHighlights);                    
                    printf("highlight read from the command line is...%s\n",userHighlights);

                    double highlights = atoi(userHighlights);
                    printf("convert to an integer highlight values = %f\n",highlights);    

                    const int v=highlights;
                    for (int y = 0; y < image->h; y++) 
                    {
                        for (int x = 0; x < image->w; x++)
                        {
                            Uint32 pixel = pixels[y * image->w + x];

                            Uint8 r = pixel >> 16 & 0xFF;
                            Uint8 g = pixel >> 8 & 0xFF;
                            Uint8 b = pixel & 0xFF;
                            
                            r = clamp(r-v);
                            g = clamp(g-v);
                            b = clamp(b-v);

                            pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                            pixels[y * image->w + x] = pixel;
                            
                            
                        }
                    }
                }

                break;
            
                //Glare Filter
                case SDLK_g: 
                {
                    char userGlare[100];
                    printf("Enter glare value:\n ");
                    scanf("%s", userGlare);                    
                    printf("glare read from the command line is...%s\n",userGlare);

                    int glare = atoi(userGlare);
                    printf("convert to an integer glare values = %d\n",glare);

                    const int v=glare;
                    for (int y = 0; y < image->h; y++) 
                    {
                        for (int x = 0; x < image->w; x++)
                    
                        {
                            Uint32 pixel = pixels[y * image->w + x];

                            Uint8 r = pixel >> 16 & 0xFF;
                            Uint8 g = pixel >> 8 & 0xFF;
                            Uint8 b = pixel & 0xFF;
                            
                            Uint8 r1 = clamp(r + clamp(r-v));
                            Uint8 g1 = clamp(g + clamp(g-v));
                            Uint8 b1 = clamp(b + clamp(b-v));
                            pixel = (0xFF << 24) | (r1<< 16) | (g1 << 8) | b1;
                            pixels[y * image->w + x] = pixel;
                            
                            
                        }
                    }
                }

                break;
            //Contrast Filter 
            ////Contrast is defined as the separation between the darkest and brightest areas of the image. 
            //Increase contrast and you increase the separation between dark and bright, making shadows darker and highlights brighter.
            case SDLK_c:
                
                {
                   
                    
                    char userContrast[30];
                    printf("Enter contrast value:\n ");
                    scanf("%s", userContrast);                    
                    printf("contrast read from the command line is...%s\n",userContrast);

                    int contrast = atoi(userContrast);
                    printf("convert to an integer contrast values = %d\n",contrast);
                    
                    
                    
                    for (int y = 0; y < image->h; y++)
                    {
                        for (int x = 0; x < image->w; x++)
                        {
                            Uint32 pixel = pixels[y * image->w + x];
   
                            float factor =(float)(259*(contrast+255))/(float)(255*(259-contrast));
                            Uint8 r = pixel >> 16 & 0xFF;
                            Uint8 g = pixel >> 8 & 0xFF;
                            Uint8 b = pixel & 0xFF;
                            r = clampf(factor*(r-128)+128)& 0xFF;
                            g = clampf(factor*(g-128)+128)& 0xFF;
                            b = clampf(factor*(b-128)+128)& 0xFF;

                            pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                            pixels[y * image->w + x] = pixel;
                        }   
                    }
                }
            break;  
            //Gamma Filter 
            //Gamma correction controls the overall brightness of an image.
            case SDLK_a:
            {
        
                char gammaCorrection[2];
                printf("Enter gamma value between 0-8:\n ");
                scanf("%s", gammaCorrection);                    
                printf("gamma read from the command line is...%s\n",gammaCorrection);

                double gamma = atoi(gammaCorrection);
                printf("convert to an integer gamma values = %f\n",gamma); 
                
                
                //int gamma = atof(argv[i]); 
                //double gamma = 2.0; // 0.01 to 7.99
                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        
                        double gammaCorrection =1.0/gamma;
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        r = clampf(255.0*pow(r/255.0, gammaCorrection))& 0xFF;
                        g = clampf(255.0*pow(g/255.0, gammaCorrection))& 0xFF;
                        b = clampf(255.0*pow(b/255.0, gammaCorrection))& 0xFF;

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                    }   
                }
            }
            break;  
            
            //Blur Filter
            case SDLK_b:
            {
                char userBlur[30];
                printf("Enter blur value:\n ");
                scanf("%s", userBlur);                    
                printf("blur read from the command line is...%s\n",userBlur);

                int blur = atoi(userBlur);
                printf("convert to an integer blur values = %d\n",blur);
                    

                //Source based on "Dr Xiaosong Yang" starts here.
                //double blur = 0.1;
                for (int max = 0; max < blur; max++)
                {
                    Uint8 r,g,b;
                    Uint32 pixel;
                    Uint8 *imageCR = (Uint8 *)malloc(sizeof(Uint8)*image->w*image->h);  
                    Uint8 *imageCG = (Uint8 *)malloc(sizeof(Uint8)*image->w*image->h);
                    Uint8 *imageCB = (Uint8 *)malloc(sizeof(Uint8)*image->w*image->h);
                    for (int y = 0; y < image->h; y++)
                    {
                        for(int x = 0; x < image->w; x++)
                        {
                            
                            pixel = pixels[y * image->w + x];
                            r = pixel >> 16 & 0xFF;
                            g = pixel >> 8 & 0xFF;
                            b = pixel & 0xFF;
                            imageCR[y * image->w + x] = r;
                            imageCG[y * image->w + x] = g;
                            imageCB[y * image->w + x] = b;
                        }
                    }
                    
                    for (int y = 0; y < image->h; y++)
                    {
                        for (int x = 0; x < image->w; x++)
                        {
                            float resultR = 0.0, resultG = 0.0, resultB = 0.0;
                            for(int m=0;m<blurFilterHeight;m++)
                            {
                                for(int n =0;n<blurFilterWidth;n++) 
                                {
                                    int imageX = (x-blurFilterWidth/2 + n)%image->w;
                                    int imageY = (y-blurFilterHeight/2 + m)%image->h;
                                    resultR+= imageCR[imageY*image->w+imageX]*blurFilter[m][n];   
                                    resultG+= imageCG[imageY*image->w+imageX]*blurFilter[m][n];    
                                    resultB+= imageCB[imageY*image->w+imageX]*blurFilter[m][n]; 
                                }    
                            }

                            r = clampf(resultR)& 0xFF;
                            g = clampf(resultG)& 0xFF;
                            b = clampf(resultB)& 0xFF;
                            pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                            pixels[y * image->w + x] = pixel;
                        }
                    }            

                    free(imageCR);
                    free(imageCG);
                    free(imageCB);
                }
            }
            break; 
            //Source based on "Dr Xiaosong Yang" ends here.

            //Sharpening Filter
            case SDLK_s:
            {
                char userSharpen[30];
                printf("Enter sharpen value from 1-2:\n ");
                scanf("%s", userSharpen);                    
                printf("sharpen read from the command line is...%s\n",userSharpen);

                int sharpen = atoi(userSharpen);
                printf("convert to an integer sharpen values = %d\n",sharpen);

                //double sharpen = 0.001;
                for (int max = 0; max < sharpen; max++)
                {
                    Uint8 r,g,b;
                    Uint32 pixel;
                    Uint8 *imageCR = (Uint8 *)malloc(sizeof(Uint8)*image->w*image->h);
                    Uint8 *imageCG = (Uint8 *)malloc(sizeof(Uint8)*image->w*image->h);
                    Uint8 *imageCB = (Uint8 *)malloc(sizeof(Uint8)*image->w*image->h);
                    for (int y = 0; y < image->h; y++)
                    {
                        for (int x = 0; x < image->w; x++)
                        {
                            pixel = pixels[y * image->w + x];
                            r = pixel >> 16 & 0xFF;
                            g = pixel >> 8 & 0xFF;
                            b = pixel & 0xFF;
                            imageCR[y * image->w + x] = r;
                            imageCG[y * image->w + x] = g;
                            imageCB[y * image->w + x] = b;
                        }
                    }   

                    for (int y = 0; y < image->h; y++)
                    {
                        for (int x = 0; x < image->w; x++)
                        {
                            float resultR = 0.0, resultG = 0.0, resultB = 0.0;
                            for(int m=0;m<sharpenFilterHeight;m++)
                            {
                                for(int n =0;n<sharpenFilterWidth;n++)
                                {
                                    int imageX = (x-sharpenFilterWidth/2 + n)%image->w;
                                    int imageY = (y-sharpenFilterHeight/2 + m)%image->h;
                                    resultR+= imageCR[imageY*image->w+imageX]*sharpenFilter[m][n];
                                    resultG+= imageCG[imageY*image->w+imageX]*sharpenFilter[m][n];
                                    resultB+= imageCB[imageY*image->w+imageX]*sharpenFilter[m][n];
                                }
                            }

                            r = clampf(resultR)& 0xFF;
                            g = clampf(resultG)& 0xFF;
                            b = clampf(resultB)& 0xFF;
                            pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                            pixels[y * image->w + x] = pixel;

                        }
                    }

                    free(imageCR);
                    free(imageCG);
                    free(imageCB);
                }
            }
            break;

            //Darkening Filter  
            case SDLK_1:

                for (int y = 0; y < image->h; y++)
                {
                    for (int x = 0; x < image->w; x++)
                    {
                        Uint32 pixel = pixels[y * image->w + x];
                        // Darkening filter
                        Uint8 r = pixel >> 16 & 0xFF;
                        Uint8 g = pixel >> 8 & 0xFF;
                        Uint8 b = pixel & 0xFF;
                        r = clampf(r-10);
                        g = clampf(g-10);
                        b = clampf(b-10);

                        pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
                        pixels[y * image->w + x] = pixel;
                        
                        
                    }
                }
                break;

            //this is where the images are saved
            case SDLK_o:
            {
                char saveimage[30];
                printf("Enter name of image: ");
                scanf("%s",saveimage);
                SDL_SaveBMP(image, saveimage);
                
            }
        }
        break;
        case SDL_QUIT:
            quit = 1;
            break;
    }
 
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
 
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
 
    return 0;
}
