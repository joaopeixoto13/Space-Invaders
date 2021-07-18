#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
#include <string.h>

#define MAXJOG 3
#define MAXCHARNOME 3
#define MAXVIDAS 3

#define MODOSOLO 1
#define MODODESAFIO 2
#define MODOEQUIPA 3

#define DIFFACIL 1
#define DIFMEDIA 2
#define DIFDIFICIL 3

#define EMJOGO 0
#define DERROTA 1
#define VITORIA 2

// Carateres de jogo
#define ARMADEVASTADORA 157
#define ARMASUPREMA 232
#define ARMAGALATICA 206
#define ESPACO 32
#define ESCUDO1 176
#define ESCUDO2 177
#define ESCUDO3 178
#define ALIEN1 87
#define ALIEN2 64
#define ALIEN3 143
#define TIROALIEN 248
#define TIROARMA 249
#define LINHA 196
#define EXPLOSAO1 120
#define EXPLOSAO2 88

// Ambiente jogo
#define MAXLIN 28 			// Total de linhas
#define MAXCOL 31 			// Total de colunas
#define MAXCOLEQ 61 		// Total de colunas para modo equipa
#define LINJOGADOR 26 		// Linha de movimento do jogador
#define LINHAFINAL 0		// Linha final
#define VELMOVALIEN 300 	// Velocidade de movimentacao dos aliens (invertido: maior = mais lento)
#define VELTIROALIEN 100 	// Velocidade de tiro dos aliens (invertido: maior = mais lento)
#define ESPACOENTRETIROS 5 	// Espera entre tiros consecutivos

// Teclas para solo
#define DIREITA 77
#define ESQUERDA 75
#define TIRO 87
#define SAIR 27

// Teclas para multijogador
#define P1E 65	// A - P1 - esquerda
#define P1D 68	// D - P1 - direita
#define P1T 87	// W - P1 - Tiro
#define P2E 74	// J - P2 - esquerda 
#define P2D 76	// L - P2 - direita
#define P2T 73	// I - P2 - Tiro
#define P3E 52	// 4 - P3 - esquerda		
#define P3D 54	// 6 - P3 - direita
#define P3T 56	// 8 - P3 - Tiro

// Variaveis globais
int gNumJog = 1;   // Numero de jogadores
int gModoJog = MODOEQUIPA;   // Modo de jogo
int gDificuldade = DIFFACIL;    // Dificuldade
char gNomeJog[MAXJOG][MAXCHARNOME];
char gArmaJog[MAXJOG];
int gPontuacaoJog[MAXJOG];
int gAliensRestantes[MAXJOG];
int gDirAliens[MAXJOG];
int gVidasJog[MAXJOG];
int gResultado[MAXJOG];
char matriz[MAXJOG][MAXLIN][MAXCOL]; 
char matrizEQ[MAXLIN][MAXCOLEQ];

//TOP10 SOLO && DESAFIO
char top10n[10][MAXCHARNOME];
int top10p[10];
//TOP10 EQUIPA
char top10nEQ[10][2*MAXCHARNOME];
int top10pEQ[10];

int menuInicial(); 
int menuJogadores();
int menuDificuldade();
char menuArma(int jogador);
void menuNome(int jogador);
void ecraInicial();
void ecraRetomar();
void jogo(int dificuldade, int novojogo);
void preparaMatriz(int modo, int njog);
int procuraPosJogador(int linha, int modo, int jog);
void processaTirosArma(int modo, int njog);
int decideTiroAlien(int dificuldade);
void geradorAtaqueAlien(int dificuldade, int modo, int njog);
void processaTirosAlien(int modo, int njog);
void moveAliens(int modo, int njog);
void cls(HANDLE hConsole);
void atualizaEcraJogo(int modo, int njog);
char moveJogadores(int modo, int njog, char tecla);
void finalizaMatriz(int modo, int njog);
void guardaJogo();
int carregaJogo();
void gravaTop10();
int carregaTop10();
void registaPontuacao(int modo, int jog);
void ecraTop10();

main()
{
    int sel = 0; // variavel selecao menu
    int op;    
    
	if(carregaTop10() != 1)
	{
		int n;
		for(n = 0 ; n < 10 ; n++)
		{
			//TOP10 SOLO && DESAFIO
			top10p[n] = 0;
			top10n[n][0] = '-';
			top10n[n][1] = '-';
			top10n[n][2] = '-';

			//TOP10 EQUIPAs
			top10pEQ[n] = 0;
			top10nEQ[n][0] = '-';
			top10nEQ[n][1] = '-';
			top10nEQ[n][2] = '-';			
			top10nEQ[n][3] = '-';			
			top10nEQ[n][4] = '-';			
			top10nEQ[n][5] = '-';		
		}
	}
	carregaJogo();

	// menu inicial
    while(sel != 5)
	{
		sel = menuInicial();
		switch(sel)
		{
			case 1:
				op = 0;
				op = menuJogadores();
				switch(op)
				{
					case 1:
						gNumJog = 1;
						gModoJog = MODOSOLO;
					break;
					case 2:
						gNumJog = 2;
						gModoJog = MODODESAFIO;
						break;
					case 3:
						gNumJog = 3;
						gModoJog = MODODESAFIO;
						break;
					case 4:
						gNumJog = 2;
						gModoJog = MODOEQUIPA;
						break;
					default:
						break;
				}
				
				int i;
				for(i = 0; i < gNumJog ; i++)
				{
					menuNome(i);
					gArmaJog[i] = menuArma(i);
					while (gModoJog == MODOEQUIPA && i == 1 && gArmaJog[0] == gArmaJog[1])
					{
						gArmaJog[i] = menuArma(i);	
					}
					gPontuacaoJog[i] = 0;
					gVidasJog[i] = MAXVIDAS;
					gDirAliens[i] = DIREITA;
					gResultado[i] = 0;
				}

				gDificuldade = menuDificuldade();
				ecraInicial();
				jogo(gDificuldade, 1);
				break;
			case 2:
				if(gModoJog == MODOEQUIPA || gModoJog == MODOSOLO)
				{
					if(gResultado[0] == EMJOGO)
					{
						ecraRetomar();
						jogo(gDificuldade, 0);
					}	
				}
				else if(gModoJog == MODODESAFIO)
				{
					if(gNumJog == 2)
					{
						if(gResultado[0] == EMJOGO || gResultado[1] == EMJOGO)
						{
							ecraRetomar();
							jogo(gDificuldade, 0);
						}
					}
					else if (gNumJog == 3)
					{
						if(gResultado[0] == EMJOGO || gResultado[1] == EMJOGO || gResultado[2] == EMJOGO)
						{
							ecraRetomar();
							jogo(gDificuldade, 0);
						}
					}
				}
				break;
			case 3:
				guardaJogo();
				break;
			case 4:
				ecraTop10();
				break;
			case 5:
				gravaTop10();
				system("cls");
				break;
		}
	}    
}

