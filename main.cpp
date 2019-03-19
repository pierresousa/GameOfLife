/*
Titulo: Game Of Life
Autor: Pierre Victor da Silva Sousa
Curso: Engenharia de Sistemas
Materia: Progração e Desenvolvimento de Software II
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <dirent.h>

#define Define_CurrentDir _getcwd

using namespace std;

//Faz listagem do arquivo
string requisitaArquivo();

//Converter string em bool
bool stringToBool(char serVivo);

//repassa a matriz verificar para a matriz impressao
void repassarCelulas(bool mapaVerificar[80][22], bool mapaImpressao[80][22], int tamanhoX, int tamanhoY);

//Seleciona cada celula
void selecionaCelula(bool mapaVerificar[80][22], bool mapaImpressao[80][22], int tamanhoX, int tamanhoY);

//Verifica a vizinhança da celula
int verificaVizinho(bool mapaVerificar[80][22], int x, int y, int tamanhoX, int tamanhoY);

//Aplica a logica na celula
void logicaCelulaViva(bool mapaVerificar[80][22], int x, int y, int vizinhos);
void logicaCelulaMorta(bool mapaVerificar[80][22], int x, int y, int vizinhos);

//Imprime celulas no terminal
void imprimeCelulas(bool mapaVerificar[80][22], int tamanhoX, int TamanhoY, int modo, int velocidade, char &c); 

int main () {
    bool mapaVerificar[80][22], mapaImpressao[80][22], serVivo;
    int x=0, y=0, tamanhoX, tamanhoY=0, vizinhos=0, modo, velocidade=0;
    string line, arquivo;
    char c;

    cout << "----------------GAME OF LIFE----------------" << endl;

    arquivo = requisitaArquivo();
        
    ifstream myfile (arquivo);
    do{
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {   
                tamanhoX = line.size();

                while(x<line.size()){
                    mapaVerificar[x][y] = stringToBool(line[x]);
                    x++;
                }
                
                y++;
                x=0;
                tamanhoY++;
            }
        } else {
            Sleep(2000);
            system("cls");
            arquivo = requisitaArquivo();
            ifstream myfile (arquivo);
        }
    }while(!myfile.is_open());
    myfile.close();
    for(y=0; y<tamanhoY;y++){
        for(x=0; x<tamanhoX;x++){
            mapaImpressao[x][y] = mapaVerificar[x][y];
        }
    }

    do{
        cout << "Escolha um modo:" << endl << "1 - Automatico" << endl << "2 - Manual" << endl << "-";
        cin >> modo;
        if(modo!=1 && modo!=2){
            cout << "Entrada invalida!" << endl << endl;
        }
    }while(modo!=1 && modo!=2);

    if(modo==1){
        do{
            cout << "Escolha velocidade (ms):" << endl << "-";
            cin >> velocidade;
            if(velocidade<0){
                cout << "Entrada invalida!" << endl << endl;
            }
        }while(velocidade<1);
    }

    system("cls");
    for(y=0; y<tamanhoY;y++){
        for(x=0; x<tamanhoX;x++){
            if(mapaImpressao[x][y]){
                cout << "+";
            } else cout << " ";
        }   
        cout << endl;
    }
    Sleep(2000);

    while(!(c=='')){
        cout << "Pressione esc para sair" << endl;
        imprimeCelulas(mapaImpressao,tamanhoX, tamanhoY, modo, velocidade, c);     
        selecionaCelula(mapaVerificar, mapaImpressao, tamanhoX, tamanhoY);
        repassarCelulas(mapaVerificar, mapaImpressao, tamanhoX, tamanhoY);
        if(kbhit()){
            c = getch();
        }
    }
    
    system("cls");
    system("PAUSE");
    return 0;
}

string requisitaArquivo(){

    DIR *dir;
    struct dirent *lsdir;
    char LOCAL_DIR[FILENAME_MAX];
    string arquivo, arquivoSelecionado, line;
    int tamanho, contador=1, menu, i=0, x, y;

    if (!Define_CurrentDir(LOCAL_DIR, sizeof(LOCAL_DIR)))
    {
        cout << errno;
    }
    dir = opendir(LOCAL_DIR);

    while ( ( lsdir = readdir(dir) ) != NULL )
    {
        arquivo = lsdir->d_name;
        tamanho = arquivo.length();
        if( arquivo[tamanho-3]=='t' && arquivo[tamanho-2]=='x' && arquivo[tamanho-1]=='t'){
            cout << contador << " - " << lsdir->d_name <<endl;
            contador++;
        }
        
    }
    
    do
    {
        cout << "Sua escolha(digite apenas numeros): ";
        cin >> menu;
        if(menu<1 || menu >contador){
            cout << "Opcao invalida!" << endl;
        }
    } while (menu<1 || menu >contador);
    
    contador = 1;
    
    dir = opendir(LOCAL_DIR);
    while ( ( lsdir = readdir(dir) ) != NULL )
    {
        arquivo = lsdir->d_name;
        tamanho = arquivo.length();
        if( arquivo[tamanho-3]=='t' && arquivo[tamanho-2]=='x' && arquivo[tamanho-1]=='t'){
            if(contador==menu){
                arquivoSelecionado = lsdir->d_name;
            }
            contador++;
        }
        
    }
    closedir(dir);

    return arquivoSelecionado;
}

bool stringToBool(char serVivo){
    if(serVivo=='1'){
        return true;
    } 
    else{
        return false;
    }
}

void repassarCelulas(bool mapaVerificar[80][22], bool mapaImpressao[80][22], int tamanhoX, int tamanhoY){
    int x, y;

    for(y=0; y<tamanhoY;y++){
        for(x=0; x<tamanhoX;x++){
            mapaVerificar[x][y] = mapaImpressao[x][y];
        }
    }
}

void selecionaCelula(bool mapaVerificar[80][22], bool mapaImpressao[80][22], int tamanhoX, int tamanhoY){
    int vizinhos = 0, x = 0, y = 0, celulas=0;

    for(y=0; y<tamanhoY;y++){
        for(x=0; x<tamanhoX;x++){
            if (mapaVerificar[x][y]){
                vizinhos = verificaVizinho(mapaVerificar,x, y, tamanhoX, tamanhoY);
                logicaCelulaViva(mapaImpressao, x, y, vizinhos);

            } else{
                vizinhos = verificaVizinho(mapaVerificar,x, y, tamanhoX, tamanhoY);
                logicaCelulaMorta(mapaImpressao, x, y, vizinhos);
                
            }
            
            
        }
    }
}

int verificaVizinho(bool mapaVerificar[80][22], int x, int y, int tamanhoX, int tamanhoY){
    int vizinhos=0;
    
    if( !((y-1)<0) ){
        if(mapaVerificar[x][y-1]){
            vizinhos++;    
        }        
    }
    if( !((y-1)<0) && ((x+1)<tamanhoX) ){
        if(mapaVerificar[x+1][y-1]){
            vizinhos++;      
        }        
    }
    if( ((x+1)<tamanhoX) ){
        if(mapaVerificar[x+1][y]){
            vizinhos++;       
        }        
    }
    if( ((y+1)<tamanhoY) && ((x+1)<tamanhoX) ){
        if(mapaVerificar[x+1][y+1]){
            vizinhos++;  
        }        
    }
    if( ((y+1)<tamanhoY) ){
        if(mapaVerificar[x][y+1]){
            vizinhos++;     
        }        
    }
    if( ((y+1)<tamanhoY) && !((x-1)<0) ){
        if(mapaVerificar[x-1][y+1]){
            vizinhos++;     
        }        
    }
    if( !((x-1)<0) ){
        if(mapaVerificar[x-1][y]){
            vizinhos++;      
        }        
    }
    if( !((y-1)<0) && !((x-1)<0) ){
        if(mapaVerificar[x-1][y-1]){
            vizinhos++;       
        }        
    }

    return vizinhos;
}

void logicaCelulaViva(bool mapaImpressao[80][22], int x, int y, int vizinhos){
    if(vizinhos<=1 || vizinhos>3){
        mapaImpressao[x][y] = 0;
    }
}

void logicaCelulaMorta(bool mapaImpressao[80][22], int x, int y, int vizinhos){
    if (vizinhos==3){
        mapaImpressao[x][y] = 1;
    }    
}

void imprimeCelulas(bool mapaImpressao[80][22], int tamanhoX, int tamanhoY, int modo, int velocidade, char &c){
    int x, y;

    for(y=0; y<tamanhoY;y++){
        for(x=0; x<tamanhoX;x++){
            if(mapaImpressao[x][y]){
                cout << "+";
            } else cout << " ";
        }   
        cout << endl;
    }

    if(modo==1){
        Sleep(velocidade);
        system("cls");
    } else{
        c = getch(); 
        system("cls");
    }
}
