/*********************Douglas Santos e Erich Morais*************************/
/******************FUNDAMENTOS DE SISTEMA OPERACIONAIS**********************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#include "driver.h"

#define TAMANHO_LINHA 9
#define TAMANHO_COLUNA 9

#define LO 0
#define CO 1
#define LD 2
#define CD 3
#define DIR 4

#define DEVICE_NAME "/dev/damas"      
#define BUF_LEN 1024

void escreve_tabuleiro(char matrix[TAMANHO_LINHA][TAMANHO_LINHA]){
    int linha, coluna, cont1 = 0, cont2 = 0;
    printf("\n");
    printf("    1   2   3   4   5   6   7   8\n");
    printf("  +---+---+---+---+---+---+---+---+\n");
    for(linha = 1 ; linha < TAMANHO_LINHA ; linha ++){
        printf("%d |", linha );
        for ( coluna = 1; coluna < TAMANHO_COLUNA ; coluna++){
            printf(" %c ", matrix[linha][coluna]);
            if(matrix[linha][coluna] == 'x' || matrix[linha][coluna] == 'X') cont1++;
            else if(matrix[linha][coluna] == 'o'|| matrix[linha][coluna] == 'O') cont2++;
            printf("|");
        }
        printf("\n");
        printf("  +---+---+---+---+---+---+---+---+\n");
    }
    printf("   Jogador 1: %d    Jogador 2: %d\n", cont1, cont2);
}

int solicita_player(){
    int r;
    printf("Deseja jogar como player 1 ou 2? ");
    printf("\nPlayer: ");
    scanf("%d", &r);

    if(r==1) return 1;
    else if(r==2) return 2;
    else return 0;
}

int praCima(char matrix[TAMANHO_LINHA][TAMANHO_LINHA], int mov[4], char peca, char inimigo, int player){
    inimigo = tolower(inimigo);
    peca = tolower(peca);
    if(matrix[mov[LD]][mov[CD]] == matrix[mov[LO]][mov[CO]]) return 0;
    else if(matrix[mov[LD]][mov[CD]] == inimigo && (matrix[mov[LD]-1][mov[CD]+mov[DIR]] == inimigo)) return 0;
    else if(matrix[mov[LD]][mov[CD]] == inimigo && (matrix[mov[LD]-1][mov[CD]+mov[DIR]] == peca)) return 0;      
    else if(matrix[mov[LD]][mov[CD]] == inimigo && (matrix[mov[LD]-1][mov[CD]+mov[DIR]] == '-') && (mov[CD]+mov[DIR]>8 || mov[CD]+mov[DIR]<1 || (mov[LD]-1) < 1 )) return 0;
    else if(matrix[mov[LD]][mov[CD]] == inimigo && (matrix[mov[LD]-1][mov[CD]+mov[DIR]] == '-')){
        matrix[mov[LD]][mov[CD]] = '-';
        if(mov[DIR] == +1 ){
            mov[LD] -= 1;
            mov[CD] += 1;   
            if(matrix[mov[LD]- 1][mov[CD]+1] == inimigo && matrix[mov[LD]-2][mov[CD]+2] == '-'&& mov[CD]+2<9  && (mov[LD]-2) > 1){
                matrix[mov[LD]-1][mov[CD]+1] = '-';
                mov[LD] -= 2;
                mov[CD] += 2;
                praCima(matrix, mov,peca,inimigo, player);
                return 1;
            }
            praCima(matrix, mov,peca,inimigo, player);
            return 1;
        }
        else{     
            mov[LD] -= 1;
            mov[CD] -= 1;
            if(matrix[mov[LD]-1][mov[CD]-1] == inimigo && matrix[mov[LD]-2][mov[CD]-2] == '-' && mov[CD]+2<9 && mov[CD]-2>0 && (mov[LD]-2) > 0){      
                matrix[mov[LD]-1][mov[CD]-1] = '-'; 
                mov[LD] -= 2;
                mov[CD] -= 2;
                praCima(matrix, mov,peca,inimigo, player); 
                return 1;
            }
            praCima(matrix, mov,peca,inimigo, player);
            return 1;
        }         
    }  
    else return 1;   
}

int praBaixo(char matrix[TAMANHO_LINHA][TAMANHO_LINHA], int mov[4], char peca, char inimigo, int player){
    inimigo = tolower(inimigo);
    peca = tolower(peca);
    if(matrix[mov[LD]][mov[CD]] == matrix[mov[LO]][mov[CO]] ) return 0;
    else if(matrix[mov[LD]][mov[CD]] == inimigo && (matrix[mov[LD]+1][mov[CD]+mov[DIR]] == inimigo)) return 0;
    else if(matrix[mov[LD]][mov[CD]] == inimigo && (matrix[mov[LD]+1][mov[CD]+mov[DIR]] == peca)) return 0;
    else if(matrix[mov[LD]][mov[CD]] == inimigo && (matrix[mov[LD]+1][mov[CD]+mov[DIR]] == '-') && (mov[CD]+mov[DIR]>8 || mov[CD]+mov[DIR]<1 || (mov[LD]+1) > 8)) return 0;
    else if(matrix[mov[LD]][mov[CD]] == inimigo && matrix[mov[LD]+1][mov[CD]+mov[DIR]] == '-'){
        matrix[mov[LD]][mov[CD]] = '-';
        if(mov[DIR] == -1){
            mov[LD] += 1;
            mov[CD] -= 1;    
            if(matrix[mov[LD]+1][mov[CD]-1] == inimigo && matrix[mov[LD]+2][mov[CD]-2] == '-' && mov[CD]+2<9 && mov[CD]-2>0 && (mov[LD]+2) < 9){
                matrix[mov[LD]+1][mov[CD]-1] = '-';
                mov[LD] += 2;
                mov[CD] -= 2;
                praBaixo(matrix, mov,peca,inimigo, player);  
                return 1;
            }
            praBaixo(matrix, mov,peca,inimigo, player);  
            return 1;
        }
        else{
            mov[LD] += 1;
            mov[CD] += 1;
            if(matrix[mov[LD]][mov[CD]] == inimigo && matrix[mov[LD]+2][mov[CD]+2] == '-' && mov[CD]+2<9 && mov[CD]-2>0 && (mov[LD]+2) < 9){
                matrix[mov[LD]+1][mov[CD]+1] = '-';
                mov[LD] += 2;
                mov[CD] += 2;
                praBaixo(matrix, mov,peca,inimigo, player);  
                return 1;
            }
            praBaixo(matrix, mov,peca,inimigo, player);  
            return 1;
        }   
        praBaixo(matrix, mov,peca,inimigo, player);  
        return 1;
    }
    else return 1;
}

int valida_movimento(char matrix[TAMANHO_LINHA][TAMANHO_LINHA], int mov[4], int player){
  
    int flag;
    //condicoes de pecas simples
    //x
    if(matrix[mov[LO]][mov[CO]] == 'x' && player == 1){
        if( mov[0] > 8 ||  mov[1] > 8 ||  mov[2] > 8 ||  mov[3] > 8 || mov[0] < 1 ||  mov[1] < 1 ||  mov[2] < 1 ||  mov[3] < 1 || mov[CD] < mov[CO]-1 || mov[CD] > mov[CO]+1 || mov[LO] == mov[LD] || mov[CO] == mov[CD] || mov[LD] > mov[LO])
            return 0;
        else{
                flag = praCima(matrix, mov,'x','o', player);
                return flag;
            }        
    }
    //o
    else if(matrix[mov[LO]][mov[CO]] == 'o' && player == 2){
        if(mov[0] > 8 ||  mov[1] > 8 ||  mov[2] > 8 ||  mov[3] > 8 || mov[0] < 1 ||  mov[1] < 1 ||  mov[2] < 1 ||  mov[3] < 1 || mov[CD] < mov[CO]-1 || mov[CD] > mov[CO]+1 || mov[LO] == mov[LD] || mov[CO] == mov[CD] || mov[LD] < mov[LO])
            return 0;
        else{
                flag = praBaixo(matrix, mov,'o','x', player);
                return flag;
            }        
    }
    //condições de peças reis
    //X
    else if(matrix[mov[LO]][mov[CO]] == 'X' && player == 1){
        if(mov[0] > 8 ||  mov[1] > 8 ||  mov[2] > 8 ||  mov[3] > 8 || mov[0] < 1 ||  mov[1] < 1 ||  mov[2] < 1 ||  mov[3] < 1 || mov[CD] < mov[CO]-1 || mov[CD] > mov[CO]+1 || mov[LO] == mov[LD] || mov[CO] == mov[CD])
            return 0;
        else{
                if(mov[LD] < mov[LO]) flag = praCima(matrix, mov,'X', 'o', player);
                else if(mov[LD] > mov[LO]) flag = praBaixo(matrix, mov,'X', 'o', player);
                return flag;
            }        
    }
    //O
    else if(matrix[mov[LO]][mov[CO]] == 'O' && player == 2){
        if( mov[0] > 8 ||  mov[1] > 8 ||  mov[2] > 8 ||  mov[3] > 8 || mov[0] < 1 ||  mov[1] < 1 ||  mov[2] < 1 ||  mov[3] < 1 || mov[CD] < mov[CO]-1 || mov[CD] > mov[CO]+1 || mov[LO] == mov[LD] || mov[CO] == mov[CD])
            return 0;
        else{
                if(mov[LD] < mov[LO]) flag = praCima(matrix, mov,'O', 'x', player);
                else if(mov[LD] > mov[LO]) flag = praBaixo(matrix, mov,'O', 'x', player);
                return flag;         
            }        
    }
    else{
        return 0;
    }
}
//funcao em desenvvolvimento
void obrigado(char matrix[TAMANHO_LINHA][TAMANHO_COLUNA], int forcedX[TAMANHO_LINHA*TAMANHO_COLUNA], int forcedO[TAMANHO_LINHA*TAMANHO_COLUNA]){
    int linha, coluna;
    int i = 0,j = 0, d;
    char inimigo, elemento;
    for(linha = 1; linha < TAMANHO_LINHA ; linha++){
        for(coluna = 1; coluna < TAMANHO_COLUNA; coluna++){
            if((linha-1 ) >= 0 && (linha+1) <= 9 && (coluna-1)>= 0 && (coluna+1)<=9){
                elemento = matrix[linha][coluna];
                switch(elemento){
                    case '-': 
                        break;
                    case 'x': inimigo = 'o';
                        if(tolower(matrix[linha-1][coluna+1]) == inimigo && matrix[linha-2][coluna+2] == '-' && (coluna+2) < 9 && (linha-2)>0) {
                            d = 1;
                            forcedX[i] = linha*1000+coluna*100+(linha-1)*10+(coluna+d);
                            i++;
                        }
                        if(tolower(matrix[linha-1][coluna-1]) == inimigo && matrix[linha-2][coluna-2] == '-' && (coluna-2) > 0 && (linha-2)>0){
                            d = -1;
                            forcedX[i] = linha*1000+coluna*100+(linha-1)*10+(coluna+d);
                            i++;
                        } 
                        break;
                    case 'o': inimigo = 'x';
                        if(tolower(matrix[linha+1][coluna+1]) == inimigo && matrix[linha+2][coluna+2] == '-' && (coluna+2) < 9  && (linha+2)<9 ) {
                            d = 1;
                            forcedO[j] = linha*1000+coluna*100+(linha+1)*10+(coluna+d);
                            j++;
                        }
                        if(tolower(matrix[linha+1][coluna-1]) == inimigo && matrix[linha+2][coluna-2] == '-' && (coluna-2) > 0 && (linha+2)<9) {
                            d = -1;
                            forcedO[j] = linha*1000+coluna*100+(linha+1)*10+(coluna+d);
                            j++;                    
                        }
                        break;
                    case 'X': inimigo = 'o'; 
                        if(tolower(matrix[linha+1][coluna+1]) == inimigo && matrix[linha+2][coluna+2] == '-') {
                            d = 1;
                            forcedX[i] = linha*1000+coluna*100+(linha+1)*10+(coluna+d);
                            i++;
                        }
                        if(tolower(matrix[linha+1][coluna-1]) == inimigo && matrix[linha+2][coluna-2] == '-') {
                            d = -1;
                            forcedX[i] = linha*1000+coluna*100+(linha+1)*10+(coluna+d);
                            i++;                    
                        }
                        if(tolower(matrix[linha-1][coluna+1]) == inimigo && matrix[linha-2][coluna+2] == '-') {
                            d = 1;
                            forcedX[i] = linha*1000+coluna*100+(linha-1)*10+(coluna+d);
                            i++;
                        }
                        if(tolower(matrix[linha-1][coluna-1]) == inimigo && matrix[linha-2][coluna-2] == '-'){
                            d = -1;
                            forcedX[i] = linha*1000+coluna*100+(linha-1)*10+(coluna+d);
                            i++;
                        } 
                        break;
                    case 'O': inimigo = 'x'; 
                        if(tolower(matrix[linha+1][coluna+1]) == inimigo && matrix[linha+2][coluna+2] == '-') {
                            d = 1;
                            forcedO[j] = linha*1000+coluna*100+(linha+1)*10+(coluna+d);
                            j++;
                        }
                        if(tolower(matrix[linha+1][coluna-1]) == inimigo && matrix[linha+2][coluna-2] == '-') {
                            d = -1;
                            forcedO[j] = linha*1000+coluna*100+(linha+1)*10+(coluna+d);
                            j++;                    
                        }
                        if(tolower(matrix[linha-1][coluna+1]) == inimigo && matrix[linha-2][coluna+2] == '-') {
                            d = 1;
                            forcedO[j] = linha*1000+coluna*100+(linha-1)*10+(coluna+d);
                            j++;
                        }
                        if(tolower(matrix[linha-1][coluna-1]) == inimigo && matrix[linha-2][coluna-2] == '-'){
                            d = -1;
                            forcedO[j] = linha*1000+coluna*100+(linha-1)*10+(coluna+d);
                            j++;
                        } 
                        break;   
                }   
            }
        }
    }
}

int solicita_jogada(char matrix[TAMANHO_LINHA][TAMANHO_LINHA], int player){
   
    int aux1, i = 0, j = 0;
    int mov[5];
    int flag = 1;
    int forcedX[TAMANHO_LINHA*TAMANHO_COLUNA];
    int forcedO[TAMANHO_LINHA*TAMANHO_COLUNA];
    
    for( int i = 0; i < TAMANHO_COLUNA*TAMANHO_LINHA ; i++) forcedX[i] = 0;
    for( int i = 0; i < TAMANHO_COLUNA*TAMANHO_LINHA ; i++) forcedO[i] = 0;

    obrigado(matrix, forcedX, forcedO);
    if((forcedX[0] != 0 && player ==1) || (forcedO[0] != 0 && player ==2)){
        flag = 0;
        printf("\nVocê tem uma jogada obrigatoria a fazer, elimine a peça do adversario\n");
        i = 0;
        /* if(player == 1){
            while(forcedX[i]){
                printf("%d%d --> %d%d |", forcedX[i]/1000, (forcedX[i]/100)%10, (forcedX[i]/10)%10, (forcedX[j]%10));
                i++;
            }
        }
        else if(player == 2)
        {
            while(forcedO[i]){
                printf("%d%d --> %d%d |", forcedO[i]/1000, (forcedO[i]/100)%10, (forcedO[i]/10)%10, (forcedX[j]%10));
                i++;
            }
        }
        printf("\n");*/
        
    } 


    printf("Digite a LINHA e COLUNA de ORIGEM: ");
    scanf("%d",&aux1);
    mov[LO] = aux1/10;
    mov[CO] = aux1%10;
    printf("Digite a LINHA e COLUNA de DESTINO: ");
    scanf("%d",&aux1);
    mov[LD] = aux1/10;
    mov[CD] = aux1%10;
 
    if(mov[CD] > mov[CO]){
        mov[DIR] = +1;
    }
    else if(mov[CD] < mov[CO]) {
        mov[DIR] = -1;
    }

   /*  printf("0: %d 1: %d 2: %d 3: %d 4: %d",forced[0],forced[0]/1000,
        (forced[0]/100)%10, 
        (forced[0]/10)%10,
        (forced[0]%10));*/

    if( player == 1){
        while(forcedX[j]){
            if( mov[LO] == forcedX[j]/1000 &&
            mov[CO] == (forcedX[j]/100)%10 && 
            mov[LD] == (forcedX[j]/10)%10 &&
            mov[CD] == (forcedX[j]%10))
            flag = 1;
            j++;
        }
    }
    else if( player == 2){
        while(forcedO[j]){
            if( mov[LO] == forcedO[j]/1000 &&
            mov[CO] == (forcedO[j]/100)%10 && 
            mov[LD] == (forcedO[j]/10)%10 &&
            mov[CD] == (forcedO[j]%10))
            flag = 1;
            j++;
        }
    }
    if(flag){
        int aux = valida_movimento(matrix, mov, player);
        if(aux == 1){
            matrix[mov[LD]][mov[CD]] = matrix[mov[LO]][mov[CO]];
            matrix[mov[LO]][mov[CO]] = '-';
            if(matrix[1][mov[CD]] == 'x') matrix[1][mov[CD]] = 'X';
            else if(matrix[8][mov[CD]] == 'o') matrix[mov[LD]][mov[CD]] = 'O';
        
            return 1;
        }
        /* Regra antiga(?), em que o jogador que come, pode jogar novamente
        else if(aux == 2) {
            matrix[mov[LD]][mov[CD]] = matrix[mov[LO]][mov[CO]];
            matrix[mov[LO]][mov[CO]] = '-';
            if(matrix[1][mov[CD]] == 'x') matrix[1][mov[CD]] = 'X';
            else if(matrix[8][mov[CD]] == 'o') matrix[mov[LD]][mov[CD]] = 'O';
            printf("ENTREI NO AUX 2");
            return 2;
        }*/
        else if(aux == 0){
            printf("\n**********************Movimento invalido**************************");
            return 0;
        }
    }
    else{
        printf("\n-------------------FACA A JOGADA OBRIGATORIA---------------------------");
        return 0;
    }
}

