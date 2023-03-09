#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <string>
#include <time.h>
#include <string.h>
#include <ctype.h>

//Define the struct used for the table
typedef struct tableau
{
    int hide;
    int nombredeBombe;
    int flag;
    int bombe;
    int safe;


}tableau;

//Initialize the replaceCase function
void replaceCase(SDL_Texture* caseTexture2, SDL_Texture* emptycaseTexture, SDL_Texture* caseTexture, SDL_Texture* bombTexture, int *nbflag, int size, tableau* table, int casesize, int casetext, int x, int y, SDL_Rect rectangle, SDL_Renderer* renderer, SDL_Surface* surfaceMessage, TTF_Font* Sans, SDL_Color Color, SDL_Texture* Message, SDL_Rect Message_rect);

//Returncase returns the coordinates of a case with its number in the table (ex: t[15])
int returncase(int i, int j, int size) {
    int result = i * size + j;
    return result;
}

//The safezone function clears the bombs around the first played case
void safeZone(int i, int j, int size, tableau* table)
{
    int result;
    result = returncase(i, j, size);
    table[result].safe = 1;
    //left
    if (j - 1 >= 0) {
        result = returncase(i, j - 1, size);
        table[result].safe = 1;


    }

    //right
    if (j + 1 <= size - 1) {
        result = returncase(i, j + 1, size);
        table[result].safe = 1;

    }

    //top
    if (i - 1 >= 0) {
        result = returncase(i - 1, j, size);
        table[result].safe = 1;

    }

    //bottom
    if (i + 1 <= size - 1) {
        result = returncase(i + 1, j, size);
        table[result].safe = 1;

    }

    //top left
    if (i - 1 >= 0 && j - 1 >= 0) {
        result = returncase(i - 1, j - 1, size);
        table[result].safe = 1;

    }

    //bottom left
    if (i + 1 >= 0 && j - 1 >= 0) {
        result = returncase(i + 1, j - 1, size);
        table[result].safe = 1;

    }

    //top right
    if (i - 1 >= 0 && j + 1 <= size - 1) {
        result = returncase(i - 1, j + 1, size);
        table[result].safe = 1;

    }

    //bottom right
    if (i + 1 >= 0 && j + 1 <= size - 1) {
        result = returncase(i + 1, j + 1, size);
        table[result].safe = 1;

    }
}

//Creates the bombs randomly, there can never be 2 bombs on the same case
void bombe(int size, tableau* t, int nbbomb, int i, int j) {
    int listebombe = 0;
    int result;


    safeZone(i, j, size, t);

    for (int i = 0; i < size * size; i++)
    {
        t[i].bombe = i;

    }
    while (listebombe < nbbomb)
    {
        int bombex = rand() % (size * size);
        if (t[bombex].nombredeBombe != 9)
        {
            if (t[bombex].safe == 0)
            {
                t[bombex].nombredeBombe = 9;
                t[i].bombe = size * size + 1;
                listebombe = listebombe + 1;
            }

        }

    }


}

