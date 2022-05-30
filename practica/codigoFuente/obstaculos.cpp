#include<bits/stdc++.h>
using namespace std;

int lastx, lasty;
/*programa de tablero de ajedrez n * n con n piezas con m obstaculos*/
void movePiece(int, int [][3], int, int *, int &, vector<pair<int, int>> &);
void searchLastSquare(int, vector<pair<int, int>> &);
void pieceNextSquare(int, int [][3], int, vector<pair<int, int>> &);
int validatePosition(int, int [][3], int, int *, vector<pair<int, int>> &);
void printSolution(int [][3], int, int *, vector<pair<int, int>> &);
void positionInitial(int, int [][3], int, vector<pair<int, int>> &);

int main(){
    int i, j, count=0;
    /*lectura de las casillas inhabilitadas*/
    int inhab; cin>>inhab;
    vector<pair<int, int>> cInhab(inhab);
    for(i=0; i<inhab; i++)
        cin>>cInhab[i].first>>cInhab[i].second;
    /*lectura del tablero-inicializacion*/
    int n; cin>>n;
    int pieceMat[n][3], typePiece[n];
    for(i=0; i<3; i++)
        for(j=0; j<n; j++){
            if(i == 0) pieceMat[j][i] = j+1;
            else pieceMat[j][i] = -1;//inicializacion, para que en la funcion movePiece le den una posicion inicial que este dentro del tablero
        }
    /*lectura de las piezas*/
    for(i=0; i<n; i++)
        cin>>typePiece[i];
    
    searchLastSquare(n, cInhab);
    movePiece(1, pieceMat, n, typePiece, count, cInhab);

    return 0;
}

