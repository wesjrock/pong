/*
Name:  Wesley Tiozzo
N#: 8077925
Institution: ICMC - USP São Carlos
Course: Introduction to Computer Science I


Pong
*/

#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include <time.h>

#define KEY_ESCAPE 27
#define KEY_ENTER 13
#define KEY_W 119
#define KEY_S 115

#define START 1
#define CREDITOS 2
#define EXIT 3
#define MOVE 4

#define R_VERTICAL 0
#define R_HORIZONTAL 1

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

//delay do while, quanto menor, mais rápido
#define GAME_SPEED 10    

#define BAT_COMPRIMENTO 75
#define BAT_LARGURA 15

//quantidade de pixels que se move por vez
#define BAT_SPEED 30    

#define SCORE_MAX 10   

typedef struct
{
    //posições
    int x, y; 

    int length, width;   

    //número de pixels por vez 
    int speed_y;   
} Jogadores; 

typedef struct
{
    int x, y;
    int raio, speed_x, speed_y;
} Bolinha;

typedef struct
{
    int scoreA, scoreB;
} Pontuacao;

void desenhajogo(Jogadores *p1, Jogadores *p2, Bolinha *ball, Pontuacao *pontos);
void initialize(Jogadores *p1, Jogadores *p2, Bolinha *ball, Pontuacao *pontos);
void movimenta_bolinha(Bolinha *ball, Jogadores *p1, Jogadores *p2, Pontuacao *pontos);
void alterar_sentido(Bolinha *ball, int reflexao);
int colide(Bolinha *ball, Jogadores *p);
void desenhamenu(int selecao);
void play();

int main()
{
    int janela, opcao = MOVE;

    char comando;

    //selecao = item selecionado, opcao é pressionado enter no item selecionado
    int selecao = START;     

    //para nao redesenhar sem necessidade
    int flag = 0; 
    char ch;

    //300,300 posicao inicial na tela
    janela = initwindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Teste do Pong", 300, 300); 
    setcurrentwindow(janela);
    setbkcolor(BLACK);

    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    outtextxy((WINDOW_WIDTH/2) - (WINDOW_WIDTH/16), 0, "Pong");

    desenhamenu(START);

    //MENU
    do                           
    {
        flag = 0;
        //se pressionar uma tecla, irá fazer com que o char ch seja a tecla
        if(kbhit())     
        {
            ch = getch();
        }
        else
        {
            ch = '\0';
        }

        switch(ch)
        {
        case KEY_UP:
            if(selecao == START)
            {
                selecao = EXIT;
            }
            else
            {
                selecao--;
            }
            flag = 1;
            break;
        case KEY_DOWN:
            if(selecao == EXIT)
            {
                selecao = START;
            }
            else
            {
                selecao++;
            }
            flag = 1;
            break;
        case KEY_ESCAPE:
            selecao = EXIT;
            flag = 1;
            break;
        case KEY_ENTER:
            opcao = selecao;
            break;
        default:
            break;
        }

        if(flag == 1)
        {
            desenhamenu(selecao);
        }

        if(opcao == START)
        {
            play();
            opcao = MOVE;
        }



    }
    while(opcao != EXIT);

    closegraph(janela);
    return 0;
}

void desenhamenu(int selecao)
{
    /*imagem bloco preto, que irá "atualizar a tela" quando houver uma mudança,
    rapidamente o colocando por cima de tudo e colocando as coisas de volta*/
    readimagefile("bloco.bmp", 0, 20, WINDOW_WIDTH, WINDOW_HEIGHT);      

    outtextxy(30, 100, "Start");
    outtextxy(30, 130, "Credits");
    outtextxy(30, 160, "Exit");

    if(selecao == START)
    {
        rectangle(20, 95,100, 120);
    }
    else if(selecao == CREDITOS)
    {
        rectangle(20, 125, 100, 150);
    }
    else if(selecao == EXIT)
    {
        rectangle(20, 155, 100, 180);
    }
}

void initialize(Jogadores *p1, Jogadores *p2, Bolinha *ball, Pontuacao *pontos)    
{
    int temp;
    srand(time(NULL)); 

    //setar player 1
    p1->x = 15;
    p1->y = (WINDOW_HEIGHT/2) - (WINDOW_HEIGHT/8);
    p1->length = BAT_COMPRIMENTO;
    p1->width = BAT_LARGURA;
    p1->speed_y = BAT_SPEED;

    //setar player 2
    p2->x = WINDOW_WIDTH - 30;
    p2->y = (WINDOW_HEIGHT/2) - (WINDOW_HEIGHT/8);
    p2->length = BAT_COMPRIMENTO;
    p2->width = BAT_LARGURA;
    p2->speed_y = BAT_SPEED;

    //setar bolinha
    ball->x = (WINDOW_WIDTH/2);
    ball->y = (WINDOW_HEIGHT/2);
    ball->raio = 3;

    //50% de chance de temp ser negativo, 0 ou 1
    if(rand()%2) 
    {
        temp = 1;
    }
    else
    {
        temp = -1;
    }

    //evitou cair no 0, bola inicia na direcao indicada pelo temp
    ball->speed_x = ((rand()%5)+1) * temp; 
    //pode ser 0, pois sobrara o valor de x, pode ser negativo
    ball->speed_y = rand()%6 - rand()%6; 

    //setar pontuacao
    pontos->scoreA = 0;
    pontos->scoreB = 0;

}