void jogo(int dificuldade, int novojogo)
{
    int contadorTiroAliens = 0;
	int contadorMovimento = 0;
    char keyPress = 0;
    
    system("cls");
	
	if(novojogo == 1)
	{
		preparaMatriz(gModoJog, gNumJog);
	} 
	
    while(keyPress != SAIR)
    {
        finalizaMatriz(gModoJog, gNumJog);

		// Atualiza ambiente de jogo
		processaTirosArma(gModoJog, gNumJog);
		
		if(contadorMovimento < (int) (VELMOVALIEN / gDificuldade))
        { 
            contadorMovimento++;
        }
		else
		{
	        moveAliens(gModoJog, gNumJog);
			contadorMovimento = 0;
		}
		if(contadorTiroAliens < (int) (VELTIROALIEN / gDificuldade))
        { 
            contadorTiroAliens++;
        }
        else
        {
			processaTirosAlien(gModoJog, gNumJog);
            geradorAtaqueAlien(gDificuldade, gModoJog, gNumJog);
            contadorTiroAliens = 0;
        }

		// Verifica movimento dos jogadores
		keyPress = moveJogadores(gModoJog, gNumJog, keyPress);
		
        atualizaEcraJogo(gModoJog, gNumJog);

		if(kbhit())
            keyPress = toupper(getch());
	}

	// Registar resultados
	if(gModoJog == MODOEQUIPA || gModoJog == MODOSOLO)
	{
		if(gResultado[0] != EMJOGO)
		{
			registaPontuacao(gModoJog, 0);
		}
	}
	else if(gModoJog == MODODESAFIO)
	{
		if(gNumJog == 2)
		{
			if(gResultado[0] != EMJOGO && gResultado[1] != EMJOGO)
			{
				registaPontuacao(gModoJog, 0);
				registaPontuacao(gModoJog, 1);
			}
		}
		else if (gNumJog == 3)
		{
			if(gResultado[0] != EMJOGO && gResultado[1] != EMJOGO && gResultado[2] != EMJOGO)
			{
				registaPontuacao(gModoJog, 0);
				registaPontuacao(gModoJog, 1);
				registaPontuacao(gModoJog, 2);
			}
		}
	}
}

void atualizaEcraJogo(int modo, int njog)
{
	char linhaP1[MAXCOL];
	char linhaP2[MAXCOL];
	char linhaP3[MAXCOL];
	char linhaEQ[MAXCOLEQ];
	
	// Limpar ecra
	HANDLE hStdout;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);    
	cls(hStdout);

	if (modo == MODOSOLO || modo == MODODESAFIO)
	{
		int n;
		for (n = 0 ; n < njog ; n++)
		{
			printf("| JOG %d<%c%c%c> PONT %06d A %02d V %d ", n + 1 , gNomeJog[n][0], gNomeJog[n][1], gNomeJog[n][2], gPontuacaoJog[n], gAliensRestantes[n], gVidasJog[n]);
		}
		printf("|\n");
		
		int i;
		int j;
		for (i = 0; i < MAXLIN ; i++)
		{
			for (j = 0 ; j < MAXCOL ; j++)
			{
				linhaP1[j] = matriz[0][i][j];
				if (njog >= 2)
				{
					linhaP2[j] = matriz[1][i][j];
				}
				if (njog >= 3)
				{
					linhaP3[j] = matriz[2][i][j];
				}
			}
			if (njog == 1)
			{
				printf("| %s |\n", &linhaP1);
			} else if (njog == 2)
			{
				printf("| %s | %s |\n", &linhaP1, &linhaP2);
			}else if (njog == 3)
			{
				printf("| %s | %s | %s |\n", &linhaP1, &linhaP2, &linhaP3);
			}
		}
	}
	else if (modo == MODOEQUIPA)
	{
		printf(" JOGADOR 1<%c%c%c> V%d JOGADOR 2<%c%c%c> V%d PONTUACAO %06d A%03d\n", gNomeJog[0][0], gNomeJog[0][1], gNomeJog[0][2], gVidasJog[0], gNomeJog[1][0], gNomeJog[1][1], gNomeJog[1][2], gVidasJog[1],gPontuacaoJog[0], gAliensRestantes[0]);
		int i;
		int j;
		
		for (i = 0; i < MAXLIN ; i++)
		{
			for (j = 0 ; j < MAXCOLEQ ; j++)
			{
				linhaEQ[j] = matrizEQ[i][j];
			}
			printf(" %s \n", &linhaEQ);
		}
	}
}

char moveJogadores(int modo, int njog, char tecla)
{
	int pos, pos1;

	if (modo == MODOSOLO)
	{
        pos = procuraPosJogador(LINJOGADOR, gModoJog, 0);
		if(tecla == ESQUERDA)
        {
            if(pos > 0)
            {
                matriz[0][LINJOGADOR][pos] = ESPACO;
                matriz[0][LINJOGADOR][pos-1] = gArmaJog[0];
            }
            tecla = 0;
        } else if (tecla == DIREITA)
        {
            if(pos < (MAXCOL-1) && pos >= 0)
            {
                matriz[0][LINJOGADOR][pos] = ESPACO;
                matriz[0][LINJOGADOR][pos+1] = gArmaJog[0];
            }
            tecla = 0;
        } else if (tecla == TIRO)
        {
			if (pos != -1)
			{
				int v = 0;
				int e;
				
				for (e = 1 ; e <= ESPACOENTRETIROS ; e++)
				{
					if (matriz[0][LINJOGADOR-e][pos] == (char)TIROARMA)
					{
						v++;
					}
				}
				if (v == 0) 
				{
					matriz[0][LINJOGADOR-1][pos] = TIROARMA;
				}
			}            
            tecla = 0;
        }
	}
	else if (modo == MODODESAFIO)
	{
		int n;
		for (n = 0 ; n < njog ; n++)
		{
			pos = procuraPosJogador(LINJOGADOR, gModoJog, n);
			if((tecla == P1E  && n == 0) || (tecla == P2E  && n == 1) || (tecla == P3E && n == 2))
			{
				if(pos > 0)
				{
					matriz[n][LINJOGADOR][pos] = ESPACO;
					matriz[n][LINJOGADOR][pos-1] = gArmaJog[n];
				}
				tecla = 0;
			} else if((tecla == P1D && n == 0) || (tecla == P2D && n == 1) || (tecla == P3D && n == 2))
			{
				if(pos < (MAXCOL-1) && pos >= 0)
				{
					matriz[n][LINJOGADOR][pos] = ESPACO;
					matriz[n][LINJOGADOR][pos+1] = gArmaJog[n];
				}
				tecla = 0;
			} else if((tecla == P1T && n == 0) || (tecla == P2T && n == 1) || (tecla == P3T && n == 2))
			{
				if (pos != -1)
				{
					int v = 0;
					int e;
					
					for (e = 1 ; e <= ESPACOENTRETIROS ; e++)
					{
						if (matriz[n][LINJOGADOR-e][pos] == (char)TIROARMA)
						{
							v++;
						}
					}
					if (v == 0) 
					{
						matriz[n][LINJOGADOR-1][pos] = TIROARMA;
					}
				}            
				tecla = 0;
			}
		}
	}
	else if(modo == MODOEQUIPA)
	{
		pos = procuraPosJogador(LINJOGADOR, gModoJog, 0);
		pos1 = procuraPosJogador(LINJOGADOR, gModoJog,1);

		if(tecla == P1E)
		{
			if(pos > 0)
			{
				if (matrizEQ[LINJOGADOR][pos-1] == ESPACO)
				{
					matrizEQ[LINJOGADOR][pos] = ESPACO;
					matrizEQ[LINJOGADOR][pos-1] = gArmaJog[0];
				}
			}
			tecla = 0;
		} 
		else if (tecla == P3E)
		{
			if(pos1 > 0)
			{
				if (matrizEQ[LINJOGADOR][pos1-1] == ESPACO)
				{
					matrizEQ[LINJOGADOR][pos1] = ESPACO;
					matrizEQ[LINJOGADOR][pos1-1] = gArmaJog[1];
				}
			}
			tecla = 0;		
		}
		else if(tecla == P1D)
		{
			if(pos < (MAXCOLEQ-1) && pos >= 0)
			{
				if (matrizEQ[LINJOGADOR][pos+1] == ESPACO)
				{
					matrizEQ[LINJOGADOR][pos] = ESPACO;
					matrizEQ[LINJOGADOR][pos+1] = gArmaJog[0];
				}
			}
			tecla = 0;
		} else if(tecla == P3D)
		{
			if(pos1 < (MAXCOLEQ-1) && pos1 >= 0)
			{
				if (matrizEQ[LINJOGADOR][pos1+1] == ESPACO)
				{
					matrizEQ[LINJOGADOR][pos1] = ESPACO;
					matrizEQ[LINJOGADOR][pos1+1] = gArmaJog[1];
				}
			}
			tecla = 0;
		} else if(tecla == P1T)
		{
			if (pos != -1)
			{			
				int v = 0;
				int e;
				
				for (e = 1 ; e <= ESPACOENTRETIROS ; e++)
				{
					if (matrizEQ[LINJOGADOR-e][pos] == (char)TIROARMA)
					{
						v++;
					}
				}
				if (v == 0) 
				{
					matrizEQ[LINJOGADOR-1][pos] = TIROARMA;
				}
			}
			tecla = 0;
		} else if(tecla == P3T)
		{
			if (pos1 != -1)
			{			
				int v = 0;
				int e;
				
				for (e = 1 ; e <= ESPACOENTRETIROS ; e++)
				{
					if (matrizEQ[LINJOGADOR-e][pos1] == (char)TIROARMA)
					{
						v++;
					}
				}
				if (v == 0) 
				{
					matrizEQ[LINJOGADOR-1][pos1] = TIROARMA;
				}
			}
			tecla = 0;
		}
	}
	return tecla;
}

