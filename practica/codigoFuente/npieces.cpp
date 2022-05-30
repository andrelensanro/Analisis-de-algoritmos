#include<stdlib.h>
#include<stdio.h>
#include<algorithm>

using namespace std;
/* para N piezas, las cuales pueden ser distintas o iguales 
programacion con el metodo divide y venceras, backtracking y heuristicas voraces y programacion dinamica-torres*/
void movePiece(int piece, int pieceMat[][3], int pieceNum, int *typePiece, int *count);
void pieceNextSquare(int piece, int pieceMat[][3], int pieceNum);
int validatePosition(int piece, int pieceMat[][3], int pieceNum, int *typePiece);
void printSolution(int pieceMat[][3], int pieceNum, int *typePiece);
void positionInitial(int piece, int pieceMat[][3], int pieceNum);
bool onlyRook(int* typePiece, int pieceNum);

int main(){
    int pieceNum, i=0, j=0, count=0;
    printf("Tablero: ");
    scanf("%d", &pieceNum);
    /*Arreglos de informacion*/
    int pieceMat[pieceNum][3], typePiece[pieceNum];
    printf("1.Reina 2.Torre 3.Alfil 4.Peon 5.Caballo 6.Rey\nTipo de piezas: ");
    
    for(i=0; i<pieceNum; i++)
        scanf("%d", &typePiece[i]);
    
    for(i=0; i<3; i++)
        for(j=0; j<pieceNum; j++){
            if(i == 0) pieceMat[j][i] = j+1;
            else pieceMat[j][i] = -1;//inicializacion, para que en la funcion movePiece le den una posicion inicial que este dentro del tablero
        }

    movePiece(1, pieceMat, pieceNum, typePiece, &count);//se comienza con la pieza #1
    return 0;
}
void movePiece(int piece, int pieceMat[][3], int pieceNum, int *typePiece, int *count){ 
    if(pieceMat[piece-1][1]!=-1)//si aun no se le ha asignado un lugar dentro del tablero
        pieceNextSquare(piece, pieceMat, pieceNum);//mueve las piezas a la siguiente casilla
    else
        positionInitial(piece, pieceMat, pieceNum);//lo pongo en una posicion inicial

    int val = validatePosition(piece, pieceMat, pieceNum, typePiece); //validara que no me maten y que no mate
    
    if(val == 1 || val==-2){//eres correcto
        if(piece < pieceNum && val == 1){//debes mover al resto
            movePiece(piece+1, pieceMat, pieceNum, typePiece, count);
            return;
        }
        if(piece==pieceNum){//si la ultima pieza es correcta, se imprime el resultado 
            (*count)++; 
            int permut[pieceNum];
            if(*count == 1 && onlyRook(typePiece, pieceNum)){
                for(int i=0; i<pieceNum; i++)
                    permut[i] = pieceMat[i][1];
                do{
                    for(int i=0; i<pieceNum; i++)
                        pieceMat[i][2]=permut[i];
                    printSolution(pieceMat, pieceNum, typePiece);
                    (*count)++;
                }while(next_permutation(permut, permut+pieceNum));
                if(*count==0)
                    printf("ATENCION: No encontramos solucion en ese orden\n");
                else
                    printf("Encontramos %d solucion(es)\n", (*count)-=1);
                return;
            }
            printSolution(pieceMat, pieceNum, typePiece);
        }
        if(piece == 1 && val ==-2){//sentencia que acaba con el programa, esto pasara cuando la primera pieza llegue a la ultima casilla del tablero
            if(*count==0)
                printf("ATENCION: No encontramos solucion en ese orden\n");
            else
                printf("Encontramos %d solucion(es)\n", *count);
            return;
        }
        (val == 1)?(val=0):(val=-1);//para seguir buscando soluciones
    }
    if(val == 0){//seguir moviendo la pieza por el tablero
        movePiece(piece, pieceMat, pieceNum, typePiece, count);
        return;
    }
    if(val == -1){//mover la pieza anterior, esto ocurre cuando la pieza actual ha llegado a la ultima casilla
        movePiece(piece-1, pieceMat, pieceNum, typePiece, count);
        return;
    }
}
bool onlyRook(int* typePiece, int pieceNum){
    for(int i=0; i<pieceNum; i++)
        if(typePiece[i]!=2)
            return false;
    return true;
}

