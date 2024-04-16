#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

struct Inimigo{
    int x, y;
};

/// Função:  Contagem numérica de inimigos e armazenagem de suas respectivas posições.
int quantidadeInimiga(int m[13][31], vector<Inimigo>& inimigos) {
    inimigos.clear(); // Limpeza de vetor para atualização de possições.
    int quantidadeInimiga = 0;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 31; j++) {
            if (m[i][j] == 3) {
                Inimigo inimigo;
                inimigo.x = i;
                inimigo.y = j;
                inimigos.push_back(inimigo);
                quantidadeInimiga++;
            }
        }
    }
    return quantidadeInimiga;
}

/// Função: Movimento aleatório de inimigos.
void movimentoInimigo(int m[13][31], vector<Inimigo>& inimigos, int bx, int by, bool bombaColocada) {
    // Percorre cada inimigo e os move.
    for (size_t i = 0; i < inimigos.size(); i++) {
        // Escolhendo direção aleatória: 0 (acima), 1 (abaixo), 2 (esquerda), 3 (direita)
        int direcao;
        do {
            direcao = rand() % 4;
        } while (direcao == 0 && (inimigos[i].x - 1 == bx && inimigos[i].y == by) ||
                 direcao == 1 && (inimigos[i].x + 1 == bx && inimigos[i].y == by) ||
                 direcao == 2 && (inimigos[i].x == bx && inimigos[i].y - 1 == by) ||
                 direcao == 3 && (inimigos[i].x == bx && inimigos[i].y + 1 == by));

        // Armazenagem da possição original do inimigo.
        int originalX = inimigos[i].x;
        int originalY = inimigos[i].y;

        // Move o inimigo na posição escolhida, com verificação de colisão com bomba
        switch (direcao) {
            case 0: // acima
                if (m[inimigos[i].x - 1][inimigos[i].y] == 0) {
                    inimigos[i].x--;
                }
                break;
            case 1: // abaixo
                if (m[inimigos[i].x + 1][inimigos[i].y] == 0) {
                    inimigos[i].x++;
                }
                break;
            case 2: // esquerda
                if (m[inimigos[i].x][inimigos[i].y - 1] == 0) {
                    inimigos[i].y--;
                }
                break;
            case 3: // direita
                if (m[inimigos[i].x][inimigos[i].y + 1] == 0) {
                    inimigos[i].y++;
                }
                break;
            default:
                break;
        }

        // Atualiza o campo do jogo com a nova posição inimiga.
        m[originalX][originalY] = 0;
        m[inimigos[i].x][inimigos[i].y] = 3;
    }
}


///Função: Processo de explosão da Bomba
void explosaoBomba(int m[13][31], int bx, int by, bool &bombaColocada, bool &movimentoDoPersonagem, clock_t &explosao, clock_t tempoInicialDaBomba) {

    clock_t tempoAtual = clock();
    double duracaoDaBomba = (tempoAtual - tempoInicialDaBomba) / (double)CLOCKS_PER_SEC;

    if ((tempoAtual - tempoInicialDaBomba) / (double)CLOCKS_PER_SEC >= 3.5) {
        // Mecanismo da explosão
        m[bx][by] = 0;

        if (m[bx - 1][by] != 1) {
            m[bx - 1][by] = 0;
        }
        if (m[bx][by - 1] != 1) {
            m[bx][by - 1] = 0;
        }
        if (m[bx + 1][by] != 1) {
            m[bx + 1][by] = 0;
        }
        if (m[bx][by + 1] != 1) {
            m[bx][by + 1] = 0;
        }
        bombaColocada = false;
        movimentoDoPersonagem = false;
        explosao = tempoAtual; // Começa o timer da limpeza da explosao
    }

    if (bombaColocada && (duracaoDaBomba >= 3.0)) {
        m[bx][by] = 5;

        if (m[bx - 1][by] != 1) { //Limite da explosão.
            m[bx - 1][by] = 5;
        }
        if (m[bx][by - 1] != 1) {
            m[bx][by - 1] = 5;
        }
        if (m[bx + 1][by] != 1) {
            m[bx + 1][by] = 5;
        }
        if (m[bx][by + 1] != 1) {
            m[bx][by + 1] = 5;
        }
        explosao = 0; //Reseta o timer da limpeza
    }
}