//The check function checks every bomb around a case so the number of bombs can be shown inside the case
void check(tableau* table, int size) {

    int i, j, count = 0;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            int result = returncase(i, j, size);
            if (table[result].nombredeBombe == 9) {

            }
            else {

                //left
                if (j - 1 >= 0) {
                    result = returncase(i, j - 1, size);
                    if (table[result].nombredeBombe == 9) {
                        count++;
                    }
                }

                //right
                if (j + 1 <= size - 1) {
                    result = returncase(i, j + 1, size);
                    if (table[result].nombredeBombe == 9) {
                        count++;
                    }
                }

                //top
                if (i - 1 >= 0) {
                    result = returncase(i - 1, j, size);
                    if (table[result].nombredeBombe == 9) {
                        count++;
                    }
                }

                //bottom
                if (i + 1 <= size - 1) {
                    result = returncase(i + 1, j, size);
                    if (table[result].nombredeBombe == 9) {
                        count++;
                    }
                }

                //top left
                if (i - 1 >= 0 && j - 1 >= 0) {
                    result = returncase(i - 1, j - 1, size);
                    if (table[result].nombredeBombe == 9) {
                        count++;
                    }
                }

                //bottom left
                if (i + 1 >= 0 && j - 1 >= 0) {
                    result = returncase(i + 1, j - 1, size);
                    if (table[result].nombredeBombe == 9) {
                        count++;
                    }
                }

                //top right
                if (i - 1 >= 0 && j + 1 <= size - 1) {
                    result = returncase(i - 1, j + 1, size);
                    if (table[result].nombredeBombe == 9) {
                        count++;
                    }
                }

                //bottom right
                if (i + 1 >= 0 && j + 1 <= size - 1) {
                    result = returncase(i + 1, j + 1, size);
                    if (table[result].nombredeBombe == 9) {
                        count++;
                    }
                }

                result = returncase(i, j, size);
                table[result].nombredeBombe = count;
                count = 0;
            }
        }
    }
}

//Checkwin is a function used to check if the player won or not
int checkwin(tableau* table, int size) {

    int i, j, count = 0, count2 = 0;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            int result = returncase(j, i, size);
            if (table[result].nombredeBombe >= 0 && table[result].nombredeBombe <= 8) {
                count++;
            }
            if (table[result].hide == 1) {
                count2++;
            }
        }
    }
    if (count2 == count) {
        return 1;
    }

    return 0;
}

//Initialize the table
void initTable(tableau* table, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        table[i].bombe = 0;
        table[i].flag = 0;
        table[i].hide = 0;
        table[i].nombredeBombe = 0;
        table[i].safe = 0;
    }
}

//This function is used to retrieve the color of the selected coordinates
Uint32 getpixel(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

switch (bpp)
{
case 1:
    return *p;
    break;

case 2:
    return *(Uint16*)p;
    break;

case 3:
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        return p[0] << 16 | p[1] << 8 | p[2];
    else
        return p[0] | p[1] << 8 | p[2] << 16;
    break;

case 4:
    return *(Uint32*)p;
    break;

default:
    return 0;
}
}

//The createtext function is used to create text
void createText(const char* text, int x, int y, int textSize, SDL_Surface* surfaceMessage, TTF_Font* Sans, SDL_Color Color, SDL_Texture* Message, SDL_Renderer* renderer, SDL_Rect Message_rect) {
    Sans = TTF_OpenFont("Fonts/Roboto-Black.ttf", textSize);
    surfaceMessage =
        TTF_RenderText_Blended(Sans, text, Color);
    Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = surfaceMessage->w;
    Message_rect.h = surfaceMessage->h;

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}

//The displaytable displays the table before the game starts, we then modify the area we need to, without creating the table all over again
void displayTable(SDL_Texture* caseTexture, int casevalue, int size, int casesize, int viewportwidth, SDL_Rect rectangle, SDL_Surface* surfaceMessage, TTF_Font* Sans, SDL_Color Color, SDL_Texture* Message, SDL_Renderer* renderer, SDL_Rect Message_rect) {
    char buffer[256];
    for (int i = 0; i < size * size; i++) {

        SDL_RenderCopy(renderer, caseTexture, NULL, &rectangle);
        if (casevalue == 0) {

        }
        else {
            sprintf_s(buffer, 256, "%d", casevalue);
            createText(buffer, rectangle.x + casesize / 2.8, rectangle.y + casesize / 5, casesize / 2, surfaceMessage, Sans, Color, Message, renderer, Message_rect);
        }

        if (rectangle.x + casesize + 5 >= viewportwidth - casesize) {
            rectangle.y += rectangle.h;
            rectangle.x = 0 + casesize;
        }
        else {
            rectangle.x += casesize;
        }
    }
}

