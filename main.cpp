#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h> /* Inclusion du header de SDL_image (adapter le dossier au besoin) */
int etat;
SDL_Surface *ecran = NULL;
int suduko [9][9];
int sudukoRecente [9][9];
int iS,jS;
void pause();

bool non_pleine(int grille[9][9])
{
  int i,j;
  for(i=0;i<9;i++) //initialisation du matrice
     {
        for(j=0;j<9;j++)
           if (suduko[i][j]!=0)
                   return false;
     }
     return true;
}

void initialiser_grille(int grille[9][9])
{
  int i,j;
  for(i=0;i<9;i++) //initialisation du matrice
     {
        for(j=0;j<9;j++)
           suduko[i][j]=0;

     }
}

void copier_grille(int grille[9][9],int grille1[9][9])
{
  int i,j;
  for(i=0;i<9;i++)
     {
        for(j=0;j<9;j++)
           grille[i][j]=grille1[i][j];

     }
}

bool absentSurLigne (int k, int grille[9][9], int i)
{
int j;
    for (j=0; j < 9; j++)

        if (grille[i][j] == k)

            return false;

    return true;

}

bool absentSurColonne (int k, int grille[9][9], int j)

{
int i;
    for ( i=0; i < 9; i++)

        if (grille[i][j] == k)

            return false;

    return true;

}

bool absentSurBloc (int k, int grille[9][9], int i, int j)

{

    int _i = i-(i%3), _j = j-(j%3);  // ou encore : _i = 3*(i/3), _j = 3*(j/3);

    for (i=_i; i < _i+3; i++)

        for (j=_j; j < _j+3; j++)

            if (grille[i][j] == k)

                return false;

    return true;

}

bool estValide (int grille[9][9], int position)

{
int k;
    if (position == 9*9)

        return true;

    int i = position/9, j = position%9;

    if (grille[i][j] != 0)

        return estValide(grille, position+1);

    for ( k=1; k <= 9; k++)

    {

        if (absentSurLigne(k,grille,i) && absentSurColonne(k,grille,j) && absentSurBloc(k,grille,i,j))

        {

            grille[i][j] = k;

            if ( estValide (grille, position+1) )

                return true;

        }

    }

    grille[i][j] = 0;

    return false;

}

void dessinerEcran()
{
    SDL_Surface *imageDeFond = NULL;
    SDL_Rect positionFond;
    SDL_Rect az;
    int i,j;
    switch(etat)
    {

        case 0:
            imageDeFond = IMG_Load("fenetre1.jpg");
            positionFond.x = 0;
            positionFond.y = 0;
            SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);

        break;
        case 1:
            imageDeFond = IMG_Load("fenetre2.jpg");
            positionFond.x = 0;
            positionFond.y = 0;
            SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);
            az.x = (iS*46)+63;//az rectangle seléctionée
            az.y = (jS*46)+168;
            imageDeFond = IMG_Load("selection.jpg");
            SDL_BlitSurface(imageDeFond, NULL, ecran, &az);
            for(i=0;i<9;i++)
            {
                for(j=0;j<9;j++)
                {
                    if(suduko[i][j]>0)
                    {
                        char fname[20]="numero .png";//ajouter ttes les nbres 1-->9 number .jpg
                        az.x = (i*46)+63;
                        az.y = (j*46)+168;
                        fname[6]=suduko[i][j]+48;
                        imageDeFond = IMG_Load(fname);
                        SDL_BlitSurface(imageDeFond, NULL, ecran, &az);
                    }
                }
            }
        break;
        case 2:

           copier_grille(sudukoRecente,suduko);
           if (estValide(suduko,0))
                 imageDeFond = IMG_Load("fenetre3.jpg");
           else
             {
               imageDeFond = IMG_Load("fenetre4.jpg");
             }
            positionFond.x = 0;
            positionFond.y = 0;
            az.x = (iS*46)+63;//az rectangle seléctionée
            az.y = (jS*46)+168;
            SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);

            //ajouter boucle for afficher la matrice par par les images "boucle précédent
            for(i=0;i<9;i++)
            {
                for(j=0;j<9;j++)
                {
                    char fname[20]="numer  .png";//ajouter ttes les nbres 1-->9 number .jpg

                    if (sudukoRecente[i][j]==0)
                        fname[5]='a';
                    if (sudukoRecente[i][j]!=0)
                        fname[5]='o';

                        az.x = (i*46)+63;
                        az.y = (j*46)+168;
                        fname[6]=suduko[i][j]+48;
                        imageDeFond = IMG_Load(fname);
                        SDL_BlitSurface(imageDeFond, NULL, ecran, &az);
                }
            }
        break;
    }
    SDL_Flip(ecran);
}


int main(int argc, char *argv[])
{

    SDL_Init(SDL_INIT_VIDEO);

    SDL_WM_SetIcon(IMG_Load("icone.bmp"), NULL);

    ecran = SDL_SetVideoMode(950, 594, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("Chargement d'images en SDL", NULL);
    etat=0;

    iS=0;
    jS=0;

    pause();

    SDL_Quit();

    return EXIT_SUCCESS;
}

void pause()
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        dessinerEcran();
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
            break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    int posX,posY;
                    SDL_GetMouseState(&posX,&posY);
                    if(etat==0)
                    {
                        if(posX<267&&posY<447&&posX>104&&posY>416)
                        {
                            initialiser_grille(suduko);
                            etat=1;
                        }

                        if(posX<267&&posY<491&&posX>104&&posY>460)
                        {
                         SDL_Quit();
                        }
                    }
                    else
                    {
                        if(etat==1)
                        {
                            if(posX>51&&posY>158&&posX<463&&posY<570)
                            {
                                iS=(posX-63)/46;//is et js position d'élément d'un matrice
                                jS=(posY-168)/46;
                            }
                            if((posX<660&&posY<245&&posX>512&&posY>212) &&(!non_pleine(suduko)))
                            {
                              etat=2;
                            }

                            if(posX<660&&posY<201&&posX>512&&posY>170)
                            {
                              initialiser_grille(suduko);
                              etat=0;
                            }
                        }

                        else
                            if (etat==2)
                        {

                            if(posX<660&&posY<245&&posX>512&&posY>212)
                            {
                              initialiser_grille(suduko);
                              etat=1;
                            }

                            if(posX<660&&posY<201&&posX>512&&posY>170)
                            {
                              initialiser_grille(suduko);
                              etat=0;
                            }
                        }
                    }

                }
            break;
            case SDL_KEYUP :
                if(etat==1)
                {
                    switch(event.key.keysym.sym)  //evenement de clavier ya9ra ay nombre yod5ol i5alih automatiquement fil matrice
                    {

                        case SDLK_DELETE:
                            suduko[iS][jS]=0;
                        break;
                        case SDLK_KP1:
                            suduko[iS][jS]=1;
                        break;
                        case SDLK_KP2:
                            suduko[iS][jS]=2;
                        break;
                        case SDLK_KP3:
                            suduko[iS][jS]=3 ;
                        break;
                        case SDLK_KP4:
                            suduko[iS][jS]=4;
                        break;
                        case SDLK_KP5:
                            suduko[iS][jS]=5;
                        break;
                        case SDLK_KP6:
                            suduko[iS][jS]=6;
                        break;
                        case SDLK_KP7:
                            suduko[iS][jS]=7;
                        break;
                        case SDLK_KP8:
                            suduko[iS][jS]=8;
                        break;
                        case SDLK_KP9:
                            suduko[iS][jS]=9;
                        break;
                    }
                }
            break;
        }
    }
}