void play()
{
    Jogadores player1, player2;
    Bolinha ball;
    Pontuacao cont_pontos;
    char ch;

    desenhajogo(&player1, &player2, &ball, &cont_pontos);

    initialize(&player1, &player2, &ball, &cont_pontos);

    while((cont_pontos.scoreA) != SCORE_MAX && (cont_pontos.scoreB) != SCORE_MAX)
    {
        //se pressionar tacla, vai capturar e movimentar a barrinha
        if(kbhit())       
        {
            ch = getch();
        }
        else
        {
            ch = '\0';
        }
        switch(ch)
        {
        case KEY_W:
            if (player1.y>0){
            player1.y += -player1.speed_y;    //pra cima é em direcao ao y 0
            }
            break;

        case KEY_S:
            if (player1.y + player1.length<WINDOW_HEIGHT){
            player1.y += +player1.speed_y;
            }
            break;
        case KEY_UP:
            if (player2.y>0){
            player2.y += -player2.speed_y;
            }
            break;
        case KEY_DOWN:
            if (player2.y + player2.length<WINDOW_HEIGHT){
            player2.y += +player2.speed_y;
            }
            break;
        case KEY_ESCAPE:
            break;
        }
        movimenta_bolinha(&ball, &player1, &player2, &cont_pontos);
        // Redesenhando
        desenhajogo(&player1, &player2, &ball, &cont_pontos); 
        delay(GAME_SPEED);
    }
    cleardevice();
    if (cont_pontos.scoreA){
        outtextxy ((WINDOW_WIDTH/2) - (WINDOW_WIDTH/8) - (WINDOW_WIDTH/32), (WINDOW_HEIGHT/2) - WINDOW_HEIGHT/8, "JOGADOR 2 GANHOU!");
    }
    else outtextxy ((WINDOW_WIDTH/2) - (WINDOW_WIDTH/8) - (WINDOW_WIDTH/32), (WINDOW_HEIGHT/2) - WINDOW_HEIGHT/8, "JOGADOR 1 GANHOU!");

}

void movimenta_bolinha(Bolinha *ball, Jogadores *p1, Jogadores *p2, Pontuacao *pontos)
{
    int previsao;

    //Previsao da posicao que a bolinha tera se ela se mover dessa vez, ou seja, nova coordenada
    previsao = ball->x + ball->speed_x; 
    if(colide(ball, p1) || colide(ball, p2) )
    {
        alterar_sentido(ball, R_HORIZONTAL);
    }
    else if(previsao < 0)
    {
        pontos->scoreB++;
        alterar_sentido(ball, R_HORIZONTAL);
    }
    else if(previsao > WINDOW_WIDTH)
    {
        pontos->scoreA++;
        alterar_sentido(ball, R_HORIZONTAL);
    }

    //se x = 10, e speed = 3, nova posicao sera 13
    ball->x += ball->speed_x; 

    //colisão vertical
    previsao = ball->y + ball->speed_y;   
    if(previsao < 0 || previsao > WINDOW_HEIGHT)
    {
        alterar_sentido(ball, R_VERTICAL);
    }
    ball->y += ball->speed_y;
}

int colide(Bolinha *ball, Jogadores *p)
{
    int bx = ball->x;
    int by = ball->y;
    int pxi = p->x;
    int pxf = p->x + p->width;
    int pyi = p->y;
    int pyf = p->y + p->length;

    // Verificando se a bolinha está em cima da barra
    if(bx < pxf && bx > pxi && by > pyi && by < pyf) 
    {
       return 1;
    }

    return 0;
}

void alterar_sentido(Bolinha *ball, int reflexao)
{
    switch(reflexao)
    {
        case R_VERTICAL:
            ball->speed_y *= -1;
            break;
        case R_HORIZONTAL:
            ball->speed_x *= -1;
            break;
    }
}

void desenhajogo(Jogadores *p1, Jogadores *p2, Bolinha *ball, Pontuacao *pontos)
{
    char score[5];

    readimagefile("bloco.bmp", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    sprintf(score,"%d",pontos->scoreA);
    outtextxy(30, 30, "Player 1");
    outtextxy(30, 60, "Score: ");
    outtextxy(80, 60, score);

    sprintf(score,"%d",pontos->scoreB);
    outtextxy(WINDOW_WIDTH - 100, 30, "Player 2");
    outtextxy(WINDOW_WIDTH - 100, 60, "Score: ");
    outtextxy(WINDOW_WIDTH - 50, 60, score);

    //Barra do Player 1
    bar(p1->x, p1->y, (p1->x)+(p1->width), (p1->y)+(p1->length));
    //Barra do Player 2  
    bar(p2->x, p2->y, (p2->x)+(p2->width), (p2->y)+(p2->length));  

    circle(ball->x, ball->y, ball->raio);
}