void moveAliens(int modo, int njog) 
{
    int baixar;
	int n;
	int i;
	
	if (modo == MODOSOLO || modo == MODODESAFIO)
	{
		for (n = 0 ; n < njog ; n++)
		{
			baixar = 0;
			if(gDirAliens[n] == ESQUERDA)
			{
				for (i = 0; i < MAXLIN ; i++)
				{
					if(matriz[n][i][0] == (char)ALIEN1 || matriz[n][i][0] == (char)ALIEN2 || matriz[n][i][0] == (char)ALIEN3)
					{
						baixar = 1;
						break;	
					}
				}
			}
			else if	(gDirAliens[n] == DIREITA)
			{
				int i;
				for (i = 0; i < MAXLIN ; i++)
				{
					if(matriz[n][i][MAXCOL-1] == (char)ALIEN1 || matriz[n][i][MAXCOL-1] == (char)ALIEN2 || matriz[n][i][MAXCOL-1] == (char)ALIEN3)
					{
						baixar = 1;
						break;	
					}
				}
			}		
			if (baixar == 1)
			{
				int j;
				for (j = 0 ; j < MAXCOL ; j++)
				{
					for (int i = MAXLIN - 1; i >= 0 ; i--)
					{
						if(matriz[n][i][j] == (char)ALIEN1 || matriz[n][i][j] == (char)ALIEN2 || matriz[n][i][j] == (char)ALIEN3)
						{
							matriz[n][i+1][j] = matriz[n][i][j]; // Move alien para baixo
							matriz[n][i][j] = ESPACO;
							if (i == LINJOGADOR-2)
							{
								gResultado[n] = DERROTA;
							}							
						}	
					}
				}
			}
			else
			{
				int i;
				for (i = MAXLIN - 1; i >= 0 ; i--)
				{
					if(gDirAliens[n] == ESQUERDA)
					{
						int j;
						for (j = 0 ; j < MAXCOL ; j++)
						{
							if(matriz[n][i][j] == (char)ALIEN1 || matriz[n][i][j] == (char)ALIEN2 || matriz[n][i][j] == (char)ALIEN3)
							{								
								matriz[n][i][j-1] = matriz[n][i][j]; // Move alien para esquerda
								matriz[n][i][j] = ESPACO;
							}
						}	
					}
					else if(gDirAliens[n] == DIREITA)
					{
						int j;
						for (j = MAXCOL-1 ; j >= 0 ; j--)
						{
							if(matriz[n][i][j] == (char)ALIEN1 || matriz[n][i][j] == (char)ALIEN2 || matriz[n][i][j] == (char)ALIEN3)
							{
								matriz[n][i][j+1] = matriz[n][i][j]; // Move alien para direita
								matriz[n][i][j] = ESPACO;
							}
						}							
					}
				}
			}
			if(baixar == 1 && gDirAliens[n] == ESQUERDA) // Para mudar a direcao na proxima chamada de funcao
			{
				gDirAliens[n] = DIREITA;	
			} 
			else if(baixar == 1 && gDirAliens[n] == DIREITA)
			{
				gDirAliens[n] = ESQUERDA;
			}
		}
	}
	else if (modo == MODOEQUIPA)
	{
		baixar = 0;
		if(gDirAliens[0] == ESQUERDA)
		{
			int i;
			for (i = 0; i < MAXLIN ; i++)
			{
				if(matrizEQ[i][0] == (char)ALIEN1 || matrizEQ[i][0] == (char)ALIEN2 || matrizEQ[i][0] == (char)ALIEN3)
				{
					baixar = 1;
					break;	
				}
			}
		}
		else if	(gDirAliens[0] == DIREITA)
		{
			int i;
			for (i = 0; i < MAXLIN ; i++)
			{
				if(matrizEQ[i][MAXCOLEQ-1] == (char)ALIEN1 || matrizEQ[i][MAXCOLEQ-1] == (char)ALIEN2 || matrizEQ[i][MAXCOLEQ-1] == (char)ALIEN3)
				{
					baixar = 1;
					break;	
				}
			}
		}		
		if (baixar == 1)
		{
			int j;
			int i;
			
			for (j = 0 ; j < MAXCOLEQ ; j++)
			{
				for (i = MAXLIN - 1; i >= 0 ; i--)
				{
					if(matrizEQ[i][j] == (char)ALIEN1 || matrizEQ[i][j] == (char)ALIEN2 || matrizEQ[i][j] == (char)ALIEN3)
					{
						matrizEQ[i+1][j] = matrizEQ[i][j]; // Move alien para baixo
						matrizEQ[i][j] = ESPACO;
						if (i == LINJOGADOR-2)
						{
							gResultado[0] = DERROTA;
						}							
					}	
				}
			}
		}
		else
		{
			int i;
			for (i = MAXLIN - 1; i >= 0 ; i--)
			{
				if(gDirAliens[0] == ESQUERDA)
				{
					int j;
					for (j = 0 ; j < MAXCOLEQ ; j++)
					{
						if(matrizEQ[i][j] == (char)ALIEN1 || matrizEQ[i][j] == (char)ALIEN2 || matrizEQ[i][j] == (char)ALIEN3)
						{								
							matrizEQ[i][j-1] = matrizEQ[i][j]; // Move alien para esquerda
							matrizEQ[i][j] = ESPACO;
						}
					}	
				}
				else if(gDirAliens[0] == DIREITA)
				{
					int j;
					for (j = MAXCOLEQ-1 ; j >= 0 ; j--)
					{
						if(matrizEQ[i][j] == (char)ALIEN1 || matrizEQ[i][j] == (char)ALIEN2 || matrizEQ[i][j] == (char)ALIEN3)
						{
							matrizEQ[i][j+1] = matrizEQ[i][j]; // Move alien para direita
							matrizEQ[i][j] = ESPACO;
						}
					}							
				}
			}
		}
		if(baixar == 1 && gDirAliens[0] == ESQUERDA) // Para mudar a direcao na proxima chamada de funcao
		{
			gDirAliens[0] = DIREITA;	
		} 
		else if(baixar == 1 && gDirAliens[0] == DIREITA)
		{
			gDirAliens[0] = ESQUERDA;
		}
	}
}