//This function checks if there is a 0 next to the case we have clicked on (if its a zero), it will then change it to an open case
void checkzeros(SDL_Texture* caseTexture2, SDL_Texture* emptycaseTexture, SDL_Texture* caseTexture, SDL_Texture* bombTexture, int *nbflag, int size, tableau* table, int casesize, int casetext, int x, int y, SDL_Rect rectangle, SDL_Renderer* renderer, SDL_Surface* surfaceMessage, TTF_Font* Sans, SDL_Color Color, SDL_Texture* Message, SDL_Rect Message_rect) {
    
    
    if (table[x * size + y].nombredeBombe == 0) {
        //left
        if (x - 1 >= 0) {
            replaceCase(caseTexture2, emptycaseTexture, caseTexture, bombTexture, nbflag, size, table, casesize, casetext, x - 1, y, rectangle, renderer, surfaceMessage, Sans, Color, Message, Message_rect);
        }
        //right
        if (x + 1 < size) {
            replaceCase(caseTexture2, emptycaseTexture, caseTexture, bombTexture, nbflag, size, table, casesize, casetext, x + 1, y, rectangle, renderer, surfaceMessage, Sans, Color, Message, Message_rect);
        }
        //bottom
        if (y + 1 < size) {
            replaceCase(caseTexture2, emptycaseTexture, caseTexture, bombTexture, nbflag, size, table, casesize, casetext, x, y + 1, rectangle, renderer, surfaceMessage, Sans, Color, Message, Message_rect);
        }
        //top
        if (y - 1 >= 0) {
            replaceCase(caseTexture2, emptycaseTexture, caseTexture, bombTexture, nbflag, size, table, casesize, casetext, x, y - 1, rectangle, renderer, surfaceMessage, Sans, Color, Message, Message_rect);
        }
        //top left
        if (y - 1 >= 0 && x - 1 >= 0) {
            replaceCase(caseTexture2, emptycaseTexture, caseTexture, bombTexture, nbflag, size, table, casesize, casetext, x - 1, y - 1, rectangle, renderer, surfaceMessage, Sans, Color, Message, Message_rect);
        }
        //bottom left
        if (y + 1 < size && x + 1 < size) {
            replaceCase(caseTexture2, emptycaseTexture, caseTexture, bombTexture, nbflag, size, table, casesize, casetext, x + 1, y + 1, rectangle, renderer, surfaceMessage, Sans, Color, Message, Message_rect);
        }
        //top right
        if (y - 1 >= 0 && x + 1 < size) {
            replaceCase(caseTexture2, emptycaseTexture, caseTexture, bombTexture, nbflag, size, table, casesize, casetext, x + 1, y - 1, rectangle, renderer, surfaceMessage, Sans, Color, Message, Message_rect);
        }
        //bottom right
        if (x - 1 >= 0 && y + 1 < size) {
            replaceCase(caseTexture2, emptycaseTexture, caseTexture, bombTexture, nbflag, size, table, casesize, casetext, x - 1, y + 1, rectangle, renderer, surfaceMessage, Sans, Color, Message, Message_rect);
        }
    }
}

