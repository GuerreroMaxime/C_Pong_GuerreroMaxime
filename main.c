#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 500


typedef struct game{   //type structure


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_ptexture;
     SDL_Surface *g_psurface;


}game;

typedef struct gameState{

    int g_bRunning;


}gameState;

typedef struct coordonnees{ //POur les coordonnées des points

    double x;
    double y;

}coordonnees;

typedef enum Hauteur      //Pour controler la position en Y
{
    Montee,Descente
} Hauteur;

typedef enum Largeur      //Pour controler la position en X
{
    Gauche,Droite
} Largeur;

//prototypes
int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame);
void destroy(game *myGame);
void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2,coordonnees *dep3);
Largeur LargeurBalle(coordonnees *dep3, Largeur sens_w);
Hauteur HauteurBalle(coordonnees *dep3,Hauteur sens_h);
void delay(unsigned int frameLimit);
void renderTexture(game *myGame,coordonnees *dep,coordonnees *dep2 ,coordonnees *dep3);




int main(int argc, char *argv[])
{


     game myGame;
     gameState state;

     unsigned int frameLimit = SDL_GetTicks() + 16;
     coordonnees dep;
     coordonnees dep2;
     coordonnees dep3;
     dep3.x=(SCREEN_WIDTH/2)-10;
     dep3.y=(SCREEN_HEIGHT/2)-10;
     int choix=0;
     Hauteur sens_h;
     Largeur sens_w;
     sens_h=Descente;
     sens_w=Droite;
     state.g_bRunning=1;


        myGame.g_ptexture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);
        while(state.g_bRunning){
        if (choix==0)
        {
            printf("1:Pong\n Control J1: Z pour monter et S pour décendre\n J1: Fleche Haut pour monter et Fleche Bas pour décendre\n");
            scanf("%i",&choix);
        }
        if (choix==1){
        init("Chapter 1 setting up SDL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame);
            while(state.g_bRunning){    //Lancement de la partie

                                    sens_h=HauteurBalle(&dep3,sens_h);
                                    sens_w=LargeurBalle(&dep3,sens_w);
                                    handleEvents(&state,&dep,&dep2,&dep3);
                                    renderTexture(&myGame,&dep,&dep2,&dep3);
                                    }

                 }
                 system ("cls");

            }delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16;

        destroy(&myGame);

        SDL_Quit();


    return 0;
}


int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame){


    myGame->g_pWindow=NULL;
    myGame->g_pRenderer=NULL;
    myGame->g_psurface=NULL;
    myGame->g_ptexture=NULL;

    //initialize SDL

    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {

            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);


            }


    }else{


        return 0;
    }



    return 1;



}




void renderTexture(game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *dep3) {


       //Définition du rectangle 1 a dessiner
        SDL_Rect rectangle;
        rectangle.x=0;//debut x
        rectangle.y=dep->y;//debut y
        rectangle.w=10; //Largeur
        rectangle.h=100; //Hauteur


        //Définition du rectangle 2 a dessiner
        SDL_Rect rectangle2;
        rectangle2.x=SCREEN_WIDTH-10;//debut x
        rectangle2.y=dep2->y;//debut y
        rectangle2.w=10; //Largeur
        rectangle2.h=100; //Hauteur


        //Définition de la balle a dessiner
        SDL_Rect ballon;
        ballon.x=dep3->x;//debut x
        ballon.y=dep3->y;//debut y
        ballon.w=15; //Largeur
        ballon.h=15; //Hauteur

        //Draw in texture
        SDL_SetRenderDrawColor(myGame->g_pRenderer,230,230,230,230);
        SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture); //on modifie la texture

        SDL_RenderFillRect(myGame->g_pRenderer, &rectangle);
        SDL_RenderFillRect(myGame->g_pRenderer, &rectangle2);
        SDL_RenderFillRect(myGame->g_pRenderer, &ballon);

        SDL_SetRenderTarget(myGame->g_pRenderer, NULL);// Dorénavent, on modifie à nouveau le renderer

        SDL_Rect position;//position de la texture dans screen
        position.x = 0;
        position.y = 0;
        SDL_QueryTexture(myGame->g_ptexture, NULL, NULL, &position.w, &position.h);
        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&position);

         SDL_RenderPresent(myGame->g_pRenderer);
         SDL_SetRenderDrawColor(myGame->g_pRenderer,800,800,800,800);
         SDL_DestroyTexture(myGame->g_ptexture);
         SDL_RenderClear(myGame->g_pRenderer);




}

void destroy(game *myGame){


     if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);

     if(myGame->g_ptexture!=NULL)
        SDL_DestroyTexture(myGame->g_ptexture);


     if(myGame->g_psurface!=NULL)
         SDL_FreeSurface(myGame->g_psurface);


    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}


void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2,coordonnees *dep3){


    SDL_Event event;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=0;break;
        case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                            {
                                case SDLK_UP:    dep2->y-=7; break;
                                case SDLK_DOWN:  dep2->y+=7; break;
                                case SDLK_z:     dep->y-=7; break;
                                case SDLK_s:     dep->y+=7;; break;
                            }
                            break;

        case SDL_KEYUP:;break;

        default:break;

        }
    }


}

Hauteur HauteurBalle(coordonnees *dep3,Hauteur sens_h)
{
 if (sens_h==Montee)
{
    dep3->y-=2;
}
else if (sens_h==Descente)
{
 dep3->y+=2;
}
if (dep3->y>SCREEN_HEIGHT-20)
{
    sens_h=Montee;
}
else if (dep3->y<0)
{
    sens_h=Descente;
}
return sens_h;
}



Largeur LargeurBalle(coordonnees *dep3,Largeur sens_w)
{
 if (sens_w==Gauche)
{
    dep3->x-=2;
}
else if (sens_w==Droite)
{
    dep3->x+=2;
}
if (dep3->x>SCREEN_WIDTH-20)
{
    sens_w=Gauche;
}
else if (dep3->x<0)

{
    sens_w=Droite;
}
return sens_w;
}

void delay(unsigned int frameLimit)
{
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}