void processaTirosArma(int modo, int njog)
{
    if (modo == MODOSOLO || modo == MODODESAFIO)
	{
		int n;
		int i;
		int j;
		
		for (n = 0 ; n < njog ; n++)
		{
			for (i = 0; i < MAXLIN ; i++)
			{
				for (j = 0 ; j < MAXCOL ; j++)
				{
					if(matriz[n][i][j] == (char)TIROARMA)
					{
						if(i == LINHAFINAL)
						{
							matriz[n][i][j] = EXPLOSAO1;
						} 
						else if(matriz[n][i-1][j] == (char)ALIEN1)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i-1][j] = EXPLOSAO1;
							gPontuacaoJog[n] = gPontuacaoJog[n] + (((MAXLIN-1)-(i-1))*3);    //Fórmula para contar a pontuação
							gAliensRestantes[n]--;                                           //Quanto mais próximo da linha final, mais pontos se ganha
							if (gAliensRestantes[n] == 0)
							{
								gResultado[n] = VITORIA;
							}
						} 
						else if(matriz[n][i-1][j] == (char)ALIEN2)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i-1][j] = ALIEN1;
							gPontuacaoJog[n] = gPontuacaoJog[n] + (((MAXLIN-1)-(i-1))*6);
						} 
						else if(matriz[n][i-1][j] == (char)ALIEN3)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i-1][j] = ALIEN2;
							gPontuacaoJog[n] = gPontuacaoJog[n] + (((MAXLIN-1)-(i-1))*9);
						} 
						else if(matriz[n][i-1][j] == (char)TIROALIEN)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i-1][j] = EXPLOSAO1;
						} 
						else if(matriz[n][i-1][j] == (char)TIROALIEN)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i-1][j] = EXPLOSAO1;
						} 
						else if(matriz[n][i-1][j] == (char)ESCUDO1)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i-1][j] = ESPACO;
						} 
						else if(matriz[n][i-1][j] == (char)ESCUDO2)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i-1][j] = ESCUDO1;
						} 
						else if(matriz[n][i-1][j] == (char)ESCUDO3)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i-1][j] = ESCUDO2;
						} 
						else if(matriz[n][i-1][j] == (char)ESPACO)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i-1][j] = TIROARMA;
						}
					} 
					else if (matriz[n][i][j] == (char)EXPLOSAO2)
					{
						matriz[n][i][j] = ESPACO;
					} 
					else if (matriz[n][i][j] == (char)EXPLOSAO1)
					{
						matriz[n][i][j] = EXPLOSAO2;
					}
				}
			}
		}
	}  else if (modo == MODOEQUIPA)  
	{
		int i;
		int j;
		
		for (i = 0; i < MAXLIN ; i++)
		{
			for (j = 0 ; j < MAXCOLEQ ; j++)
			{
				if(matrizEQ[i][j] == (char)TIROARMA)
				{
					if(i == LINHAFINAL)
					{
						matrizEQ[i][j] = EXPLOSAO1;
					} 
					else if(matrizEQ[i-1][j] == (char)ALIEN1)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i-1][j] = EXPLOSAO1;
						gPontuacaoJog[0] = gPontuacaoJog[0] + (((MAXLIN-1)-(i-1))*3);
						gAliensRestantes[0]--;
						if (gAliensRestantes[0] == 0)
						{
							gResultado[0] = VITORIA;
						}
					} 
					else if(matrizEQ[i-1][j] == (char)ALIEN2)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i-1][j] = ALIEN1;
						gPontuacaoJog[0] = gPontuacaoJog[0] + (((MAXLIN-1)-(i-1))*6);
					} 
					else if(matrizEQ[i-1][j] == (char)ALIEN3)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i-1][j] = ALIEN2;
						gPontuacaoJog[0] = gPontuacaoJog[0] + (((MAXLIN-1)-(i-1))*9);
					} 
					else if(matrizEQ[i-1][j] == (char)TIROALIEN)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i-1][j] = EXPLOSAO1;
					} 
					else if(matrizEQ[i-1][j] == (char)TIROALIEN)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i-1][j] = EXPLOSAO1;
					} 
					else if(matrizEQ[i-1][j] == (char)ESCUDO1)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i-1][j] = ESPACO;
					} 
					else if(matrizEQ[i-1][j] == (char)ESCUDO2)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i-1][j] = ESCUDO1;
					} 
					else if(matrizEQ[i-1][j] == (char)ESCUDO3)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i-1][j] = ESCUDO2;
					} 
					else if(matrizEQ[i-1][j] == (char)ESPACO)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i-1][j] = TIROARMA;
					}
				} 
				else if (matrizEQ[i][j] == (char)EXPLOSAO2)
				{
					matrizEQ[i][j] = ESPACO;
				} 
				else if (matrizEQ[i][j] == (char)EXPLOSAO1)
				{
					matrizEQ[i][j] = EXPLOSAO2;
				}
			}
		}
	}
}