//replacecase will change the case of our choice to its corresponding display
void replaceCase(SDL_Texture* caseTexture2, SDL_Texture* emptycaseTexture, SDL_Texture* caseTexture, SDL_Texture* bombTexture, int *nbflag, int size, tableau* table, int casesize, int casetext, int x, int y, SDL_Rect rectangle, SDL_Renderer* renderer, SDL_Surface* surfaceMessage, TTF_Font* Sans, SDL_Color Color, SDL_Texture* Message, SDL_Rect Message_rect) {
    char buffer[256];

    //1
    SDL_Color Blue = { 0, 0, 255 };
    //2
    SDL_Color Green = { 0, 255, 0 };
    //3
    SDL_Color Red = { 255, 0, 0 };
    //4
    SDL_Color Purple = { 128, 0, 128 };
    //5
    SDL_Color Maroon = { 128, 0, 0 };
    //6
    SDL_Color Turquoise = { 64, 224, 208 };
    //7
    SDL_Color Black = { 10, 10, 10 };
    //8
    SDL_Color Gray = { 128, 128, 128 };

    if (table[x * size + y].hide == 0) {

        if (table[x * size + y].flag == 1) {
            table[x * size + y].flag = 0;
            *nbflag += 1;
        }

        rectangle.x = x * casesize + casesize;
        rectangle.y = y * casesize + casesize;

        SDL_RenderCopy(renderer, emptycaseTexture, NULL, &rectangle);
        SDL_RenderCopy(renderer, caseTexture2, NULL, &rectangle);
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
        SDL_RenderCopy(renderer, emptycaseTexture, NULL, &rectangle);
        SDL_RenderPresent(renderer);


        table[x * size + y].hide = 1;


        if (table[x * size + y].nombredeBombe == 9) {
            SDL_RenderCopy(renderer, bombTexture, NULL, &rectangle);

            createText("You Lose", ((size * casesize + casesize * 2) / 2) / 2, ((size * casesize + casesize * 2) / 2) / 1.2, ((size * casesize + casesize * 2) / 2) / 3.5, surfaceMessage, Sans, Red, Message, renderer, Message_rect);
            SDL_RenderPresent(renderer);
            SDL_Delay(5000);
            exit(0);
        }

        else if (table[x * size + y].nombredeBombe == 0) {
            checkzeros(caseTexture2, emptycaseTexture, caseTexture, bombTexture, nbflag, size, table, casesize, casetext, x, y, rectangle, renderer, surfaceMessage, Sans, Color, Message, Message_rect);
        }
        else {
            if (table[x * size + y].nombredeBombe == 1) {
                sprintf_s(buffer, 256, "%d", table[x * size + y].nombredeBombe);
                createText(buffer, rectangle.x + casesize / 2.8, rectangle.y + casesize / 5, casesize / 2, surfaceMessage, Sans, Blue, Message, renderer, Message_rect);
            }
            if (table[x * size + y].nombredeBombe == 2) {
                sprintf_s(buffer, 256, "%d", table[x * size + y].nombredeBombe);
                createText(buffer, rectangle.x + casesize / 2.8, rectangle.y + casesize / 5, casesize / 2, surfaceMessage, Sans, Green, Message, renderer, Message_rect);
            }
            if (table[x * size + y].nombredeBombe == 3) {
                sprintf_s(buffer, 256, "%d", table[x * size + y].nombredeBombe);
                createText(buffer, rectangle.x + casesize / 2.8, rectangle.y + casesize / 5, casesize / 2, surfaceMessage, Sans, Red, Message, renderer, Message_rect);
            }
            if (table[x * size + y].nombredeBombe == 4) {
                sprintf_s(buffer, 256, "%d", table[x * size + y].nombredeBombe);
                createText(buffer, rectangle.x + casesize / 2.8, rectangle.y + casesize / 5, casesize / 2, surfaceMessage, Sans, Purple, Message, renderer, Message_rect);
            }
            if (table[x * size + y].nombredeBombe == 5) {
                sprintf_s(buffer, 256, "%d", table[x * size + y].nombredeBombe);
                createText(buffer, rectangle.x + casesize / 2.8, rectangle.y + casesize / 5, casesize / 2, surfaceMessage, Sans, Maroon, Message, renderer, Message_rect);
            }
            if (table[x * size + y].nombredeBombe == 6) {
                sprintf_s(buffer, 256, "%d", table[x * size + y].nombredeBombe);
                createText(buffer, rectangle.x + casesize / 2.8, rectangle.y + casesize / 5, casesize / 2, surfaceMessage, Sans, Turquoise, Message, renderer, Message_rect);
            }
            if (table[x * size + y].nombredeBombe == 7) {
                sprintf_s(buffer, 256, "%d", table[x * size + y].nombredeBombe);
                createText(buffer, rectangle.x + casesize / 2.8, rectangle.y + casesize / 5, casesize / 2, surfaceMessage, Sans, Black, Message, renderer, Message_rect);
            }
            if (table[x * size + y].nombredeBombe == 8) {
                sprintf_s(buffer, 256, "%d", table[x * size + y].nombredeBombe);
                createText(buffer, rectangle.x + casesize / 2.8, rectangle.y + casesize / 5, casesize / 2, surfaceMessage, Sans, Gray, Message, renderer, Message_rect);
            }
            
        }
        

    }



}