int verifica_vitoria(char matrix[TAMANHO_LINHA][TAMANHO_COLUNA]){

    int linha, coluna;
    int cont1=0, cont2=0;
    int winner = 0;

    for (linha = 1; linha < TAMANHO_LINHA; linha++)  {
        for (coluna = 1; coluna < TAMANHO_COLUNA; coluna++) {
            if(matrix[linha][coluna] == 'x' || matrix[linha][coluna] == 'X') cont1++;
            else if (matrix[linha][coluna] == 'o' || matrix[linha][coluna] == 'O') cont2++;
        }
    }

    if(cont1 == 0) return 2; 
    else if (cont2 == 0) return 1;
    else return 0;
}

//Monta um vetor com os valores da matriz do game
void montaIoctl(char matrix[TAMANHO_LINHA][TAMANHO_COLUNA], char matrixString[BUF_LEN]){
    int i=0;
    for (int linha = 1; linha < TAMANHO_LINHA; linha++){
        for (int coluna = 1; coluna < TAMANHO_COLUNA; coluna++){
            matrixString[i] = matrix[linha][coluna]; 
            i++;
        }
    } 
}

//Le o vetor da IOCTL e atualiza matriz do jogo
void le_tabuleiro(char matrix[TAMANHO_LINHA][TAMANHO_COLUNA], char retorno[BUF_LEN]){   
    int k=0;
    for (int linha = 1; linha < TAMANHO_LINHA; linha++) {
        for (int coluna = 1; coluna < TAMANHO_COLUNA; coluna++){
            matrix[linha][coluna] = retorno[k];
            k++;
        }
    }
}