int main(){

    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.

    int m[13][31]={ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                    1,2,2,0,0,0,0,2,2,0,0,0,0,2,0,2,2,0,2,0,0,0,2,0,2,0,0,0,0,2,1,
                    1,0,1,0,1,2,1,0,1,2,1,0,1,2,1,2,1,2,1,0,1,2,1,2,1,2,1,2,1,0,1,
                    1,0,0,3,0,0,0,2,2,0,0,2,2,0,0,2,0,0,2,0,0,0,2,0,0,0,0,3,2,2,1,
                    1,0,1,0,1,0,1,0,1,2,1,0,1,0,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,
                    1,2,2,2,2,0,0,0,0,2,0,0,0,0,0,2,2,0,0,0,0,0,0,2,0,0,0,0,0,0,1,
                    1,0,1,2,1,2,1,2,1,2,1,2,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,2,1,0,1,
                    1,0,0,2,0,0,2,2,0,0,2,2,2,0,0,0,2,0,0,0,0,0,2,2,2,2,0,0,0,0,1,
                    1,2,1,0,1,0,1,0,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,0,1,2,1,
                    1,2,0,3,0,0,0,2,2,0,0,0,2,0,2,0,0,0,2,2,0,0,0,0,0,0,0,3,0,0,1,
                    1,0,1,0,1,0,1,0,1,0,1,2,1,2,1,0,1,0,1,2,1,0,1,0,1,2,1,0,1,0,1,
                    1,2,0,0,2,2,0,0,0,2,2,2,2,0,0,0,2,0,0,2,2,0,0,0,2,0,0,2,2,0,1,
                    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, };




    int x=6, y=15;//Posicao inicial do personagem no console
    char tecla;//Variavel para tecla precionada
    int bx, by; //Posicao da bomba
    bool bombaColocada = false; // verificador de bomba colocada
    bool movimentoDoPersonagem = false; // verifica se o personagem se moveu da posição da bomba
    clock_t tempoInicialDaBomba; //Inicialização de tempo da bomba
    clock_t explosao;
    clock_t tempoDeMovimentoInimigo = clock(); //Tempo de movimento inimigo*/

    // Definindo vetor que segura inimigos.
    vector<Inimigo> inimigos;

    // Inicializando inimigos
    for (int i = 0; i < 3; ++i) { // Ajusta o número de inimigos como for necessário
        Inimigo inimigo;
        inimigo.x = 6; // Posição inicial 'x' para os inimigos.
        inimigo.y = 6 + i * 2; // Posição inicial 'y' para os inimigos.
        inimigos.push_back(inimigo);
    }


    while(true){
        ///Posiciona a escrita no inicio do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<13;i++){
            for(int j=0;j<31;j++){
                if(i==x && j==y){
                    cout<<char(225); //personagem
                } else {
                    switch (m[i][j]){
                        case 0: cout<<" "; break; //caminho
                        case 1: cout<<char(219); break; //parede externa/interna indestrutivel
                        case 2: cout<<char(177); break; //parede frágil
                        case 3: cout<<char(51); break; //inimigo
                        case 4: cout<<char(208); break; //bomba
                        case 5: cout<<char(35); break; //explosão
                        //default: cout<<"-"; //erro
                    } //fim switch
                }
            }
            cout<<"\n";
        } //fim do mapa

         //Atualização da posição/coordenadas do personagem
        int x1=x, y1=y;

        ///executa os movimentos
         if ( _kbhit() ){
            tecla = getch();
            if(tecla == 72 || tecla == 'w'){//Cima
                x1--;
            }else if(tecla == 80 || tecla == 's'){//Baixo
                x1++;
            }else if(tecla == 75 || tecla == 'a'){//Esquerda
                y1--;
            }else if(tecla == 77 || tecla == 'd'){//Direita
                y1++;
            }else if(tecla == 32){//Colocaa bomba
                if(!bombaColocada){// Checa se a bomba ja foi colocada
                    m[x][y] = 4; //coloca a bomba na posicao
                    bx = x;
                    by = y;
                    bombaColocada = true;
                    tempoInicialDaBomba = clock(); // comeca o timer da bomba
                }
            }
        }

        ///Colisao do jogador nas paredes, bombas, etc
        if(m[x1][y1] != 1 && m[x1][y1] != 2 && m[x1][y1] != 4){
            x=x1, y=y1;
            movimentoDoPersonagem = true; // O personagen saiu da mesma posicao da bomba
        }

        // Condição de vitória
        if(quantidadeInimiga(m, inimigos) == 0){
            cout<<"Vitoria! Todos os inimigos derrotados!";
            break;
        }

        //Condição de derrota
        if(m[x][y] == 3){
            cout<<"Game Over! Morte por ataque inimigo!";
            break;
        }else if(m[x][y] == 5){
            cout<<"Game Over! Morte por presenca em area de explosao";
            break;
        }

    // Checa o timer da bomba e se explodiu
    if (bombaColocada && movimentoDoPersonagem) {
        explosaoBomba(m, bx, by, bombaColocada, movimentoDoPersonagem, explosao, tempoInicialDaBomba);
    }
    // Movimenta inimigos
        clock_t tempoAtual = clock();
        if ((tempoAtual - tempoDeMovimentoInimigo) / (double)CLOCKS_PER_SEC >= 4.0) {
            quantidadeInimiga(m, inimigos); // Contagem e atualização da posição inimiga.
            movimentoInimigo(m, inimigos, bx, by, bombaColocada);
            tempoDeMovimentoInimigo = tempoAtual; // Atualiza o tempo de movimento dos inimigos.
        }

    }//encerramento do loop principal
return 0;
}