//replaceone will do the same as the replaceCase function, but without calling the checkzeros function
int replaceOne(SDL_Texture* caseTexture2, SDL_Texture* emptycaseTexture, SDL_Texture* caseTexture, SDL_Texture* bombTexture, int* nbflag, int size, tableau* table, int casesize, int casetext, int padding, int x, int y, SDL_Rect rectangle, SDL_Renderer* renderer, SDL_Surface* surfaceMessage, TTF_Font* Sans, SDL_Color Color, SDL_Texture* Message, SDL_Rect Message_rect) {
    char buffer[256];

    table[x * size + y].hide = 1;

    rectangle.x = x * casesize + padding;
    rectangle.y = y * casesize + padding;

    SDL_RenderCopy(renderer, emptycaseTexture, NULL, &rectangle);
    SDL_RenderCopy(renderer, caseTexture2, NULL, &rectangle);
    SDL_RenderPresent(renderer);
    SDL_Delay(50);
    SDL_RenderCopy(renderer, emptycaseTexture, NULL, &rectangle);
    SDL_RenderPresent(renderer);
    int resultat = x * size + y;
    return resultat;

}

//This is the main function where everything happens
int main(int argc, char* argv[])
{

    //FPS setup
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    Uint32 frameTime = 1;
    int frameRate;
    char buffer[256];
    int animstart = 0;

    //Initiatlize SDL2 window and renderer
    TTF_Init();
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int statut = EXIT_FAILURE;

    //Initialize table
        //Table size
        int size = 8;
        //Size of each case
        int casesize = 50;
        //Width and height of the viewport
        int viewportwidth = size * casesize + casesize * 2;
        int viewportheight = size * casesize + casesize * 2;
        //Number of bombs
        int nbbomb = size * size * 0.13;
        //Number of flags
        int nbflag = nbbomb;
        //Table total size
        int tailleTotal = size * size;

        //Malloc to have a modifiable size for the table
        tableau* table = (tableau*)malloc(sizeof(tableau) * tailleTotal);

        //Initialize the coordinates
        int x, y;
        //Initialize the rgb variable
        SDL_Color rgb;
        //Initialize the tour variable
        int tour = 0;

    //Create the table
    initTable(table, tailleTotal);

    //Intialize the srand value to get a random value and not the same
    srand(time(NULL));

    //Set the window and the renderer so they can be shown
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        viewportwidth, viewportheight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        //Every font sizes for the text
            //12 - 18 - 24 - 36 - 48 - 60 - 72

    //Create the text and some colors
    TTF_Font* Sans = TTF_OpenFont("Fonts/Roboto-Black.ttf", 72);
    SDL_Color White = { 255, 255, 255 };
    SDL_Color Green = { 0, 255, 0 };
    SDL_Color Red = { 255, 0, 0 };
    SDL_Surface* surfaceMessage =
        TTF_RenderText_Blended(Sans, "text", White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    //Create the textures and initialize the main texture
    SDL_Surface* surface = SDL_CreateRGBSurface(0, viewportwidth, viewportheight, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_Surface* flag = SDL_LoadBMP("img/flag.bmp");
    SDL_Texture* flagTexture = SDL_CreateTextureFromSurface(renderer, flag);

    SDL_Surface* bomb = SDL_LoadBMP("img/bomb.bmp");
    SDL_Texture* bombTexture = SDL_CreateTextureFromSurface(renderer, bomb);

    SDL_Surface* casesurface = SDL_LoadBMP("img/case.bmp");
    SDL_Texture* caseTexture = SDL_CreateTextureFromSurface(renderer, casesurface);
    SDL_Surface* casesurface2 = SDL_LoadBMP("img/case2.bmp");
    SDL_Texture* caseTexture2 = SDL_CreateTextureFromSurface(renderer, casesurface2);
    SDL_Surface* emptycase = SDL_LoadBMP("img/emptycase.bmp");
    SDL_Texture* emptycaseTexture = SDL_CreateTextureFromSurface(renderer, emptycase);
    SDL_Texture* texture;

    //Create the rectangle for the texts
    SDL_Rect Message_rect;
    Message_rect.w = surfaceMessage->w;
    Message_rect.h = surfaceMessage->h;

    //Create the rectangle for the cases
    SDL_Rect rectangle;
    rectangle.w = casesize;
    rectangle.h = casesize;
    rectangle.y = 0 + casesize;
    rectangle.x = 0 + casesize;

    //Create the rectangle for the renderer
    SDL_Rect Renderer_rect;
    Renderer_rect.x = 0;
    Renderer_rect.y = 0;
    Renderer_rect.w = viewportwidth;
    Renderer_rect.h = viewportheight;

    //Change the cases' color
    SDL_SetRenderDrawColor(renderer, 112, 168, 237, SDL_ALPHA_OPAQUE);
    //Create the table for the renderer
    displayTable(caseTexture, 0, size, casesize, viewportwidth, rectangle, surfaceMessage, Sans, White, Message, renderer, Message_rect);
    //Render the table
    SDL_RenderPresent(renderer);
    //Read the renderer
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    //Save the renderer as a texture
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    while (1) {
        //Framerate setup
        frameStart = SDL_GetTicks();
        frameRate = 1000 / frameTime;


        //Initialize events
        SDL_Event event;

        //Clear the renderer
        SDL_RenderClear(renderer);
        //Add the main texture to the renderer
        SDL_RenderCopy(renderer, texture, NULL, &Renderer_rect);
        //Show the framerate
        sprintf_s(buffer, 256, "%d", frameRate);
        createText(buffer, 0, 0, 24, surfaceMessage, Sans, White, Message, renderer, Message_rect);
        //Show the number of flags
        sprintf_s(buffer, 256, "Flags : %d", nbflag);
        createText(buffer, 0, 20, 24, surfaceMessage, Sans, Red, Message, renderer, Message_rect);


        //Display the renderer
        SDL_RenderPresent(renderer);

        

        //If an event is triggered by the player
        if (SDL_PollEvent(&event)) {

            //If the event is a mouse button pressed
            if (SDL_MOUSEBUTTONDOWN == event.type) {

                //Get the coordinates of the mouse
                SDL_GetMouseState(&x, &y);
                //Get the color of the pixel the mouse is on
                Uint32 data = getpixel(surface, x, y);
                SDL_GetRGB(data, surface->format, &rgb.r, &rgb.g, &rgb.b);
                //Change x and y to their table coordinates
                x = (x - casesize) / casesize;
                y = (y - casesize) / casesize;
                //Print the coordinates
                printf("%d, %d\n", x, y);

                //Set the coordinates of the current selection to its corresponding values
                rectangle.x = x * casesize + casesize;
                rectangle.y = y * casesize + casesize;

                //If the color we're clicking on is black, this means we shouldn't click there, so we're printing it
                if (rgb.r == 0 && rgb.g == 0 && rgb.b == 0) {
                    printf("black pixel\n");
                }
                //Else we can play according to the player's action
                else{

                    //If the left button is pressed
                    if (SDL_BUTTON_LEFT == event.button.button) {
                       
                        
                        //Print the pressed button
                        printf("Left MB down\n");
                        if (tour == 0) {
                            printf("premier tour\n");
                            tour = tour + 1;

                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, texture, NULL, &Renderer_rect);

                            replaceOne(caseTexture2, emptycaseTexture, caseTexture, bombTexture, &nbflag, size, table, casesize, 0, casesize, x, y, rectangle, renderer, surfaceMessage, Sans, White, Message, Message_rect);
                            SDL_RenderPresent(renderer);
                            SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
                            texture = SDL_CreateTextureFromSurface(renderer, surface);
                            bombe(size, table, nbbomb, x, y);

                            check(table, size);


                            checkzeros(caseTexture2, emptycaseTexture, caseTexture, bombTexture, &nbflag, size, table, casesize, 0, x, y, rectangle, renderer, surfaceMessage, Sans, White, Message, Message_rect);
                            SDL_RenderPresent(renderer);
                            SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
                            texture = SDL_CreateTextureFromSurface(renderer, surface);
                        }else{

                            //Clear the renderer so we can replace save it again with changed values but without texts such as the framerate
                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, texture, NULL, &Renderer_rect);

                            //Call the replacecase function, that's going to replace the corresponding cases with corresponding values
                            replaceCase(caseTexture2, emptycaseTexture, caseTexture, bombTexture, &nbflag, size, table, casesize, 1, x, y, rectangle, renderer, surfaceMessage, Sans, White, Message, Message_rect);
                        
                            //If the function checkwin returns 1, it means we won, so play this winning part
                            if (checkwin(table, size) == 1) {
                                createText("You Win", (viewportheight / 2) / 2, (viewportwidth / 2) / 1.2, ((size * casesize + casesize * 2) / 2) / 3.5, surfaceMessage, Sans, Green, Message, renderer, Message_rect);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(5000);
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_FreeSurface(surfaceMessage);
                                SDL_DestroyTexture(Message);
                                SDL_Quit();
                                TTF_Quit();
                                main(argc, argv);
                                return 0;
                            }
                            //Render the game and save its new state as the new main texture
                            SDL_RenderPresent(renderer);
                            SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
                            texture = SDL_CreateTextureFromSurface(renderer, surface);
                            }
                        }
                    //If the right mouse button is pressed
                    if (SDL_BUTTON_RIGHT == event.button.button) {
                        //Print the pressed button
                        printf("Right MB down\n");

                        //If the case we're on is hidden
                        if (table[x * size + y].hide == 0) {
                            //If the case we're on doesn't have a flag yet
                            if (table[x * size + y].flag == 0) {

                                //Update the case
                                table[x * size + y].flag = 1;
                                nbflag -= 1;

                                //Render the game and save its new state as the new main texture
                                SDL_RenderClear(renderer);
                                SDL_RenderCopy(renderer, texture, NULL, &Renderer_rect);
                                SDL_RenderCopy(renderer, flagTexture, NULL, &rectangle);

                                SDL_RenderPresent(renderer);
                                SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
                                texture = SDL_CreateTextureFromSurface(renderer, surface);
                            }
                            //Else if the case we're on does have a flag
                            else if (table[x * size + y].flag == 1) {

                                //Update the case
                                table[x * size + y].flag = 0;
                                nbflag += 1;

                                //Render the game and save its new state as the new main texture
                                SDL_RenderClear(renderer);
                                SDL_RenderCopy(renderer, texture, NULL, &Renderer_rect);

                                SDL_RenderCopy(renderer, caseTexture, NULL, &rectangle);

                                SDL_RenderPresent(renderer);
                                SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
                                texture = SDL_CreateTextureFromSurface(renderer, surface);
                            }
                        }
                    }

                }
            }
        }

        //Framerate end setup
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime == 0)
            frameTime++;

        if (frameDelay > frameTime) {
            frameTime = frameDelay;
            SDL_Delay(frameDelay - frameTime);
        }
    }

    createText("You Win", (viewportheight / 2) / 2, (viewportwidth / 2) / 1.2, ((size * casesize + casesize * 2) / 2) / 3.5, surfaceMessage, Sans, Green, Message, renderer, Message_rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
    TTF_Quit();

    statut = EXIT_SUCCESS;

Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
    exit(0);
}