int main(){
    int player, outro;
    int verifyWin = 0;
    int flag;
    //MATRIZ DE JOGO REAL OFICIALZAÇO
     char matrix [TAMANHO_LINHA][TAMANHO_COLUNA]  = {{'-','-','-','-','-','-','-','-','-'},
                                                    {'-','-','o','-','o','-','o','-','o'},
                                                    {'-','o','-','o','-','o','-','o','-'},
                                                    {'-','-','o','-','o','-','o','-','o'},
                                                    {'-','-','-','-','-','-','-','-','-'},
                                                    {'-','-','-','-','-','-','-','-','-'},
                                                    {'-','x','-','x','-','x','-','x','-'},							    
                                                    {'-','-','x','-','x','-','x','-','x'},
                                                    {'-','x','-','x','-','x','-','x','-'}};

     /* //MATRIZ DE VITORIA
     char matrix [TAMANHO_LINHA][TAMANHO_COLUNA]  = {{'-','-','-','-','-','-','-','-','-'},
                                                        {'-','-','-','-','-','-','-','-','-'},
                                                        {'-','-','-','-','-','-','-','-','-'},
                                                        {'-','-','-','-','-','-','-','-','-'},
                                                        {'-','-','-','-','-','-','-','-','-'},
                                                        {'-','-','-','-','o','-','-','-','-'},
                                                        {'-','x','-','x','-','x','-','x','-'},							    
                                                        {'-','-','x','-','x','-','x','-','x'},
                                                        {'-','x','-','x','-','x','-','x','-'}};*/
    //MATRIZ DE TESTE
  /*   char matrix [TAMANHO_LINHA][TAMANHO_COLUNA]  = {{'-','-','-','-','-','-','-','-','-'},
                                                    {'-','-','-','-','-','-','-','-','-'},
                                                    {'-','-','-','x','-','o','-','x','-'},
                                                    {'-','o','-','-','-','-','-','o','-'},
                                                    {'-','-','o','-','-','-','-','-','-'},
                                                    {'-','-','-','-','-','-','-','-','-'},
                                                    {'-','x','-','-','x','-','-','x','-'},							    
                                                    {'-','-','x','x','x','-','x','-','x'},
                                                    {'-','x','-','O','O','x','-','x','-'}};*/
            
    player = solicita_player();

    int file = open(DEVICE_NAME, O_RDWR);
    char stringRetorno[BUF_LEN], matrixString[BUF_LEN];
    
    if(file>0){
        montaIoctl(matrix, matrixString);
        ioctl(file,  IOCTL_SET_MSG, matrixString);
    }
    else{
        escreve_tabuleiro(matrix);
    }

    if(player == 1) outro = 2;
    else if(player == 2) outro = 1;
    
    while(!verifyWin){     

        verifyWin = verifica_vitoria(matrix);

        int change = 0;
        while (file < 0) {
            if(!change){
                printf("\n-------------------Turno do jogador %d---------------------", outro);  
                printf("\nAguarde o jogador %d terminar sua jogada\n", outro);
                change = 1;
            }
            file = open(DEVICE_NAME, O_RDWR);
        }

        int aux = 0;
        while(file > 0){
            if(aux == 0){
                ioctl(file, IOCTL_GET_MSG, stringRetorno);
                le_tabuleiro(matrix, stringRetorno);
                escreve_tabuleiro(matrix);
                verifyWin = verifica_vitoria(matrix);
                aux++;          
             }           
            if(verifyWin) break;
            printf("\n-------------------Turno do jogador %d---------------------\n", player);  
            flag = solicita_jogada(matrix, player);
            if(flag == 1)  break;
            
        } 
        //Escreve a mensagem  
        montaIoctl(matrix, matrixString);
        ioctl(file,  IOCTL_SET_MSG, matrixString);
        //Lê a mensagem
        ioctl(file, IOCTL_GET_MSG, stringRetorno);
        le_tabuleiro(matrix, stringRetorno);
        //Atualiza para o player
        escreve_tabuleiro(matrix);


        verifyWin = verifica_vitoria(matrix);
        close(file);
        file = -1;
        sleep(1);

        if(verifyWin != 0) break;
    }
    verifyWin = verifica_vitoria(matrix);
    if(verifyWin == player) printf("\n****************JOGADOR %d VENCEU***************\n", player);
    if(verifyWin == outro)  printf("\n****************JOGADOR %d VENCEU***************\n", outro);
    return 0;
    /* 
     while(1){
         switch(player){         
             case 1:  
                 ioctl(file, IOCTL_GET_MSG, stringRetorno);
                leTabuleiro(matrix, stringRetorno, player);            
                escreve_tabuleiro(matrix);
                printf("-------------------Turno do jogador 1---------------------\n");
                flag = solicita_jogada(matrix, player);
                if(flag == 1 ){
                    printf("Entrei 1");
                    player = 1;
                    //system("clear");
                }
                else if(flag == 2){
                    printf("Entrei 2");
                    player = 2;
                   // system("clear");
                }
                fflush(stdin);
                break;
            case 2:             
                escreve_tabuleiro(matrix);
                printf("-------------------Turno do jogador 2---------------------\n");
                flag = solicita_jogada(matrix, player) ;
                if(flag == 1 ){
                    printf("Entrei 3");
                    player = 2;
                    //system("clear");
 
                }
                else if(flag == 2){
                    printf("Entrei 4");
                    player = 1;
                    //system("clear");
                }
                fflush(stdin);
                break;
        }
    }*/
}