void movePiece(int piece, int pieceMat[][3], int n, int *typePiece, int &count, vector<pair<int, int>> &cInhab){ 
    if(pieceMat[piece-1][1]!=-1)//si aun no se le ha asignado un lugar dentro del tablero
        pieceNextSquare(piece, pieceMat, n, cInhab);//mueve las piezas a la siguiente casilla
    else
        positionInitial(piece, pieceMat, n, cInhab);//lo pongo en una posicion inicial

    int val = validatePosition(piece, pieceMat, n, typePiece, cInhab); //validara que no me maten y que no mate
    //cout<<"reina : "<<piece<<" el valor de val = "<<val<<endl;
    if(val == 1 || val==-2){//eres correcto
        if(piece < n && val == 1){//debes mover al resto
            movePiece(piece+1, pieceMat, n, typePiece, count, cInhab);
            return;
        }
        if(piece==n){//si la ultima pieza es correcta, se imprime el resultado 
            count++; 
            printSolution(pieceMat, n, typePiece, cInhab);
        }
        if(piece == 1 && val ==-2){//sentencia que acaba con el programa, esto pasara cuando la primera pieza llegue a la ultima casilla del tablero
            if(count==0)
                cout<<"ATENCION: No encontramos solucion en ese orden\n";
            else
                cout<<"Encontramos "<<count<<" solucion(es)\n";
            return;
        }
        (val == 1)?(val=0):(val=-1);//para seguir buscando soluciones
    }
    if(val == 0 || val == -3 ){//seguir moviendo la pieza por el tablero, porque eres incorrecto ya sea por 
        movePiece(piece, pieceMat, n, typePiece, count, cInhab);
        return;
    }
    if(val == -1){//mover la pieza anterior, esto ocurre cuando la pieza actual ha llegado a la ultima casilla
        movePiece(piece-1, pieceMat, n, typePiece, count, cInhab);
        return;
    }
}
bool onlyRook(int* typePiece, int n){
    for(int i=0; i<n; i++)
        if(typePiece[i]!=2)
            return false;
    return true;
}
void searchLastSquare(int n, vector<pair<int, int>> &cInhab){
    int i, j, k, count;
    for(i=0; i<n; i++)//columnas - x
        for(j=0; j<n; j++){//filas - y
            count=0;
            for(k=0; k<cInhab.size(); k++){
                if(cInhab[k].first == i && cInhab[k].second == j)
                    count++;
            }
            if(count==0){
                lastx=i; 
                lasty=j;
            }
        }
}
void pieceNextSquare(int piece, int pieceMat[][3], int n, vector<pair<int, int>> &cInhab){
    int x = pieceMat[piece-1][1], y = pieceMat[piece-1][2];

    if(x == lastx && y == lasty){
        //lo ponemos enseguida de su anterior cuando ya se encuentre en la casilla (n-1, n-1)
        int xPrev = pieceMat[piece-2][1], yPrev = pieceMat[piece-2][2];
        if( yPrev == n-1 )
            pieceMat[piece-1][2]=0;
        else
            pieceMat[piece-1][2]=yPrev+1;
        if( pieceMat[piece-1][2]==0 )
            pieceMat[piece-1][1]=xPrev+1;
        else
            pieceMat[piece-1][1]=xPrev;
    }else{
        //lo ponemos en la siguiente casilla
        if( y == n-1 )
            pieceMat[piece-1][2]=0;
        else
            pieceMat[piece-1][2]+=1; 
        if( pieceMat[piece-1][2] == 0 )
            pieceMat[piece-1][1]+=1;
    }
}
int validatePosition(int piece, int pieceMat[][3], int n, int *typePiece, vector<pair<int, int>> &cInhab){
    /* pared */
    int x = pieceMat[piece-1][1], y = pieceMat[piece-1][2];
    //cout<<"REINA : "<<piece<<" esta en "<<x<<" "<<y<<endl;
    if(x<lastx || y<lasty || (x==lastx && y<lasty) || (x<lastx && y==lasty)){
        //cout<<"Estoy antes de la ultima casilla y vere si estoy en una pared\n";
        for(int i=0; i<cInhab.size(); i++){
            //cout<<"pared en "<<cInhab[i].first<<" "<<cInhab[i].second<<endl;
            if(pieceMat[piece-1][1]==cInhab[i].first && pieceMat[piece-1][2]==cInhab[i].second){
                //cout<<"Estoy en una pared\n";
                return -3;
            }
        }
    }
    //cout<<"Reina: "<<piece<<" No estoy en una pared\n";
    int lim = 0, xaux, yaux;
    //movimientos del caballo, hacia adelante "hx-hy" & hacia atras "hxx-hyy"
    int hx[] = {1, 2, 2, 1}, hy[] = {-2, -1, 1, 2}, hxx[] = {-1, -2, -2, -1}, hyy[] = {-2, -1, 1, 2};
    //movimientos del rey, hacia adelante "rx-ry" & hacia atras "rxx-ryy"
    int rx[] = {0, 1, 1, 1, 0}, ry[] = {-1, -1, 0, 1, 1}, rxx[] = {0, -1, -1, -1, 0}, ryy[] = {-1, -1, 0, 1, 1};
    
    if(x == lastx && y == lasty)// estas en la ultima casilla del tablero
        lim = 1;
    for(int i=0; i<piece-1; i++){
        int xv = pieceMat[i][1], yv = pieceMat[i][2];
        //checar si alguna pieza no me mata
        switch(typePiece[i]){
            case 1:{//Reina
                // derecha
                int xaux = xv; // se incrementa
                int yaux = yv; // se queda fija 
                while(xaux < n){
                    int f=0;
                    xaux++;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo a la derecha para la reina: "<<i<<" en "<<xaux<<" "<<yaux<<endl;
                        break;
                    }
                    if(x == xaux && y == yaux){
                        //cout<<"me matan con direccion a la derecha, la reina: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0; 
                    }
                    
                }
                // abajo
                xaux = xv; // se queda fija 
                yaux = yv; // se incrementa
                while(yaux < n){
                    int f=0;
                    yaux++;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo abajo para la reina: "<<i<<" en "<<xaux<<" "<<yaux<<endl;
                        break;
                    }
                    if(y == yaux && x == xaux){
                        //cout<<"me matan con direccion hacia abajo la reina: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0; 
                    }
                }
                //diagonal abajo
                xaux = xv; 
                yaux = yv;
                while( xaux < n && yaux < n ){
                    int f=0;
                    xaux++; yaux++;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo en mi diagonal abajo, estoy en "<<xv<<" "<<yv<<endl;
                        break;
                    }
                    if(xaux == x && yaux == y){
                        //cout<<"me matan en diagonal abajo, la reina: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0; 
                    } 
                }
                //diagonal arriba 
                xaux = xv; 
                yaux = yv;
                while( xaux < n && yaux >= 0 ){  
                    xaux++; yaux--;
                    int f=0;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo en mi diagonal arriba, estoy en "<<xv<<" "<<yv<<endl;
                        break;
                    }
                    if( xaux == x && yaux == y ){
                        //cout<<"me matan en diagonal arriba la reina: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0; 
                    }
                }
                break;
            }case 2:{//Torre
                xaux = xv; // se aumenta para ver en horizontal 
                yaux = yv; // permanece
                while(xaux < n){
                    int f=0;
                    xaux++;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo a la derecha para la torre: "<<i<<" en "<<xaux<<" "<<yaux<<endl;
                        break;
                    }
                    if(x == xaux && y == yaux){
                        //cout<<"me matan con direccion a la derecha, la torre: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0; 
                    }
                }   
                xaux = xv; // se queda fija 
                yaux = yv; // se incrementa
                while(yaux < n){
                    int f=0;
                    yaux++;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo abajo para la torre: "<<i<<" en "<<xaux<<" "<<yaux<<endl;
                        break;
                    }
                    if(y == yaux && x == xaux){
                        //cout<<"me matan con direccion hacia abajo la torre: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0; 
                    }
                } 
                break;
            }case 3:{//Alfil
                xaux = xv; yaux = yv;
                while( xaux < n && yaux < n ){
                    xaux++; yaux++;
                    int f=0;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo en mi diagonal arriba, estoy en "<<xv<<" "<<yv<<endl;
                        break;
                    } 
                    if(xaux == x && yaux == y)
                        return (lim==1)?-1:0;  
                }
                xaux = xv; yaux = yv;
                while( xaux < n && yaux >= 0 ){    
                    xaux++; yaux--;
                    int f=0;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo en mi diagonal arriba, estoy en "<<xv<<" "<<yv<<endl;
                        break;
                    }
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
            case 1:{//reina
                //izquierda
                xaux = x; //se disminuye x
                yaux = y; //se queda fijo
                while(xaux >= 0){
                    xaux--;
                    int f=0;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo a la izquierda que me impide matar\n";
                        break;
                    }
                    if(xv == xaux && yv == yaux){
                        //cout<<"a la izquierda mato a la reina: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0;
                    } 
                }   
                xaux = x; //queda fija 
                yaux = y; //se decrementa
                while(yaux >= 0 ){
                    int f=0;
                    yaux--;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo arriba, lo que me impide matar\n";
                        break;
                    }
                    if(xv == xaux && yv == yaux ){
                        //cout<<"hacia arriba mato a la reina: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0;
                    } 
                } 
                //izquierda arriba
                xaux = xv; 
                yaux = yv;
                while( xaux >= 0 && yaux >= 0 ){
                    int f=0;
                    xaux--; yaux--;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo en mi diagonal izquierda arriba que me impide matar\n";
                        break;
                    }
                    if(xv == xaux && yv == yaux){
                        //cout<<"en diagonal izquierda arriba mato a la reina: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0;  
                    }
                }
                //izquierda abajo
                xaux = xv; 
                yaux = yv;
                while( xaux >= 0 && yaux < n ){           
                    int f=0;
                    xaux--; yaux++;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo en mi diagonal izquierda abajo que me impide matar\n";
                        break;
                    }
                    if( xv == xaux && yv == yaux ){
                        //cout<<"en diagonal izquierda abajo mato a la reina: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0;  
                    }
                }
                
                break;
            }case 2:{//torre
                xaux = x; //se disminuye x
                yaux = y; //se queda fijo
                while(xaux >= 0){
                    int f=0;
                    xaux--;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo a la izquierda que me impide matar\n";
                        break;
                    }
                    if(xv == xaux && yv == yaux){
                        //cout<<"a la izquierda mato a la pieza: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0;
                    } 
                }   
                xaux = x; //queda fija 
                yaux = y; //se decrementa
                while(yaux >= 0 ){
                    int f=0;
                    yaux--;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo arriba, lo que me impide matar\n";
                        break;
                    }
                    if(xv == xaux && yv == yaux ){
                        //cout<<"hacia arriba mato a la pieza: "<<i<<" que esta en "<<xv<<" "<<yv<<endl;
                        return (lim==1)?-1:0;
                    } 
                } 
                break;
            }case 3:{//alfil
                xaux = x; yaux = y;
                while( xaux >= 0 && yaux >= 0 ){
                    xaux--; yaux--;
                    int f=0;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo en mi diagonal abajo, estoy en "<<xv<<" "<<yv<<endl;
                        break;
                    }
                    if( xv == xaux && yv == yaux )
                        return (lim==1)?-1:0;  
                }
                xaux = x; yaux = y;
                while( xaux >= 0 && yaux < n ){   
                    xaux--; yaux++;
                    int f=0;
                    for(int j=0; j<cInhab.size(); j++)
                        if(cInhab[j].first == xaux && cInhab[j].second == yaux){
                            f=1;
                            break;
                        }
                    if(f){
                        //cout<<"Dejar de buscar - hay un obstaculo en mi diagonal abajo, estoy en "<<xv<<" "<<yv<<endl;
                        break;
                    }
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
void printSolution(int pieceMat[][3], int n,int* typePiece, vector<pair<int, int>> &cInhab){
    cout<<"Encontre solucion\n";
    int x=0, y=0, k=0; 
    int mat[n][n];
    for( int i=0; i<n; i++ ){
        for( int j=0; j<n; j++ ){
            x = pieceMat[k][1]; y = pieceMat[k][2];
            if(x==i && y ==j){
                mat[i][j]=typePiece[k];
                (k==n-1)?k=0:k++;
            }else
                mat[i][j]=0;
        }
    }
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            for(int k=0; k<cInhab.size(); k++)
                if(cInhab[k].first==i && cInhab[k].second==j)
                    mat[i][j]=8;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            cout<<mat[j][i]<<" ";
        cout<<endl;
    }
    return;
}
void positionInitial(int piece, int pieceMat[][3], int n, vector<pair<int, int>> &cInhab){
    if(piece == 1){//si la pieza 1 no ha sido acomodada la pongo en la primera casilla del tablero
        pieceMat[piece-1][1]=0;
        pieceMat[piece-1][2]=0;
    }else{//si es cualquier otra pieza, la pongo enseguida del anterior
        int xPrev = pieceMat[piece-2][1];
        int yPrev = pieceMat[piece-2][2];
        if(yPrev==n-1)
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