void pieceNextSquare(int piece, int pieceMat[][3], int pieceNum){
    int x = pieceMat[piece-1][1], y = pieceMat[piece-1][2];

    if(x == pieceNum-1 && y == pieceNum-1){
        //lo ponemos enseguida de su anterior cuando ya se encuentre en la casilla (pieceNum-1, pieceNum-1)
        int xPrev = pieceMat[piece-2][1], yPrev = pieceMat[piece-2][2];
        if( yPrev == pieceNum-1 )
            pieceMat[piece-1][2]=0;
        else
            pieceMat[piece-1][2]=yPrev+1;
        if( pieceMat[piece-1][2]==0 )
            pieceMat[piece-1][1]=xPrev+1;
        else
            pieceMat[piece-1][1]=xPrev;
    }else{
        //lo ponemos en la siguiente casilla
        if( y == pieceNum-1 )
            pieceMat[piece-1][2]=0;
        else
            pieceMat[piece-1][2]+=1; 
        if( pieceMat[piece-1][2] == 0 )
            pieceMat[piece-1][1]+=1;
    }
}
int validatePosition(int piece, int pieceMat[][3], int pieceNum, int *typePiece){
    int x = pieceMat[piece-1][1], y = pieceMat[piece-1][2], lim = 0, xaux, yaux;
    //movimientos del caballo, hacia adelante "hx-hy" & hacia atras "hxx-hyy"
    int hx[] = {1, 2, 2, 1}, hy[] = {-2, -1, 1, 2}, hxx[] = {-1, -2, -2, -1}, hyy[] = {-2, -1, 1, 2};
    //movimientos del rey, hacia adelante "rx-ry" & hacia atras "rxx-ryy"
    int rx[] = {0, 1, 1, 1, 0}, ry[] = {-1, -1, 0, 1, 1}, rxx[] = {0, -1, -1, -1, 0}, ryy[] = {-1, -1, 0, 1, 1};
    
    if(x == pieceNum-1 && y == pieceNum-1)// estas en la ultima casilla del tablero
        lim = 1;
    for(int i=0; i<piece-1; i++){

        int xv = pieceMat[i][1], yv = pieceMat[i][2];
        //checar si alguna pieza no me mata
        switch(typePiece[i]){
            case 1:{//Reina
                if(x == xv || y == yv)
                    return (lim==1)?-1:0; 
                xaux = xv; yaux = yv;
                while( xaux < pieceNum && yaux < pieceNum ){
                    xaux++; yaux++;
                    if(xaux == x && yaux == y)
                        return (lim==1)?-1:0;  
                }
                xaux = xv; yaux = yv;
                while( xaux < pieceNum && yaux >= 0 ){    
                    xaux++; yaux--;
                    if( xaux == x && yaux == y )
                        return (lim==1)?-1:0; 
                }
                break;
            }case 2:{//Torre
                if(x == xv || y == yv)
                    return (lim==1)?-1:0; 
                break;
            }case 3:{//Alfil
                xaux = xv; yaux = yv;
                while( xaux < pieceNum && yaux < pieceNum ){
                    xaux++; yaux++;
                    if(xaux == x && yaux == y)
                        return (lim==1)?-1:0;  
                }
                xaux = xv; yaux = yv;
                while( xaux < pieceNum && yaux >= 0 ){    
                    xaux++; yaux--;
                    if( xaux == x && yaux == y )
                        return (lim==1)?-1:0; 
                }
                break;
            }case 4:{//Peon
                if(x == xv+1 && y == yv-1)
                    return (lim==1)?-1:0; 
                if(x == xv+1 && y == yv+1)
                    return (lim==1)?-1:0; 
                break;
            }case 5:{//Caballo
                for(int j=0; j<4; j++)
                    if(x == xv+hx[j] && y == yv+hy[j])
                        return (lim==1)?-1:0; 
                break;
            }case 6://Rey
                for(int j=0; j<5; j++)
                    if(x == xv+rx[j] && y == yv+ry[j])
                        return (lim==1)?-1:0; 
                break;
        }
        //checar que no mate a las piezas ya puestas
        switch(typePiece[piece-1]){
            case 1:{
                xaux = x; yaux = y; 
                if(x == xv || y == yv)
                    return (lim==1)?-1:0; 
                while( xaux >= 0 && yaux >= 0 ){
                    xaux--; yaux--;
                    if(xv == xaux && yv == yaux)
                        return (lim==1)?-1:0;   
                }
                xaux = x; yaux = y;
                while( xaux >= 0 && yaux < pieceNum ){    
                    xaux--; yaux++;
                    if( xv == xaux && yv == yaux )
                        return (lim==1)?-1:0; 
                }
                break;
            }case 2:{
                if(x == xv || y == yv)
                    return (lim==1)?-1:0; 
                break;
            }case 3:{
                xaux = x; yaux = y;
                while( xaux >= 0 && yaux >= 0 ){
                    xaux--; yaux--;
                    if( xv == xaux && yv == yaux )
                        return (lim==1)?-1:0;  
                }
                xaux = x; yaux = y;
                while( xaux >= 0 && yaux < pieceNum ){   
                    xaux--; yaux++;
                    if( xv == xaux && yv == yaux )
                        return (lim==1)?-1:0;
                }
                break;
            }case 4:{
                if(xv == x-1 && yv == y-1)
                    return (lim==1)?-1:0;  
                if(xv == x-1 && yv == y+1)
                    return (lim==1)?-1:0;  
                break;
            }case 5:{
                for(int j=0; j<4; j++)
                    if(xv == x+hxx[j] && yv == y+hyy[j])
                        return (lim==1)?-1:0; 
                break;
            }case 6:
                for(int j=0; j<5; j++)
                    if(xv == x+rxx[j]&& yv == y+ryy[j])
                        return (lim==1)?-1:0; 
                break;
        }

    }
    if(lim == 1){//estoy en la ultima casilla y soy correcto
        return -2;
    }
    //soy correcto y estoy en cualquier otro lugar del tablero
    return 1;
}
void printSolution(int pieceMat[][3], int pieceNum,int* typePiece){
    printf("Encontre solucion\n");
    int x=0, y=0, k=0; 
    int mat[pieceNum][pieceNum];
    for( int i=0; i<pieceNum; i++ ){
        for( int j=0; j<pieceNum; j++ ){
            x = pieceMat[k][1]; y = pieceMat[k][2];
            if(x==i && y ==j){
                mat[i][j]=typePiece[k];
                (k==pieceNum-1)?k=0:k++;
            }else
                mat[i][j]=0;
        }
    }
    for(int i=0; i<pieceNum; i++){
        for(int j=0; j<pieceNum; j++)
            printf("%d ", mat[j][i]);
        printf("\n");
    }
    return;
}
void positionInitial(int piece, int pieceMat[][3], int pieceNum){
    if(piece == 1){//si la pieza 1 no ha sido acomodada la pongo en la primera casilla del tablero
        pieceMat[piece-1][1]=0;
        pieceMat[piece-1][2]=0;
    }else{//si es cualquier otra pieza, la pongo enseguida del anterior
        int xPrev = pieceMat[piece-2][1];
        int yPrev = pieceMat[piece-2][2];
        if(yPrev==pieceNum-1)
            pieceMat[piece-1][2]=0;
        else
            pieceMat[piece-1][2]=yPrev+1;
        if(pieceMat[piece-1][2]==0)
            pieceMat[piece-1][1]=xPrev+1;
        else
            pieceMat[piece-1][1]=xPrev;
    }
    return;
}