void processaTirosAlien(int modo, int njog)
{
    if (modo == MODOSOLO || modo == MODODESAFIO)
	{
		int n;
		int i;
		int j;
		
		for (n = 0 ; n < njog ; n++)
		{
			for (i = MAXLIN - 1; i >= 0 ; i--)
			{
				for (j = 0 ; j < MAXCOL ; j++)
				{
					if(matriz[n][i][j] == (char)TIROALIEN)
					{
						if(i == LINJOGADOR)
						{
							matriz[n][i][j] = EXPLOSAO1;
						} else if(matriz[n][i+1][j] == (char)gArmaJog[n])
						{
							matriz[n][i][j] = ESPACO;
							if (gVidasJog[n] > 0)
							{
								gVidasJog[n]--;
							}
							else
							{
								matriz[n][i+1][j] = EXPLOSAO1;
								if (gVidasJog[n] == 0)
								{
									gResultado[n] = DERROTA;
								}
							}
						} else if(matriz[n][i+1][j] == (char)ESCUDO1)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i+1][j] = ESPACO;
						} else if(matriz[n][i+1][j] == (char)ESCUDO2)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i+1][j] = ESCUDO1;
						} else if(matriz[n][i+1][j] == (char)ESCUDO3)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i+1][j] = ESCUDO2;
						} else if(matriz[n][i+1][j] == (char)ESPACO)
						{
							matriz[n][i][j] = ESPACO;
							matriz[n][i+1][j] = TIROALIEN;
						} 
					} 
				}
			}
		}
	}
	else if (modo == MODOEQUIPA)
	{
		int i;
		int j;
		
		for (i = MAXLIN - 1; i >= 0 ; i--)
		{
			for (j = 0 ; j < MAXCOLEQ ; j++)
			{
				if(matrizEQ[i][j] == (char)TIROALIEN)
				{
					if(i == LINJOGADOR)
					{
						matrizEQ[i][j] = EXPLOSAO1;
					} 
					else if(matrizEQ[i+1][j] == (char)gArmaJog[0])
					{
						matrizEQ[i][j] = ESPACO;
						if (gVidasJog[0] > 0)
						{
							gVidasJog[0]--;
						}
						else
						{
							matrizEQ[i+1][j] = EXPLOSAO1;
							if (gVidasJog[0] == 0 && gVidasJog[1] == 0)
							{
								gResultado[0] = DERROTA;
							}
						}
						
					} 
					else if(matrizEQ[i+1][j] == (char)gArmaJog[1] )
					{
						matrizEQ[i][j] = ESPACO;
						if (gVidasJog[1] > 0)
						{
							gVidasJog[1]--;
						}
						else
						{
							matrizEQ[i+1][j] = EXPLOSAO1;
							gResultado[0] = DERROTA;
						}
						
					} else if(matrizEQ[i+1][j] == (char)ESCUDO1)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i+1][j] = ESPACO;
					} else if(matrizEQ[i+1][j] == (char)ESCUDO2)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i+1][j] = ESCUDO1;
					} else if(matrizEQ[i+1][j] == (char)ESCUDO3)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i+1][j] = ESCUDO2;
					} else if(matrizEQ[i+1][j] == (char)ESPACO)
					{
						matrizEQ[i][j] = ESPACO;
						matrizEQ[i+1][j] = TIROALIEN;
					} 
				} 
			}
		}		
	}    
}

void geradorAtaqueAlien(int dificuldade, int modo, int njog)
{
    if (modo == MODOSOLO || modo == MODODESAFIO)
	{
		int n;
		int j;
		int i;
		
		for (n = 0 ; n < njog ; n++)
		{
			for (j = 0 ; j < MAXCOL ; j++)
			{
				for (i = MAXLIN - 1; i >= 0 ; i--)
				{
					if(matriz[n][i][j] == (char)  ALIEN1 || matriz[n][i][j] == (char)  ALIEN2 || matriz[n][i][j] == (char)  ALIEN3)
					{
						if(matriz[n][i+1][j] == (char) ESPACO && decideTiroAlien(dificuldade) == 1)
						{
							matriz[n][i+1][j] = TIROALIEN;
						}
						i = -1;
					}
				}
			}
		}
	}
	else if (modo == MODOEQUIPA)
	{
		int j;
		int i;
		
		for (j = 0 ; j < MAXCOLEQ ; j++)
		{
			for (i = MAXLIN - 1; i >= 0 ; i--)
			{
				if(matrizEQ[i][j] == (char)  ALIEN1 || matrizEQ[i][j] == (char)  ALIEN2 || matrizEQ[i][j] == (char)  ALIEN3)
				{
					if(matrizEQ[i+1][j] == (char) ESPACO && decideTiroAlien(dificuldade) == 1)
					{
						matrizEQ[i+1][j] = TIROALIEN;
					}
					i = -1;
				}
			}
		}
	}
}

int decideTiroAlien(int dificuldade)
{
    int res;
    int prob_min = 1;
    res = rand() % 100 + 1;
    if(res <= prob_min*dificuldade)     //Dificuldade-> Probabilidade do Allien de lançar tiros 
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int procuraPosJogador(int linha, int modo, int jog)
{
    int posicao = -1;

    if(modo == MODOSOLO || modo == MODODESAFIO)
    {
		int j;    
        for (j=0;j<MAXCOL;j++)
        { 
            if (matriz[jog][linha][j] == gArmaJog[jog])
                posicao = j;
        }     
    } else if (modo == MODOEQUIPA)
   	{
	   	int j;    
        for (j=0;j<MAXCOLEQ;j++)
        { 
            if (matrizEQ[linha][j] == gArmaJog[jog])
                posicao = j;
        }     
    }	
    return posicao;
}

void preparaMatriz(int modo, int njog)
{
	int i;
	int j;
	int n;
	
    if(modo == MODOSOLO || modo == MODODESAFIO)
    {
        for (n = 0; n < njog; n++)
		{
			gAliensRestantes[n] = 0;
			
			// Espaco
		
			for (i = 0 ; i < MAXLIN ; i++) 
			{
				for (j = 0 ; j < MAXCOL ; j++)
				{ 
					matriz[n][i][j] = ESPACO;
				}
			}
			// Aliens
			
			for (i = 1 ; i < 4 ; i = i+2)
			{
				for (j = 2 ; j < 23 ; j = j+2)
				{
					matriz[n][i][j] = ALIEN1;
					gAliensRestantes[n]++;
				}
			}
			
			
			for (i = 5 ; i < 8 ; i = i + 2)
			{
				for (j = 2 ; j < 23 ; j = j+2)
				{
					matriz[n][i][j] = ALIEN2;
					gAliensRestantes[n]++;
				}
			}
			
		
			for (j = 2 ; j < 23 ; j = j+2)
			{
					matriz[n][9][j] = ALIEN3;
					gAliensRestantes[n]++;
			}
			
			// Escudos
			
			for (j = 3 ; j < 25 ; j = j+7)
			{
				matriz[n][LINJOGADOR-4][j+1] = ESCUDO3;
				matriz[n][LINJOGADOR-4][j+2] = ESCUDO3;
				matriz[n][LINJOGADOR-3][j] = ESCUDO3;
				matriz[n][LINJOGADOR-3][j+1] = ESCUDO3;
				matriz[n][LINJOGADOR-3][j+2] = ESCUDO3;
				matriz[n][LINJOGADOR-3][j+3] = ESCUDO3;
				matriz[n][LINJOGADOR-2][j] = ESCUDO3;
				matriz[n][LINJOGADOR-2][j+3] = ESCUDO3;
			}
			
			// Jogador
			matriz[n][LINJOGADOR][15] = gArmaJog[n];
			
			// Linha final
			
			for (j = 0 ; j < MAXCOL ; j++)
			{ 
				matriz[n][MAXLIN-1][j] = LINHA;
			}
		}    
    }
	else if (modo == MODOEQUIPA)
	{
        gAliensRestantes[0] = 0;
		
		// Espaco
		
        for (i = 0 ; i < MAXLIN ; i++) 
        {
            for (j = 0 ; j < MAXCOLEQ ; j++)
            { 
                matrizEQ[i][j] = ESPACO;
            }
        }
        // Aliens
       
        for (i = 1 ; i < 4 ; i = i+2)
        {
            for (j = 10 ; j < 50 ; j = j+2)
            {
                matrizEQ[i][j] = ALIEN1;
				gAliensRestantes[0]++;
            }
        }
        
        
        for (i = 5 ; i < 8 ; i = i+2)
        {
            for (j = 10 ; j < 50 ; j = j+2)
            {
                matrizEQ[i][j] = ALIEN2;~
				gAliensRestantes[0]++;
            }
        }
        
        for (j = 10 ; j < 50 ; j = j+2)
        {
                matrizEQ[9][j] = ALIEN3;
				gAliensRestantes[0]++;
        }
        
		// Escudos
		
		for (j = 4 ; j < 54 ; j = j+7)
        {
            matrizEQ[LINJOGADOR-4][j+1] = ESCUDO3;
			matrizEQ[LINJOGADOR-4][j+2] = ESCUDO3;
			matrizEQ[LINJOGADOR-3][j] = ESCUDO3;
			matrizEQ[LINJOGADOR-3][j+1] = ESCUDO3;
			matrizEQ[LINJOGADOR-3][j+2] = ESCUDO3;
			matrizEQ[LINJOGADOR-3][j+3] = ESCUDO3;
			matrizEQ[LINJOGADOR-2][j] = ESCUDO3;
			matrizEQ[LINJOGADOR-2][j+3] = ESCUDO3;
        }
        
		// Jogador
        matrizEQ[LINJOGADOR][20] = gArmaJog[0];
        matrizEQ[LINJOGADOR][40] = gArmaJog[1];
        
		// Linha final
		
        for (j = 0 ; j < MAXCOLEQ ; j++)
        { 
            matrizEQ[MAXLIN-1][j] = LINHA;
        }    		
	}
}

void finalizaMatriz(int modo, int njog)
{
    if(modo == MODOSOLO || modo == MODODESAFIO)
    {
    	int n;
        for (n = 0; n < njog; n++)
		{
			if (gResultado[n] == VITORIA)
			{
				int i;
				for (i = 0 ; i < MAXLIN ; i++) 
				{
					int j;
					for (j = 0 ; j < MAXCOL ; j++)
					{ 
						matriz[n][i][j] = ESPACO;
					}
				}
				matriz[n][10][12] = 'V';
				matriz[n][10][13] = 'I';
				matriz[n][10][14] = 'T';
				matriz[n][10][15] = 'O';
				matriz[n][10][16] = 'R';
				matriz[n][10][17] = 'I';
				matriz[n][10][18] = 'A';
			}
			else if (gResultado[n] == DERROTA)
			{
				int i;
				for (i = 0 ; i < MAXLIN ; i++) 
				{
					int j;
					for (j = 0 ; j < MAXCOL ; j++)
					{ 
						matriz[n][i][j] = ESPACO;
					}
				}
				matriz[n][10][12] = 'D';
				matriz[n][10][13] = 'E';
				matriz[n][10][14] = 'R';
				matriz[n][10][15] = 'R';
				matriz[n][10][16] = 'O';
				matriz[n][10][17] = 'T';
				matriz[n][10][18] = 'A';
			}
		}    
    }
	else if (modo == MODOEQUIPA)
	{
		if (gResultado[0] == VITORIA)
		{
			int i;
			for (i = 0 ; i < MAXLIN ; i++) 
        	{
        		int j;
            	for (j = 0 ; j < MAXCOLEQ ; j++)
            	{ 
                	matrizEQ[i][j] = ESPACO;
            	}
        	}
			matrizEQ[10][26] = 'V';
			matrizEQ[10][27] = 'I';
			matrizEQ[10][28] = 'T';
			matrizEQ[10][29] = 'O';
			matrizEQ[10][30] = 'R';
			matrizEQ[10][31] = 'I';
			matrizEQ[10][32] = 'A';
		}
		else if (gResultado[0] == DERROTA)
		{
			int i;
			for (i = 0 ; i < MAXLIN ; i++) 
        	{
        		int j;
            	for (j = 0 ; j < MAXCOLEQ ; j++)
            	{ 
                	matrizEQ[i][j] = ESPACO;
            	}
        	}
			matrizEQ[10][26] = 'D';
			matrizEQ[10][27] = 'E';
			matrizEQ[10][28] = 'R';
			matrizEQ[10][29] = 'R';
			matrizEQ[10][30] = 'O';
			matrizEQ[10][31] = 'T';
			matrizEQ[10][32] = 'A';
		}
	}
}

int menuInicial() 
{
    int op = 1;
	system("cls");
    printf("\n\n                                          SPACE INVADERS  \n \n \n");
	Sleep(50);
	printf("                                          1 - Novo jogo\n \n");
	Sleep(50);
	printf("                                          2 - Retomar jogo\n \n");
	Sleep(50);
	printf("                                          3 - Gravar jogo\n \n");
	Sleep(50);
	printf("                                          4 - Placar de pontuacoes\n \n");
	Sleep(50);
	printf("                                          5 - Sair\n \n");
	Sleep(50);
	printf("                                          Opcao: ");
	Sleep(50);
	scanf("%d", &op);
	return op;
}

int menuJogadores()
{
	int op = 1;
    system("cls");

    printf("\n\n                                          MODO DE JOGO E NUMERO DE JOGADORES\n\n\n");
	Sleep(50);
	printf("                                          1 - Modo Solo    [ 1 Jogador   ]\n\n");
	Sleep(50);
	printf("                                          2 - Modo Desafio [ 2 Jogadores ]\n\n");
	Sleep(50);
	printf("                                          3 - Modo Desafio [ 3 Jogadores ]\n\n");
	Sleep(50);
	printf("                                          4 - Modo Equipa  [ 2 Jogadores ]\n\n");
	Sleep(50);
	printf("                                          Opcao: ");
	Sleep(50);
	scanf("%d", &op);        
	return op;
}

char menuArma(int jogador)
{
    int op = 1;
    
    system("cls");
    printf("\n\n                                          SELECAO DE ARMA - JOGADOR %i\n\n\n", jogador + 1);
	Sleep(50);
	printf("                                          1 - Arma Devastadora [ %c ]\n\n", ARMADEVASTADORA);
	Sleep(50);
	printf("                                          2 - Arma Suprema     [ %c ]\n\n", ARMASUPREMA);
	Sleep(50);
	printf("                                          3 - Arma Galatica    [ %c ]\n\n", ARMAGALATICA);
	Sleep(50);
	printf("                                          Opcao: ");
	Sleep(50);
	scanf("%d", &op);        
	
    if (op == 3)
        return ARMAGALATICA;	
	else if(op == 2) 
        return ARMASUPREMA;
	else
	    return ARMADEVASTADORA;
}

void menuNome(int jogador)
{
    char nome[256];
    system("cls");
    printf("\n\n\n\n\n");
	Sleep(50);
	printf("                                          NOME JOGADOR %d \n\n", jogador + 1);
	Sleep(50);
	printf("                                          [ 3 LETRAS ]\n\n\n");
    Sleep(50);
	printf("                                          -> ");
	Sleep(50);
	scanf("%s", &nome);
    
    int i;
    for (i = 0; i < 3 ; i++)
        gNomeJog[jogador][i] = toupper(nome[i]);
}

int menuDificuldade()
{
	int op = 1;
    system("cls");
    printf("\n \n                                          DIFICULDADE\n\n\n");
	Sleep(50);
	printf("                                          1 - Facil\n\n");
	Sleep(50);
	printf("                                          2 - Media\n\n");
	Sleep(50);
	printf("                                          3 - Dificil\n\n");
	Sleep(50);
	printf("                                          Opcao: ");
	Sleep(50);
	scanf("%d", &op);
	return op;
}

void ecraInicial()
{
    system("cls");
    printf("\n\n                                          BEM VINDO COMANDANTE, ESTAVAMOS A SUA ESPERA!\n\n\n");
	Sleep(50);
	printf("                                          CHEGOU A HORA DE DERROTAR OS INIMIGOS!\n\n\n");
	Sleep(50);
	printf("                                          BOA SORTE!!!\n\n\n\n\n\n");
	Sleep(50);
	printf("                                          PRESSIONE QUALQUER TECLA PARA INICIAR");
	getch();
}

void ecraRetomar()
{
    system("cls");
    printf("\n\n                                          BEM VINDO DE VOLTA COMANDANTE!\n\n\n");
	Sleep(50);
	printf("                                          ESTAVAMOS A SUA ESPERA!\n\n\n");
	Sleep(50);
	printf("                                          BOA SORTE!!!\n\n\n\n\n\n");
	Sleep(50);
	printf("                                          PRESSIONE QUALQUER TECLA PARA RETOMAR");
	getch();
}

void ecraTop10()
{
   	system("cls");
    printf("\n                                          TOP 10 INDIVIDUAL\n\n");
	for(int n = 0 ; n < 10 ; n++)
	{
		printf("                                          %d - %c%c%c - %d pontos\n", n+1, top10n[n][0], top10n[n][1], top10n[n][2], top10p[n]);
		Sleep(50);
	}
	printf("\n                                          TOP 10 EQUIPAS\n");
	
	int n;
	for(n = 0 ; n < 10 ; n++)
	{
		printf("                                          %d - %c%c%c + %c%c%c  - %d pontos\n", n+1, top10nEQ[n][0], top10nEQ[n][1], top10nEQ[n][2], top10nEQ[n][3], top10nEQ[n][4], top10nEQ[n][5], top10pEQ[n]);
		Sleep(50);
	}
	Sleep(50);
	printf("\n                                          PRESSIONE QUALQUER TECLA PARA RETOMAR");
	getch();
}

void guardaJogo()
{
	FILE *file;
	file = fopen("UltimoJogo.txt", "w");

	fprintf(file, "%d %d %d \n", gModoJog, gDificuldade, gNumJog);

	if(gModoJog == MODOSOLO || gModoJog == MODODESAFIO)
	{
		int n;
		for (n = 0 ; n < gNumJog ; n++)
		{
			fprintf(file, " \n");
			fprintf(file, "%d %d %d ", (int)gNomeJog[n][0], (int)gNomeJog[n][1], (int)gNomeJog[n][2]);
			fprintf(file, "%d %d %d ", (int)gArmaJog[n], gPontuacaoJog[n], gAliensRestantes[n]);
			fprintf(file, "%d %d %d \n", gDirAliens[n], gVidasJog[n], gResultado[n]);
			
			int i;
			for(i = 0 ; i < MAXLIN ; i++)
			{
				int j;
				for(j = 0 ; j < MAXCOL ; j++)
				{
					fprintf(file, "%d ", (int)matriz[n][i][j]);
				}
				fprintf(file, " \n");
			}
		}
	}
	else if(gModoJog == MODOEQUIPA)
	{
		fprintf(file, " \n");
		fprintf(file, "%d %d %d ", (int)gNomeJog[0][0], (int)gNomeJog[0][1], (int)gNomeJog[0][2]);
		fprintf(file, "%d %d %d ", (int)gNomeJog[1][0], (int)gNomeJog[1][1], (int)gNomeJog[1][2]);
		fprintf(file, "%d %d %d ", (int)gArmaJog[0], (int)gArmaJog[1], gPontuacaoJog[0]);
		fprintf(file, "%d %d %d %d %d \n", gAliensRestantes[0], gDirAliens[0], gVidasJog[0], gVidasJog[1], gResultado[0]);
		
		int i;
		for(i = 0 ; i < MAXLIN ; i++)
		{
			int j;
			for(j = 0 ; j < MAXCOLEQ ; j++)
			{
				fprintf(file, "%d ", matrizEQ[i][j]);
			}
			fprintf(file, " \n");
		}
	}
	fclose(file);
}

int carregaJogo()
{
	int result = 0;
	FILE *file;
	file = fopen("UltimoJogo.txt", "r");
	char linha[500];
	char *aux;
	int t = 0;
	system("cls");
	if(file == NULL)
		return 0;
	
	if(fgets(linha, 500, file) == NULL)
		return 0;
	aux = strtok(linha, " ");
	t = 1;
	while (aux != NULL)
	{
		if(t==1) 	// Recupera Modo de Jogo
			gModoJog = atoi(aux);
			
		if(t==2)	// Recupera Dificuldade
			gDificuldade = atoi(aux);
			
		if(t==3)	// Recupera Numero de Jogadores
			gNumJog = atoi(aux);
		
		aux = strtok(NULL, " ");
		t++;
	}
	if(t < 5)	// Verifica se carregou todos os dados
		return 0;
	if(fgets(linha, 500, file) == NULL) // liberta linha separadora
		return 0; 
	if (gModoJog == MODOSOLO || gModoJog == MODODESAFIO)
	{
		int n;
		for(n = 0 ; n < gNumJog ; n++)
		{
			if(fgets(linha, 500, file) == NULL)
				return 0;
			aux = strtok(linha, " ");
			t = 1;
			while (aux != NULL)
			{
				if(t == 1)
					gNomeJog[n][0] = atoi(aux);
				if(t == 2)
					gNomeJog[n][1] = atoi(aux);					
				if(t == 3)
					gNomeJog[n][2] = atoi(aux);
				if(t == 4) 	// Recupera Arma Jog n
					gArmaJog[n] = atoi(aux);
				if(t == 5)	// Recupera Pontuacao Jog n
					gPontuacaoJog[n] = atoi(aux);
				if(t == 6)	// Recupera Numero de Aliens Restantes Jog n
					gAliensRestantes[n] = atoi(aux);
				if(t == 7) 	// Recupera Direcao Aliens Jog n
					gDirAliens[n] = atoi(aux);
				if(t == 8)	// Recupera Vidas Jog n
					gVidasJog[n] = atoi(aux);
				if(t == 9)	// Recupera Resultado Jog n
					gResultado[n] = atoi(aux);				

				aux = strtok(NULL, " ");
				t++;
			}
			if(t < 11)	// Verifica se carregou todos os dados
				return 0;			
		
			// Carrega Matriz jogador n
			int i;
			for(i = 0 ; i < MAXLIN ; i++)
			{
				if(fgets(linha, 500, file) == NULL)
					return 0;
				aux = strtok(linha, " ");
				t = 1;
				while (aux != NULL)
				{
					matriz[n][i][t-1] = atoi(aux);
					aux = strtok(NULL, " ");
					t++;
				}	
				if(t < MAXCOL)	// Verifica se carregou todos os dados
				return 0;	
			}
			if(fgets(linha, 500, file) == NULL) // Eliminar linha separadora
				return 0; 
		}
	}
	else if(gModoJog == MODOEQUIPA)
	{
		if(fgets(linha, 500, file) == NULL)
			return 0;
		aux = strtok(linha, " ");
		t = 1;
		while (aux != NULL)
		{
			if(t == 1)
				gNomeJog[0][0] = atoi(aux);
			if(t == 2)
				gNomeJog[0][1] = atoi(aux);					
			if(t == 3)
				gNomeJog[0][2] = atoi(aux);
			if(t == 4)
				gNomeJog[1][0] = atoi(aux);
			if(t == 5)
				gNomeJog[1][1] = atoi(aux);					
			if(t == 6)
				gNomeJog[1][2] = atoi(aux);
			if(t == 7) 	// Recupera Arma Jog 0
				gArmaJog[0] = atoi(aux);
			if(t == 8) 	// Recupera Arma Jog 1
				gArmaJog[1] = atoi(aux);
			if(t == 9)	// Recupera Pontuacao Jog 0
				gPontuacaoJog[0] = atoi(aux);
			if(t == 10)	// Recupera Numero de Aliens Restantes Jog 0
				gAliensRestantes[0] = atoi(aux);
			if(t == 11) 	// Recupera Direcao Aliens Jog 0
				gDirAliens[0] = atoi(aux);
			if(t == 12)	// Recupera Vidas Jog 0
				gVidasJog[0] = atoi(aux);
			if(t == 13)	// Recupera Vidas Jog 1
				gVidasJog[1] = atoi(aux);
			if(t == 14)	// Recupera Resultado Jog 0
				gResultado[0] = atoi(aux);				

			aux = strtok(NULL, " ");
			t++;
		}
		if(t < 16)	// Verifica se carregou todos os dados
		return 0;	

		// Carrega matriz de jogo equipa
		int i;
		for(i = 0 ; i < MAXLIN ; i++)
		{
			if(fgets(linha, 500, file) == NULL)
				return 0;
			aux = strtok(linha, " ");
			t = 1;
			while (aux != NULL)
			{
				matrizEQ[i][t-1] = atoi(aux);
				aux = strtok(NULL, " ");
				t++;
			}	
			if(t < MAXCOLEQ)	// Verifica se carregou todos os dados
				return 0;	
		}
	}
	fclose(file);
	return 1;
}

void gravaTop10()
{
	FILE *file;
	file = fopen("GuardaPontuacao.txt", "w");

	int n;
	for (n = 0 ; n < 10 ; n++)
	{
		fprintf(file, "%d %d %d %d \n", (int)top10n[n][0], (int)top10n[n][1], (int)top10n[n][2], top10p[n]);
	}
	
	for (n = 0 ; n < 10 ; n++)
	{
		fprintf(file, "%d %d %d ", (int)top10nEQ[n][0], (int)top10nEQ[n][1], (int)top10nEQ[n][2]);
		fprintf(file, "%d %d %d %d \n", (int)top10nEQ[n][3], (int)top10nEQ[n][4], (int)top10nEQ[n][5], top10pEQ[n]);
	}	
	fclose(file);
}

int carregaTop10()
{
	int result = 0;
	FILE *file;
	file = fopen("GuardaPontuacao.txt", "r");
	char linha[100];
	char *aux;
	int t = 0;
	system("cls");
	if(file == NULL)
		return 0;
	
	//TOP10 SOLO E DESAFIO
	int n;
	for(n = 0 ; n < 10 ; n++)
	{
		if(fgets(linha, 100, file) == NULL)
			return 0;
		aux = strtok(linha, " ");
		t = 0;
		while (aux != NULL)
		{
			if(t >= 0 && t < 3)
				top10n[n][t] = atoi(aux);
			if(t == 3) 	
				top10p[n] = atoi(aux);

			aux = strtok(NULL, " ");
			t++;
		}
		if(t < 5)	// Verifica se carregou todos os dados
			return 0;	
	}
	
	// TOP10 EQUIPA

	for(n = 0 ; n < 10 ; n++)
	{
		if(fgets(linha, 100, file) == NULL)
			return 0;
		aux = strtok(linha, " ");
		t = 0;
		while (aux != NULL)
		{
			if(t >= 0 && t < 6)
				top10nEQ[n][t] = atoi(aux);
			if(t == 6) 
				top10pEQ[n] = atoi(aux);

			aux = strtok(NULL, " ");
			t++;
		}
		if(t < 8)	// Verifica se carregou todos os dados
			return 0;	
	}
	fclose(file);
	return 1;
}

void registaPontuacao(int modo, int jog)
{
	int pAct,pTemp;
	char nAct[MAXCHARNOME];
	char nTemp[MAXCHARNOME];	
	char nActEQ[2*MAXCHARNOME];
	char nTempEQ[2*MAXCHARNOME];
	
	int i;
	int n;
	
	if(modo == MODOSOLO || modo == MODODESAFIO)
	{
		pAct = gPontuacaoJog[jog];
		
		for (i = 0 ; i < MAXCHARNOME ; i++)
		{
			nAct[i]=gNomeJog[jog][i];
		}	

		for(n = 0 ; n < 10 ; n++)
		{
			if(pAct > top10p[n])
			{
				// guarda pos atual no temp
				pTemp = top10p[n];
				for (i = 0 ; i < MAXCHARNOME ; i++)
				{
					nTemp[i]=top10n[n][i];
				}
				// atualiza pos atual
				top10p[n] = pAct;
				for (i = 0 ; i < MAXCHARNOME ; i++)
				{
					top10n[n][i] = nAct[i];
				}

				// atual passa ao conteudo da ultima pos
				pAct = pTemp;
				for (i = 0 ; i < MAXCHARNOME ; i++)
				{
					nAct[i]=nTemp[i];
				}				
			}
		}
	}
	else if(modo == MODOEQUIPA)
	{
		pAct = gPontuacaoJog[0];
		for (i = 0 ; i < 2*MAXCHARNOME ; i++)
		{
			nActEQ[i]=gNomeJog[0][i];
			nActEQ[i+3]=gNomeJog[1][i];			
		}	

		for(n = 0 ; n <  10 ; n++)
		{
			if(pAct > top10pEQ[n])
			{
				pTemp = top10pEQ[n];
				for (i = 0 ; i < 2*MAXCHARNOME ; i++)
				{
					nTempEQ[i]=top10nEQ[n][i];
				}
				
				top10pEQ[n] = pAct;
				for (i = 0 ; i < 2*MAXCHARNOME ; i++)
				{
					top10nEQ[n][i] = nActEQ[i];
				}

				pAct = pTemp;
				for (i = 0 ; i < 2*MAXCHARNOME ; i++)
				{
					nActEQ[i]=nTempEQ[i];
				}				
			}
		}
	}
}

void cls(HANDLE hConsole)
{
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(hConsole, &info);

   COORD coordScreen = { 0, 0 };    // home for the cursor 
   SetConsoleCursorPosition( hConsole, coordScreen );
}
