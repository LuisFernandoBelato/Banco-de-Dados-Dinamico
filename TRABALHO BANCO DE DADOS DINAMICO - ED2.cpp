#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<conio2.h>	

#define TAM 60 


struct PontDB
{
	char NomeBanco [TAM];
	struct PTabelas *ptrtabelas;
};
typedef struct PontDB PontDB;


struct PTabelas
{
	char TabelaNome [TAM];
	struct PCampos *ptrcampos;
	struct PTabelas *ant,*prox;
};
typedef struct PTabelas PTabelas;


struct PCampos
{
	struct PAtual *patual;
	struct PAtual *pdados;
	char NomeCampo [60];
	char Tipo;
	char PK;
	struct PCampos *FK;
	struct PCampos *prox;
};

// TABELAS DE CIMA Sï¿½O AS PRIMEIRAS E MAIS PRIMITIVAS / TABELA DE BAIXO CONTï¿½M UNION


union TIPOS
{
	int INTEGER;
	float NUMERIC;
	char DATE[15];
	char CHARACTER_1;
	char CHARACTER_20[30];
};

struct PAtual
{
	union TIPOS Tipos;
	struct PAtual *prox;
};
typedef struct PAtual PAtual;



// TAD PRA LISTA AUXLIAR (COLOCAR AS STRINGS DO INSERT)

struct ListaSelect
{
	char coluna[40];
	struct ListaSelect *prox;
};
typedef struct ListaSelect ListaSelect;


struct Lista
{
	char Coluna[900];
	char Value[900];
	struct Lista *prox;
};
typedef struct Lista Lista;


char IsEmpty (Lista *L)
{
	return L==NULL;
}

void Inserir (Lista **L, char Coluna[], char Valor[])
{
	Lista *Nova;
	if (*L == NULL)
	{
		*L = (Lista*) malloc(sizeof(Lista));
		strcpy((*L)->Coluna,Coluna);
		strcpy((*L)->Value,Valor);
		(*L)->prox = NULL;
	}
	else
	{
		Nova = (Lista*) malloc(sizeof(Lista));
		strcpy(Nova->Coluna,Coluna);
		strcpy(Nova->Value,Valor);
		Nova->prox = *L;
		*L = Nova;
	}
}

Lista* Retirar (Lista **L)
{
	Lista *Aux = *L;
	Lista *Retorno;
	Retorno = (Lista*) malloc(sizeof(Lista));
	
	strcpy(Retorno->Coluna,Aux->Coluna);
	strcpy(Retorno->Value,Aux->Value);
	
	*L = (*L)->prox;
	return Retorno;
}

Lista* RetirarFIM (Lista **L, Lista** Retorno)
{
	printf("INICIO\n");
	Lista *Aux = *L;
	*Retorno = (Lista*) malloc(sizeof(Lista));
	if ((*L)->prox == NULL)
	{
		printf("COMECO\n");
		strcpy((*Retorno)->Coluna,(*L)->Coluna);
		free(*L);
		*L = NULL;
		printf("termino\n");
	}
	else
	{
		while (Aux->prox != NULL && Aux->prox->prox != NULL)
			Aux = Aux->prox;
		strcpy((*Retorno)->Coluna,Aux->Coluna);
		free(Aux->prox);
		Aux->prox = NULL;
	}	
	printf("%s\n",(*Retorno)->Coluna);
}

// LISTA PARA GUARDAR AS TABELAS

struct ListaGuardaTabelas
{
	char NomeTabela[900];
	struct ListaGuardaTabelas *prox;
};

char IsEmpty (ListaGuardaTabelas *LT)
{
	return LT == NULL;
}

void Inserir (ListaGuardaTabelas **LT, char NomeTabela[])
{
	ListaGuardaTabelas *Nova;
	if (*LT == NULL)
	{
		*LT = (ListaGuardaTabelas*) malloc(sizeof(ListaGuardaTabelas));
		strcpy((*LT)->NomeTabela,NomeTabela);
		(*LT)->prox = NULL;
	}
	else
	{
		Nova = (ListaGuardaTabelas*) malloc(sizeof(ListaGuardaTabelas));
		strcpy((*LT)->NomeTabela,NomeTabela);
		Nova->prox = *LT;
		*LT = Nova;
	}
}

ListaGuardaTabelas* Retirar (ListaGuardaTabelas **LT)
{
	ListaGuardaTabelas *Aux = *LT;
	ListaGuardaTabelas *Retorno;
	Retorno = (ListaGuardaTabelas*) malloc(sizeof(ListaGuardaTabelas));
	
	strcpy(Retorno->NomeTabela,Aux->NomeTabela);
	
	*LT = (*LT)->prox;
	return Retorno;
}

// TAD FILA - PARA PERCORRER DO JEITO MAIS BRABO

// FILAS AUXLIARES

struct FILA
{
	struct PCampos *ptr;
	struct FILA *prox;
};
typedef struct FILA FILA;

void Init (FILA **F)
{
	*F = NULL;
}

char IsEmpty (FILA *F)
{
	return F == NULL;
}

void enqueue (FILA **F, PCampos *L)
{
	FILA *Nova;
	Nova = (FILA*) malloc(sizeof(FILA));
	Nova->prox = NULL;
	Nova->ptr = L;
	
	if (*F == NULL)
	{
		*F = Nova;
	}
	else
	{
		FILA *Aux = *F;
		while (Aux->prox != NULL)
			Aux = Aux->prox;
		Aux->prox = Nova;
	}
}

void dequeue (FILA **F, PCampos **L)
{
	*L = (*F)->ptr;
	if (*F != NULL)
		*F = (*F)->prox;
}

// PILHA - Inverter a Fila

struct Pilha
{
	struct PCampos *ptr;
	struct Pilha *prox;
};
typedef struct Pilha Pilha;

void Init (Pilha **P)
{
	*P = NULL;
}

void Push (Pilha **P, PCampos *ptr)
{
	Pilha *Nova;
	Nova = (Pilha*) malloc(sizeof(Pilha));
	Nova->ptr = ptr;
	Nova->prox = NULL;
	
	if (*P == NULL)
		*P = Nova;
	else
	{
		Nova->prox = *P;
		*P = Nova;
	}
}

char IsEmpty (Pilha *P)
{
	return P == NULL;
}

void Pop (Pilha **P, PCampos **ptr)
{
	*ptr = (*P)->ptr;
	if (*P != NULL)
		*P = (*P)->prox;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


// BUSCAS - TABELA E COLUNA / INSERï¿½ï¿½O DE DADOS (INSERT)

PTabelas* BuscaTabela (PontDB *BANCODEDADOS, char NomeTabela[])
{
	PTabelas *Aux = BANCODEDADOS->ptrtabelas;
	while (Aux != NULL && strcmp(Aux->TabelaNome,NomeTabela) != 0)
	{
		Aux = Aux->prox;
	}
		
	return Aux;
}

PCampos* BuscaCampo (PTabelas *CabecaTabela, char NomeCampo[])
{
	PCampos *Aux = CabecaTabela->ptrcampos;
	while (Aux != NULL && strcmp(Aux->NomeCampo,NomeCampo) != 0)
		Aux = Aux->prox;	
	return Aux;
}

void INSERT_DADOS (PCampos **C, char Value[])
{
	int DADOint;
	float DADOfloat;
	PAtual *Nova;
	Nova = (PAtual*) malloc(sizeof(PAtual));
	
	// CONVERSï¿½O DE TIPOS
	
	if ((*C)->Tipo == 'I')
	{
		DADOint = atoi(Value);
		Nova->Tipos.INTEGER = DADOint; 
	}
	else if ((*C)->Tipo == 'N')
	{
		DADOfloat = atof(Value);
		Nova->Tipos.NUMERIC = DADOfloat;
	}
	else if ((*C)->Tipo == 'D')
	{
		strcpy(Nova->Tipos.DATE,Value);
	}
	else if ((*C)->Tipo == 'C')
	{
		Nova->Tipos.CHARACTER_1 = Value[1];
	}
	else 
	{
		strcpy(Nova->Tipos.CHARACTER_20,Value);
	}
	
	Nova->prox = NULL;
	
	if ((*C)->pdados == NULL) // LISTA VAZIA
	{
		(*C)->pdados = Nova;
		(*C)->patual = (*C)->pdados;
	}
	else // INSERE NO FINAL
 	{
		PAtual *Aux = (*C)->pdados;
		while (Aux->prox != NULL)
			Aux = Aux->prox;
		Aux->prox = Nova;
	}
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void CREATEDATABASE (PontDB **BANCODEDADOS, char nomedobanco[])
{
	// ALOCAR PONTEIRO DO BANCO
	(*BANCODEDADOS) = (PontDB*) malloc(sizeof(PontDB));
	strcpy((*BANCODEDADOS)->NomeBanco,nomedobanco);
	(*BANCODEDADOS)->ptrtabelas = NULL;
}

void CREATETABLE (PontDB **BANCODEDADOS, char NomeTabela[])
{
	PTabelas *Nova;
	Nova = (PTabelas*) malloc(sizeof(PTabelas));
	Nova->ptrcampos = NULL;
	Nova->ant = Nova->prox = NULL;
	strcpy(Nova->TabelaNome,NomeTabela);
	
	// BANCO SEM NENHUMA TABELA
	if ((*BANCODEDADOS)->ptrtabelas == NULL)
	{
		(*BANCODEDADOS)->ptrtabelas = Nova;
	}
	// BANCO COM PELO MENOS UM TABELA
	else
	{
		PTabelas *Aux = (*BANCODEDADOS)->ptrtabelas;
		while (Aux->prox != NULL)
			Aux = Aux->prox;
		Aux->prox = Nova;
	}
}

void CREATECAMPO (PontDB *BANCODEDADOS,char NomeTabela[], char NomeCampo[], char TIPO)
{
	// ALOCAï¿½ï¿½O PADRï¿½O
	
	PCampos *Nova;
	Nova = (PCampos*) malloc(sizeof(PCampos));
	Nova->PK = 'N';
	Nova->FK = NULL;
	Nova->prox = NULL;
	Nova->patual = NULL;
	Nova->pdados = NULL;
	strcpy(Nova->NomeCampo,NomeCampo);
	
	// TIPO DE DADO
	
	Nova->Tipo = TIPO;
	
	// PROCURAR A TABELA
	
	PTabelas *Aux = BANCODEDADOS->ptrtabelas;
	Aux = BuscaTabela(BANCODEDADOS,NomeTabela);
	
	if (Aux != NULL)
	{
		// INSERIR NA LISTA
		
		if (Aux->ptrcampos == NULL)
		{
			Aux->ptrcampos = Nova;
		}
		else
		{
			PCampos *AuxC = Aux->ptrcampos;
			while (AuxC->prox != NULL)
				AuxC = AuxC->prox;
			AuxC->prox = Nova;
		}
	}
	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////


void ModificaPK (PontDB *BANCODEDADOS, char NomeTabela[], char NomeCampo[])
{	
	PTabelas *AuxT;
	PCampos *AuxC;
	
 	AuxT = BuscaTabela(BANCODEDADOS,NomeTabela);
	if (AuxT != NULL)
	{
		AuxC = BuscaCampo(AuxT,NomeCampo);
		if (AuxC != NULL)
		{
			AuxC->PK = 'S';
		}
	}
}

void ModificaFK(PontDB *BANCODEDADOS,char TABELA_FK[],char COLUNA_FK[],char TABELA_REFERENCIADA[], char COLUNA_REFERENCIADA[])
{
	// TABELA_FK ï¿½ A TABELA QUE TEM A COLUNA COM A FK
	// COLUNA_FK TEM O NOME DA COLUNA QUE VAI TER A REFERENCIA
	// TABELA_REFERENCIADA ï¿½ A TABELA QUE TEM A COLUNA QUE VOU REFERENCIAR
	// COLUNA_REFERENCIADA ï¿½ A COLUNA REFERENCIADA
	
	PCampos *ColunaReferenciada;
	
	PTabelas *Aux;
	PCampos *AuxC;
	
	// BUSCAR A TABELA E COLUNA QUE VAI SER REFERENCIADA
	
	Aux = BuscaTabela(BANCODEDADOS,TABELA_REFERENCIADA);
	if (Aux != NULL)
	{
		AuxC = BuscaCampo(Aux,COLUNA_REFERENCIADA);
		if (AuxC != NULL)
		{
			ColunaReferenciada = AuxC;
		}
		// BUSCAR A TABELA E COLUNA QUE VAI RECEBER A REFERENCIA
		
		Aux = BuscaTabela(BANCODEDADOS,TABELA_FK);
		if (Aux != NULL)
		{
			AuxC = BuscaCampo(Aux,COLUNA_FK);
			if (AuxC != NULL)
			{
				AuxC->FK = ColunaReferenciada;
			}
		}
	}
	
}


void AddDadosNoBanco(PontDB **BANCODEDADOS,char NomeTabela[],char Coluna[],char Value[])
{
	PTabelas *AuxT;
	PCampos *AuxC;
 	AuxT = BuscaTabela(*BANCODEDADOS,NomeTabela);
	if (AuxT != NULL)
	{
		AuxC = BuscaCampo(AuxT,Coluna);
		if (AuxC != NULL)
		{
			INSERT_DADOS(&AuxC,Value);	
		}
	}
} 




void LeituraScript (PontDB **BANCODEDADOS)
{
	// Abrir o Arquivo Texto e Ler a Primeira Linha
	FILE *ptrtxt = fopen("ScriptBanco.txt","r+");
	// Strings Principais
	char Plv1[700], Plv2[700], Plv3[700], Plv4[700], Plv5[700], Plv6[700], Plv7[700], Plv8[700];
	// Variaveis
	char StrAux[70];
	char Str[5];
	char TIPO;
	char StringArq[70];
	Str[1] = '\0';
	int i,j;
	int flag=0;
	textcolor(BLUE);
	printf("\t### ESTRUTURA DO BANCO DE DADOS ###\n");
	//system("pause");
	textcolor(WHITE);
	
	// Leitura para o CREATE DATABASE
	
	fscanf(ptrtxt,"%s %s %[^;];\n",&Plv1,&Plv2,&Plv3);
	if (strcmp(Plv1,"CREATE")==0)
		if (strcmp(Plv2,"DATABASE")==0)
			CREATEDATABASE(&(*BANCODEDADOS),Plv3);

	// Leitura dos CREATE TABLE
	
	char Parenteses;
	
	while (!feof(ptrtxt))
	{
		Plv1[0] = Plv2[0] = Plv3[0] = '\0';
		fscanf(ptrtxt,"%s %s %s %c\n",&Plv1,&Plv2,&Plv3,&Parenteses);
		flag=0;
		if (strcmp(Plv1,"CREATE") == 0 && strcmp(Plv2,"TABLE") == 0) // CRIAR A TABELA
		{
			CREATETABLE(&(*BANCODEDADOS),Plv3);
			strcpy(Plv8,Plv3);
			// CRIAR OS CAMPOS
			fscanf(ptrtxt,"%s %s %[^\n]",&Plv1,&Plv2,&Plv3);
			while(flag!=1)
			{
				
				// IF PRA PERGUNTAR SE ï¿½ CAMPO DE PK
				
				if (strcmp(Plv1,"CONSTRAINT") == 0) 
				{
					// PEGAR OS NOMES DOS CAMPOS QUE Vï¿½O VIRAR PK / StrAux ï¿½ o nome do Campo

					i=0;
					StrAux[0] = '\0';
					while(Plv3[i] != '(')
						i++;
					i++;
					while(Plv3[i] != ')')
					{
						j=0;
						while (Plv3[i] != ',' && Plv3[i] != ')')
							StrAux[j++] = Plv3[i++];
						StrAux[j] = '\0';
						
						if (Plv3[i] == ',')
							i+=2;
						
						// BUSCA O NOME DO CAMPO PARA MODIFICAR A PK
						ModificaPK(*BANCODEDADOS,Plv8,StrAux);
					}
					fscanf(ptrtxt,"%s\n",&Plv1);
					flag=1;
				}
				else // CAMPO NORMAL
				{
					// Ja que a String que sai do fscanf ï¿½ toda cagada, valida agora
					if (Plv2[0] == 'I')
						TIPO = 'I';
					else if (Plv2[0] == 'N')
						TIPO = 'N';
					else if (Plv2[0] == 'D')
						TIPO = 'D';
					else if (Plv2[10] == '1')
						TIPO = 'C';
					else
						TIPO = 'T';
					
					CREATECAMPO(*BANCODEDADOS,Plv8,Plv1,TIPO);
					fscanf(ptrtxt,"%s %s %[^\n]",&Plv1,&Plv2,&Plv3);
				}
			}
		}
		if (strcmp(Plv1,"ALTER") == 0) // LINHA DO ALTER TABLE
		{
			strcpy(Plv8,Plv3);
			fscanf(ptrtxt,"%[^\n]",Plv2);
			fscanf(ptrtxt,"%s %s (%[^)] %c %s %s (%[^)]\n",&Plv2,&Plv3,&Plv4,&Parenteses,&Plv5,&Plv6,&Plv7);
			fscanf(ptrtxt,"%s\n",&StrAux);
			// Plv8 ï¿½ A TABELA QUE TEM A COLUNA COM A FK
			// Plv4 TEM O NOME DA COLUNA QUE VAI TER A REFERENCIA
			// Plv6 ï¿½ A TABELA QUE TEM A COLUNA QUE VOU REFERENCIAR
			// Plv7 ï¿½ A COLUNA REFERENCIADA
			ModificaFK(*BANCODEDADOS,Plv8,Plv4,Plv6,Plv7);
		}
	}
}

void ExibeSelectTodascolunas (PTabelas *P);
void ExibeSelectalgumascolunas (PTabelas *AuxT,char colunaprintf[]);
void PercorreSemWhere (PTabelas *Tabela);
void SELECTAlgumasColunas (PTabelas *Tabela, FILA *F1);
void UPDATE (PTabelas *Tabela, char Coluna_Condicao[], char Coluna_Alterada[], char Valor_Comparar[], char Valor_Alterar[]);
void DELETEFROM (PTabelas *Tabela, char Coluna_Estatica[], char value[]);

void SELECT_WHERE_IGUALDADE (char Valor_Comparar[], PCampos *ColCondicao, PTabelas *Tabela, FILA *F1)
{
	FILA *F2; Init(&F2);
	PCampos *Aux;
	int flag=0;
	
	printf("\n\n");
	printf("### TABELA : %s ###\n\n", Tabela->TabelaNome);
	
	int valorint;
	float valorfloat;
	char valorchar;
	char valordata[900], valortexto[900];
	
	switch(ColCondicao->Tipo)
	{
		case 'I': valorint = atoi(Valor_Comparar); break;
		case 'N': valorfloat = atof(Valor_Comparar); break;
		case 'D': strcpy(valordata,Valor_Comparar); break;
		case 'C': valorchar = Valor_Comparar[1]; break;
		case 'T': strcpy(valortexto,Valor_Comparar); break;
	}
	
	
	// MOSTRAR AS COLUNAS
	while (!IsEmpty(F1))
	{
		dequeue(&F1,&Aux);
		
		if (Aux == ColCondicao)
			flag=1;
			
		Aux->patual = Aux->pdados;
		printf("/ %s /",Aux->NomeCampo);
		enqueue(&F2,Aux);
	}
	while (!IsEmpty(F2))
	{
		dequeue(&F2,&Aux);
		enqueue(&F1,Aux);
	}
	
	printf("\n\n");
	if (flag == 1)
	{
		while (!IsEmpty(F1))
		{		
				switch(ColCondicao->Tipo)
				{
					case 'N': if (ColCondicao->patual != NULL && ColCondicao->patual->Tipos.NUMERIC == valorfloat)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
										
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								break;
								
					case 'I': if (ColCondicao->patual != NULL && ColCondicao->patual->Tipos.INTEGER == valorint)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
											
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								break;
								
								
					case 'C': if (ColCondicao->patual != NULL && ColCondicao->patual->Tipos.CHARACTER_1 == valorchar)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
										
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								break;		
					case 'D': if (ColCondicao->patual != NULL && strcmp(ColCondicao->patual->Tipos.DATE,valordata) == 0)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
										
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								break;	
					case 'T': if (ColCondicao->patual != NULL && strcmp(ColCondicao->patual->Tipos.CHARACTER_20,valortexto) == 0)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
										
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								break;
				}	
			while (!IsEmpty(F2))
			{
				dequeue(&F2,&Aux);
				if (Aux != NULL && Aux->patual != NULL)
					enqueue(&F1,Aux);
			}
		}
	}
	else
	{
		ColCondicao->patual = ColCondicao->pdados;
		while (!IsEmpty(F1))
		{		

				switch(ColCondicao->Tipo)
				{
					case 'N': if (ColCondicao->patual != NULL && ColCondicao->patual->Tipos.NUMERIC == valorfloat)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
										
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								ColCondicao->patual = ColCondicao->patual->prox;
								break;
								
					case 'I': 	/*if (ColCondicao->patual == NULL)
									printf("EH NULO\n");
								else
									printf("INTEIRO: %d\n",ColCondicao->patual->Tipos.INTEGER);*/
								if (ColCondicao->patual != NULL && ColCondicao->patual->Tipos.INTEGER == valorint)
								{
									//printf("valorint: %d\n",valorint);
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
											
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								ColCondicao->patual = ColCondicao->patual->prox;
								break;
								
					case 'C': if (ColCondicao->patual != NULL && ColCondicao->patual->Tipos.CHARACTER_1 == valorchar)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
											
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								ColCondicao->patual = ColCondicao->patual->prox;
								break;
					
					case 'D': if (ColCondicao->patual != NULL && strcmp(ColCondicao->patual->Tipos.DATE, valordata) == 0)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
											
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								ColCondicao->patual = ColCondicao->patual->prox;
								break;
					
					case 'T': if (ColCondicao->patual != NULL && strcmp(ColCondicao->patual->Tipos.CHARACTER_20, valortexto) == 0)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
											
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								ColCondicao->patual = ColCondicao->patual->prox;
								break;
								
				}	
			while (!IsEmpty(F2))
			{
				dequeue(&F2,&Aux);
				if (Aux != NULL && Aux->patual != NULL)
					enqueue(&F1,Aux);
			}
		}
	}
}




void SELECTCOMWHERE(float primeiro,float segundo,PCampos *ColCondicao,PTabelas *Tabela,FILA *F1)
{
	FILA *F2; Init(&F2);
	PCampos *Aux;
	int flag=0;
	
	printf("\n\n");
	printf("### TABELA : %s ###\n\n", Tabela->TabelaNome);
	
	// MOSTRAR AS COLUNAS
	while (!IsEmpty(F1))
	{
		dequeue(&F1,&Aux);
		
		if (Aux == ColCondicao)
			flag=1;
			
		Aux->patual = Aux->pdados;
		printf("/ %s /",Aux->NomeCampo);
		enqueue(&F2,Aux);
	}
	while (!IsEmpty(F2))
	{
		dequeue(&F2,&Aux);
		enqueue(&F1,Aux);
	}
	
	printf("\n\n");
	if (flag == 1)
	{
		while (!IsEmpty(F1))
		{		
				switch(ColCondicao->Tipo)
				{
					case 'N': if (ColCondicao->patual != NULL && ColCondicao->patual->Tipos.NUMERIC >= primeiro && ColCondicao->patual->Tipos.NUMERIC <= segundo)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
										
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								break;
								
					case 'I': if (ColCondicao->patual != NULL && ColCondicao->patual->Tipos.INTEGER >= primeiro && ColCondicao->patual->Tipos.INTEGER <= segundo)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
											
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								break;
				}	
			while (!IsEmpty(F2))
			{
				dequeue(&F2,&Aux);
				if (Aux != NULL && Aux->patual != NULL)
					enqueue(&F1,Aux);
			}
		}
	}
	else
	{
		ColCondicao->patual = ColCondicao->pdados;
		while (!IsEmpty(F1))
		{		
				switch(ColCondicao->Tipo)
				{
					case 'N': if (ColCondicao->patual != NULL && ColCondicao->patual->Tipos.NUMERIC >= primeiro && ColCondicao->patual->Tipos.NUMERIC <= segundo)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
										
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								ColCondicao->patual = ColCondicao->patual->prox;
								break;
								
					case 'I': if (ColCondicao->patual != NULL && ColCondicao->patual->Tipos.INTEGER >= primeiro && ColCondicao->patual->Tipos.INTEGER <= segundo)
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										if (Aux != NULL && Aux->patual != NULL)
										{
											if (Aux->Tipo == 'I') 
												printf("%d\t",Aux->patual->Tipos.INTEGER);
											else
											if (Aux->Tipo == 'N')
												printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
											else
											if (Aux->Tipo == 'C')
												printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
											else
											if (Aux->Tipo == 'D')
												printf("%s\t",Aux->patual->Tipos.DATE);
											else // Tipo Text = T
												printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
											Aux->patual = Aux->patual->prox;
											
											if (Aux->patual != NULL)
												enqueue(&F2,Aux);
										}
									}
									printf("\n");
								}
								else
								{
									while (!IsEmpty(F1))
									{
										dequeue(&F1,&Aux);
										Aux->patual = Aux->patual->prox;
										if (Aux->patual != NULL)
											enqueue(&F2,Aux);
									}
								}
								ColCondicao->patual = ColCondicao->patual->prox;
								break;
				}	
			while (!IsEmpty(F2))
			{
				dequeue(&F2,&Aux);
				if (Aux != NULL && Aux->patual != NULL)
					enqueue(&F1,Aux);
			}
		}
	}
	
}	



void LeituraTerminal (PontDB **BANCODEDADOS)
{
	int i,j,z,y,value;
	char Comando[900];
	char Colunas[900], Valores[900], NomeTabela[900];
	textcolor(BLUE);
	printf("\n\n\n### INSIRA COMANDOS DML & DQL ###\n\n\n");
	textcolor(WHITE);
	fflush(stdin);
	gets(Comando);
	fflush(stdin);
	
	//INSERT INTO peca (id_peca, descricao, valor, estoque) VALUES (1, kit_embreagem, 499, 5);
	//INSERT INTO servico (id_servico, descricao, valor) VALUES (1, limpeza, 50);
	//INSERT INTO veiculo (id_veiculo, marca, modelo, dono, placa, combustivel, ano, chassi) VALUES (1, 'Volkswagen', 'Gol', 'Jose da Silva', 'ABC-1234', 'G', 2010, 'AXB1234T789');
	
	while (strcmp(Comando,"\0") != 0)
	{
		// INSERT
		
		if (Comando[0] == 'I')
		{
			i=12;
			j=0;
			while (Comando[i] != 32)
				NomeTabela[j++] = Comando[i++];
	
			NomeTabela[j] = '\0';
			
			i++;		
			
			// Pegar as colunas
			j=0;
			int qtdeColunas=1;
			while(Comando[i] != ')')
			{
				Colunas[j++] = Comando[++i];
				if(Comando[i] == ',')
				{
					qtdeColunas++;
				}
					
			}
				
			Colunas[j-1] = '\0';
			
			// Pegar os Valores
			j=0;
			i+=9;
			while(Comando[i] != ')')
				Valores[j++] = Comando[++i];
			Valores[j-1] = '\0';		
			
			Lista *L = NULL;
			int iColunas=0, iColuna, iValores, iValor;
			char Coluna[50], Valor[700];
			iColunas=iValores=0;
			i=0;
			while(i < qtdeColunas)
			{
				iColuna=0;
				Coluna[iColuna] = '\0';
				while(Colunas[iColunas] != ',')
					Coluna[iColuna++] = Colunas[iColunas++];
				Coluna[iColuna] = '\0';
				iColunas+=2;
				
				iValor=0;
				Valor[iValor] = '\0';
				while(Valores[iValores] != ',' && Valores[iValores] != ')')
					Valor[iValor++] = Valores[iValores++];
				Valor[iValor] = '\0';
				iValores+=2;
			
				Inserir(&L, Coluna, Valor);
				
				
				i++;
			}
			// INSERIR OS DADOS
			Lista *Aux;
			while (L != NULL)
			{
				Aux = Retirar(&L);
				//printf("%s / %s / NomeTabela: %s\n",Aux->Coluna,Aux->Value,NomeTabela);
				AddDadosNoBanco(&(*BANCODEDADOS),NomeTabela,Aux->Coluna,Aux->Value); 
				free(Aux);
			}
			
			
		}
		// SELECT *
		if (Comando[0] == 'S' && Comando[1] == 'E')
		{
			if(Comando[7] == '*')
			{
			
				//PEGA O NOME DA TABELA
				ListaGuardaTabelas *LT = NULL;
				char nomeDaTabela[30];
				int y = 14;
				z = 0;
				PTabelas *auxBuscaTabela;
				//SELECT * FROM peca, servico;
				//SELECT * FROM peca, servico WHERE d
				
				// PROCURA UM WHERE
				i = y;
				while (i < strlen(Comando)-3 && Comando[i] != 'W' && Comando[i+1] != 'H' && Comando[i+2] != 'E')
						i++;
				
				if (i == strlen(Comando)-3)
				{
					while(Comando[y] != ';')
					{
						z=0;
						while (Comando[y] != ',' && Comando[y] != ';' && Comando[y] != 32)
							nomeDaTabela[z++] = Comando[y++];
						nomeDaTabela[z] = '\0';
						
						
						if (Comando[y] == ',')
							y+=2;
						Inserir(&LT,nomeDaTabela);
						
						auxBuscaTabela = BuscaTabela(*BANCODEDADOS,nomeDaTabela);
						if(auxBuscaTabela != NULL)
							PercorreSemWhere(auxBuscaTabela);
					}
				}
				else // SELECT * FROM ordem_servico, veiculo, os_peca, peca WHERE ordem_servico.id_veiculo = veiculo.id_veiculo AND ordem_servico.id_os = os_peca.id_os AND os_peca.id_peca = peca.id_peca;
				{
					// PEGAR O NOME DA TABELA
					
					z=0;
					while (Comando[y] != 32)
						nomeDaTabela[z++] = Comando[y++];
					nomeDaTabela[z] = '\0';
				
					// PRA PULAR O " WHERE "
					i = y += 7;
					// SE É BETWEEN OU INNER JOIN
					while (i < strlen(Comando)-3 && Comando[i] != 'B' && Comando[i+1] != 'E' && Comando[i+2] != 'T')
						i++;
					
					// BETWEEN
					if (Comando[i] == 'B')
					{
						// COMANDO DE EXEMPLO
						//SELECT * FROM veiculo WHERE ano BETWEEN 1970 AND 2010; 
						//puts(nomeDaTabela);
						char ColunaCondicao[900], Valor1[900], Valor2[900];
						float V1,V2;
						z=0;
						while (Comando[y] != 32)
							ColunaCondicao[z++] = Comando[y++];
						ColunaCondicao[z] = '\0';
						
						//puts(ColunaCondicao);
						
						// PULAR O " BETWEEN "
						y+=9;
						
						z=0;
						while (Comando[y] != 32)
							Valor1[z++] = Comando[y++];
						Valor1[z] = '\0';
						
						//PULAR O " AND "
						y+=5;
						
						z=0;
						while (Comando[y] != 32 && Comando[y] != ';')
							Valor2[z++] = Comando[y++];
						Valor2[z] = '\0';
						
						V1 = atof(Valor1);
						V2 = atof(Valor2);
						
						//printf("%f / %f\n",V1,V2);
						
						FILA *F1;
						Init(&F1);
						
						PTabelas *TabelaEnd = BuscaTabela(*BANCODEDADOS,nomeDaTabela);
						PCampos *ColEnd;
						
						if (TabelaEnd != NULL)
						{
							ColEnd = BuscaCampo(TabelaEnd,ColunaCondicao);
							PCampos *AuxCampo = TabelaEnd->ptrcampos;
							
							// COLOCA NA FILA TODOS OS CAMPOS DA TABELA
							
							while(AuxCampo != NULL)
							{
								enqueue(&F1,AuxCampo);	
								AuxCampo = AuxCampo->prox;
							}
							SELECTCOMWHERE(V1,V2,ColEnd,TabelaEnd,F1);
						}
					}
					// 
					else
					{
						//SELECT COM WHERE DE IGUALDADE
						// SELECT * FROM peca WHERE|estoque = 10;
						
						char ColunaCondicao[900], Valor[900];
						z=0;
						
						while (Comando[y] != 32)
							ColunaCondicao[z++] = Comando[y++];
						ColunaCondicao[z] = '\0';
						
						y+=3;
						z=0;						
					
						while (Comando[y] != ';')
							Valor[z++] = Comando[y++];
						Valor[z] = '\0';
						
						PTabelas *TabelaEnd = BuscaTabela(*BANCODEDADOS,nomeDaTabela);
						PCampos *ColunaEnd = BuscaCampo(TabelaEnd,ColunaCondicao);
						
						FILA *F1; Init(&F1);
						PCampos *Auxiliar = TabelaEnd->ptrcampos;
						
						// COLOCAR OS CAMPOS NA FILA
						
						while (Auxiliar != NULL)
						{
							enqueue(&F1,Auxiliar);
							Auxiliar = Auxiliar->prox;
						}
						
						SELECT_WHERE_IGUALDADE(Valor,ColunaEnd,TabelaEnd,F1);
						
					}
					
				}
				
			}
			else
			{
				//SELECT coluna from table
				Lista *L = NULL, *Retorno;
				PCampos *Campo;
				char StringNula[5];
				StringNula[0] = '\0';
				FILA *F;
				Init(&F);
				y = 7;
				int flag = 0;
				while(flag == 0)
				{
					char enviaLista[30];
					int z = 0;
					//separa palavras para lista
					while(Comando[y] != ',' && Comando[y] != 32)
						enviaLista[z++] = Comando[y++];
					if (Comando[y] == ',')	
						y += 2;
					else 
						y += 1;	
					enviaLista[z] = '\0';
					
					if(stricmp(enviaLista,"FROM") != 0)
						Inserir(&L, enviaLista, StringNula);
					
					//achou o FROM;
					else 
						flag = 1;
				}
					char nomeTabela[30];
					int ab = 0;
					
					while(Comando[y] != 32 && Comando[y] != ';' && Comando[y] != ',')
						nomeTabela[ab++] = Comando[y++];
					
					nomeTabela[ab] = '\0';
					//SELECT SEM WHERE
					if(Comando[y] == ';')
					{
						PTabelas * Tabela = BuscaTabela(*BANCODEDADOS,nomeTabela);
						Lista *Retorno;
						Lista *Ret;
						// ESVASIAR A LISTA (COLUNAS) INSERINDO OS ENDERECOS DAS COLUNAS NA FILA
						while(!IsEmpty(L))
						{
							Retorno = Retirar(&L);
							Campo = BuscaCampo(Tabela,Retorno->Coluna);
							enqueue(&F,Campo);	
							free(Retorno);
						}	
						SELECTAlgumasColunas(Tabela, F);
					}
					else
					{
						//SELECT id_veiculo, modelo, placa, ano FROM veiculo|WHERE ano BETWEEN 1970 AND 2010;
						
						if(Comando[y] == ' ' && Comando[y+1] == 'W')// between aqui dentro
						{
							//PULAR O " WHERE "
							y+=7;
							
							char tab[30],col[30],num[10],tab2[30],col2[30]; 
							char ColCondicao[900];
							int l=0;
							while(Comando[y] != '.' && Comando[y] != 32)
								ColCondicao[l++] = Comando[y++];
							ColCondicao[l] = '\0';
						
							float V3,V4;
							
							if (Comando[y+1] == 'B' && Comando[y+2] == 'E')
							{
								PTabelas *TabelaEnd = BuscaTabela(*BANCODEDADOS,nomeTabela);
								PCampos *ColEnd = BuscaCampo(TabelaEnd,ColCondicao);
								
								y+= 9;
								l=0;
								while(Comando[y] != 32)
									num[l++] = Comando[y++];
								num[l] ='\0';
							
								V3 = atof(num);
								y+= 5;
								l=0;
								while(Comando[y] != ';' && Comando[y] != 32)
									num[l++] = Comando[y++];
								num[l] = '\0';
							
								V4 = atof(num);
								
								FILA *F1;
								Init(&F1);
								
								// PASSAR DA LISTA PARA A FILA
								while(!IsEmpty(L))
								{
									Retorno = Retirar(&L);
									Campo = BuscaCampo(TabelaEnd,Retorno->Coluna);
									enqueue(&F1,Campo);	
									free(Retorno);
								}
								
								// INVERTER A FILA
								Pilha *P1; Init(&P1);
								PCampos *Aux;
								while (!IsEmpty(F1))
								{
									dequeue(&F1,&Aux);
									Push(&P1,Aux);
								}
								while (!IsEmpty(P1))
								{
									Pop(&P1,&Aux);
									enqueue(&F1,Aux);
								}
								
								SELECTCOMWHERE(V3,V4,ColEnd,TabelaEnd,F1);
							}
							else
							{
								// SELECT COM WHERE DE IGUALDADE
								// SELECT id_peca, valor FROM peca WHERE estoque = 10;
								
								PTabelas *TabelaEnd = BuscaTabela(*BANCODEDADOS,nomeTabela);
								PCampos *ColEnd = BuscaCampo(TabelaEnd,ColCondicao);
								
								y+=3;
								
								char Valor[900];
								
								z=0;
								while (Comando[y] != ';')
									Valor[z++] = Comando[y++];
								Valor[z] = '\0';
								
								FILA *F1; Init(&F1);
								
								// PASSAR DA LISTA PARA A FILA
								while(!IsEmpty(L))
								{
									Retorno = Retirar(&L);
									Campo = BuscaCampo(TabelaEnd,Retorno->Coluna);
									puts(Campo->NomeCampo);
									enqueue(&F1,Campo);	
									free(Retorno);
								}
								
								// INVERTER A FILA
								Pilha *P1; Init(&P1);
								PCampos *Aux;
								while (!IsEmpty(F1))
								{
									dequeue(&F1,&Aux);
									Push(&P1,Aux);
								}
								while (!IsEmpty(P1))
								{
									Pop(&P1,&Aux);
									enqueue(&F1,Aux);
								}
								
								ColEnd->patual = ColEnd->pdados;
								
								SELECT_WHERE_IGUALDADE(Valor,ColEnd,TabelaEnd,F1);
							}

						}
						else
						{
							if(Comando[y] == ',')// mais de uma tabela logo nao Ã© between
							{
								ListaSelect *tabelasafterfrom = (ListaSelect*)malloc(sizeof(ListaSelect));//lista de tabelas que vem depois do FROM
								strcpy(tabelasafterfrom->coluna,nomeTabela);
								tabelasafterfrom->prox=NULL;
								ListaSelect *auxFROM = tabelasafterfrom->prox;
								char palavraenv[30]; palavraenv[0] = '\0';
								while(strcmp(palavraenv,"WHERE")!= 0)
								{
									if(Comando[y] == ',')
										y+= 2;
									else y ++;
									int k =0;
									while (Comando[y] != ',' && Comando[y] != ' ')
									{
										palavraenv[k++] = Comando[y++];
									}
									palavraenv[k] = '\0';
									if(strcmp(palavraenv,"WHERE")!= 0)// pergunto para nao entrar qualquer palavra
									{
										
										auxFROM = (ListaSelect*)malloc(sizeof(ListaSelect));
										strcpy(auxFROM->coluna,palavraenv);
										auxFROM->prox = NULL;
										auxFROM = auxFROM->prox;
									}
	
								}
								PontDB *copy;
								
								char p[40];
								strcpy(p,"where");
								CREATEDATABASE(&copy,p);
								strcpy(p,"tab");
								CREATETABLE(&copy,p);
								Lista *auxp = L;
								while(auxp != NULL) // rep para pegar os nomes das colunas
								{
								
									char nometab[40],nome[40],viracol[40];
									strcpy(nome,auxp->Coluna);
									int k=0,x =0;
									while(nome[k] != '.')
										nometab[x++] = nome[k++];
									nometab[x] = '\0';
									k++;
									x=0;
									while(nome[k] != '\0')
										viracol[x++] = nome[k++];
									viracol[x] = '\0';

									PCampos *paratipo = BuscaCampo(BuscaTabela((*BANCODEDADOS),nometab),viracol);
									strcpy(p,"tab");
									CREATECAMPO(copy,p,viracol,paratipo->Tipo);
										
									
									auxp = auxp->prox;
								}

								y++;//primeira letra da primeira palavra depois do where
								Lista *Comparacoes = NULL;
								while(Comando[y] != ';')
								{
									int x = 0; char um[500], dois[500];
									while(Comando[y] != ' ' && Comando[y] != ';')
										um[x++] = Comando[y++];
									um[x] = '\0';
									y+= 3;	
									x = 0;
									while(Comando[y] != ' ' && Comando[y] != ';')
										dois[x++] = Comando[y++];
									dois[x] = '\0';
									if(Comando[y] != ';')
										y+=5;
									puts(um);
									puts(dois);
									Inserir(&Comparacoes,um,dois);
									
								}
									
							}
	
						}
					}	
				
			}
			
		//INSERT INTO peca (id_peca, descricao, valor, estoque) values (1, 'Filtro Combustivel', 25.00, 7);
		//INSERT INTO peca (id_peca, descricao, valor, estoque) values (2, 'Vela Ignicao 4X', 96.00, 7);
		//INSERT INTO peca (id_peca, descricao, valor, estoque) values (3, 'Sensor Temperatura', 112.30, 1);

		// DELETE FROM peca WHERE estoque = 7;
		}
		if (Comando[0] == 'D')
		{
			// PULAR O "DELETE FROM " E PEGAR O NOME DA TABELA
			NomeTabela[0] = '\0';
			i = 12;
			j = 0;
			while (Comando[i] != 32 && Comando[i] != ';')
				NomeTabela[j++] = Comando[i++];
			NomeTabela[j] = '\0';
			
			// WHERE
			
			if (Comando[i+1] == 'W' && Comando[i+2] == 'H')
			{
				char CONDICAO;
				
				j = i += 7;
				
				z = 0;
				Colunas[0] = '\0';
				while (Comando[j] != 32)
					Colunas[z++] = Comando[j++];
				Colunas[z] = '\0';
				j++;
				CONDICAO = Comando[j];
				j+=2;
				z = 0;
				// PEGAR O VALOR
				while (Comando[j] != ';')
					Valores[z++] = Comando[j++];
				Valores[z] = '\0';
				
				
				PTabelas *Tabela = BuscaTabela(*BANCODEDADOS,NomeTabela);
				if (Tabela != NULL)
					DELETEFROM(Tabela, Colunas, Valores);
			}
			
		}
		// UPDATE
		if (Comando[0] == 'U')
		{
			// INSERT INTO peca (id_peca, descricao, valor, estoque) values (1, 'Filtro Combustivel', 25.00, 12);
			// INSERT INTO peca (id_peca, descricao, valor, estoque) values (2, 'Vela Ignicao 4X', 96.00, 7);
			// INSERT INTO peca (id_peca, descricao, valor, estoque) values (3, 'Embreagem', 450.00, 5);
			// UPDATE peca SET estoque = 6 WHERE id_peca = 2;
			// UPDATE peca SET descricao = 'Kit Embreagem' WHERE descricao = 'Embreagem';
			
			int valueset, valueproc;
			char CONDICAO;
			char NomeTabela[900], Coluna1[900], Coluna2[900], ValorSetar[900], ValorProcurar[900];
			i=0;
			// PULAR O "UPDATE "
			i+=7;
			y=0;
			NomeTabela[0] = '\0';
			while (Comando[i] != 32)
				NomeTabela[y++] = Comando[i++];
			NomeTabela[y] = '\0';
			// PULAR O " SET "
			i+=5;
			y=0;
			Coluna1[0] = '\0';
			while (Comando[i] != 32)
				Coluna1[y++] = Comando[i++];
			Coluna1[y] = '\0';
			i+=3;
			y=0;
			while (Comando[i] != 32)
				ValorSetar[y++] = Comando[i++];
			ValorSetar[y] = '\0';
			valueset = atoi(ValorSetar);
			
			i++;

			// IF DO WHERE
			if (Comando[i] == 'W')
			{
				i+=6;
				y=0;
				Coluna2[0] = '\0';
				while (Comando[i] != 32)
					Coluna2[y++] = Comando[i++];
				Coluna2[y] = '\0';
				
				i++;
				CONDICAO = Comando[i];
				i+=2;
				
				y=0;
				while (Comando[i] != ';')
					ValorProcurar[y++] = Comando[i++];
				ValorProcurar[y] = '\0';
				
				valueproc = atoi(ValorProcurar);
				
				//void UPDATE (PTabelas *Tabela, char Coluna_Condicao[], char Coluna_Alterada[], char Valor_Comparar[], char Valor_Alterar[])
				//printf("NT: %s / Col1: %s / Col2: %s / CONDICAO: %c / ValueSetar: %s / ValueProcurar: %s\n",NomeTabela,Coluna1,Coluna2,CONDICAO,ValorSetar,ValorProcurar);
				
				PTabelas *Tabela = BuscaTabela(*BANCODEDADOS, NomeTabela);
				if (Tabela != NULL)
					UPDATE (Tabela,Coluna2,Coluna1,ValorProcurar,ValorSetar);
			}
		}
		if (Comando[0] == 'K' || Comando[0] == 'k')
		{
			char tabela[900];
			gets(tabela);
			PTabelas *teste = BuscaTabela((*BANCODEDADOS), tabela);
			PercorreSemWhere(teste);
		}
		printf("\n\n");
		fflush(stdin);
		gets(Comando);
		
	}		
}

void ExibeConferir (PontDB *BANCODEDADOS)
{
	PTabelas *AuxT;
	PCampos *AuxC;
	
	
	AuxT = BANCODEDADOS->ptrtabelas;
	while (AuxT != NULL)
	{
		printf("\n\nTABELA: %s\n\n",AuxT->TabelaNome);
		AuxC = AuxT->ptrcampos;
		while (AuxC != NULL)
		{
			printf("END: %p / NC: %s - %c - %c - %p\n",AuxC,AuxC->NomeCampo,AuxC->Tipo,AuxC->PK,AuxC->FK);
			AuxC = AuxC->prox;
		}
		AuxT = AuxT->prox;
	}
}



void ExibeDados (PontDB *BANCODEDADOS)
{
	PTabelas *AuxT;
	PCampos *AuxC;
	PAtual *AuxP;
	
	AuxT = BANCODEDADOS->ptrtabelas;
	while (AuxT != NULL)
	{
		printf("\n\n### TABELA ### '%s'\n\n\n",AuxT->TabelaNome);
		AuxC = AuxT->ptrcampos;
		while (AuxC != NULL)
		{
			printf("\n# COLUNA # '%s' - %c - %c\n\n",AuxC->NomeCampo,AuxC->Tipo,AuxC->PK);
			AuxP = AuxC->pdados;
			while(AuxP != NULL)
			{
				if (AuxC->Tipo == 'I')
					printf("%d\n",AuxP->Tipos.INTEGER);
				else if (AuxC->Tipo == 'N')
					printf("%d\n",AuxP->Tipos.NUMERIC);
				else if (AuxC->Tipo == 'D')
					printf("%s\n",AuxP->Tipos.DATE);
				else if (AuxC->Tipo == 'C')
					printf("%c\n",AuxP->Tipos.CHARACTER_1);
				else
					printf("%s\n",AuxP->Tipos.CHARACTER_20);
				AuxP = AuxP->prox;
			}
			AuxC = AuxC->prox;
		}
		AuxT = AuxT->prox;
	}
}

void ExibeSelectTodascolunas (PTabelas *AuxT)
{
	
	PCampos *AuxC;
	PAtual *AuxP;
		printf("\n\nTABELA: %s\n",AuxT->TabelaNome);
		AuxC = AuxT->ptrcampos;
		while (AuxC != NULL)
		{
			printf("\nNC: %s - %c - %c",AuxC->NomeCampo,AuxC->Tipo,AuxC->PK);
			AuxP = AuxC->pdados;
			while(AuxP != NULL)
			{
				if (AuxC->Tipo == 'I')
					printf("\n\n%d\n\n",AuxP->Tipos.INTEGER);
				else if (AuxC->Tipo == 'N')
					printf("\n\n%.2f\n\n",AuxP->Tipos.NUMERIC);
				else if (AuxC->Tipo == 'D')
					printf("\n\n%s\n\n",AuxP->Tipos.DATE);
				else if (AuxC->Tipo == 'C')
					printf("\n\n%c\n\n",AuxP->Tipos.CHARACTER_1);
				else
					printf("\n\n%s\n\n",AuxP->Tipos.CHARACTER_20);
				AuxP = AuxP->prox;
			}
			AuxC = AuxC->prox;
		}
		printf("\n");
}


void ExibeSelectalgumascolunas (PTabelas *AuxT,char colunaprintf[])
{
	
			PCampos *AuxC = BuscaCampo(AuxT,colunaprintf);
			PAtual *AuxP = AuxC->pdados;
			while(AuxP != NULL)
			{
				if (AuxC->Tipo == 'I')
					printf("%d\n",AuxP->Tipos.INTEGER);
				else if (AuxC->Tipo == 'N')
					printf("%d\n",AuxP->Tipos.NUMERIC);
				else if (AuxC->Tipo == 'D')
					printf("%s\n",AuxP->Tipos.DATE);
				else if (AuxC->Tipo == 'C')
					printf("%c\n",AuxP->Tipos.CHARACTER_1);
				else
					printf("%s\n",AuxP->Tipos.CHARACTER_20);
				AuxP = AuxP->prox;
			}
			AuxC = AuxC->prox;
}

void DEVOLVE_F2_PARA_F1 (FILA **F1, FILA **F2)
{
	PCampos *Aux;
	while (!IsEmpty(*F2))
	{
		dequeue(&(*F2),&Aux);
		if (Aux->patual != NULL)
			enqueue(&(*F1),Aux);
	}
}


void DELETEFROM (PTabelas *Tabela, char Coluna_Estatica[], char value[]) // O SEGUNDO PARAMETRO SERIA O NOME DA COLUNA ONDE A GENTE IRIA ANALISAR A CONDICAO
{
	PCampos *Estatico, *Aux;
	PAtual *FREE;
	
	FILA *F1,*F2;
	Init(&F1);
	Init(&F2);
	
	int valorint;
	float valorfloat;
	char valorchar;
	char valordata[900], valortexto[900];
	
	Estatico = BuscaCampo(Tabela,Coluna_Estatica);
	
	switch(Estatico->Tipo)
	{
		case 'I': valorint = atoi(value); break;
		case 'N': valorfloat = atof(value); break;
		case 'D': strcpy(valordata,value); break;
		case 'C': valorchar = value[1]; break;
		case 'T': strcpy(valortexto,value); break;
	}
	
	// FILA 1 VAI ESTAR TODAS AS COLUNAS / FILA 2 VAI ARMAZENAR OS PONTEIROS QUE SAIREM DA FILA 1, E APÓS O CICLO, VOLTAR PARA A FILA 1
	
	Aux = Tabela->ptrcampos;
	while (Aux != NULL) // ENCHER A FILA COM OS ENDEREÇOS DAS COLUNAS
	{
		Aux->patual = Aux->pdados;
		if (Aux->patual != NULL)
			enqueue(&F1,Aux);
		Aux = Aux->prox;
	}
	
	
	while(!IsEmpty(F1))
	{
		
		switch(Estatico->Tipo)
		{
			case 'I': 
					if (Estatico->patual != NULL && Estatico->patual->Tipos.INTEGER == valorint)
					{
						// EXCLUI TUDO DO COMEÇO
						while(!IsEmpty(F1))
						{
							dequeue(&F1, &Aux);
							Aux->pdados = Aux->pdados->prox;
				  			free(Aux->patual);
				  	    	Aux->patual = Aux->pdados;
				  	    	enqueue(&F2, Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					if (Estatico->patual != NULL && Estatico->patual->prox != NULL && Estatico->patual->prox->Tipos.INTEGER == valorint)
					{
						// EXCLUI TUDO DO MEIO/FINAL
						while(!IsEmpty(F1))
						{
							dequeue(&F1, &Aux);
							
							FREE = Aux->patual->prox;
						    Aux->patual->prox = Aux->patual->prox->prox;
							free(FREE);
							
				  	    	enqueue(&F2, Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					else
					{
						// SÓ ANDA
						while(!IsEmpty(F1))
						{
							dequeue(&F1,&Aux);
							Aux->patual = Aux->patual->prox;
							enqueue(&F2,Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					
			break;
			case 'N': 
					if (Estatico->patual != NULL && Estatico->patual->Tipos.NUMERIC == valorfloat)
					{
						// EXCLUI TUDO DO COMEÇO
						while(!IsEmpty(F1))
						{
							dequeue(&F1, &Aux);
							Aux->pdados = Aux->pdados->prox;
				  			free(Aux->patual);
				  	    	Aux->patual = Aux->pdados;
				  	    	enqueue(&F2, Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					if (Estatico->patual != NULL && Estatico->patual->prox != NULL && Estatico->patual->prox->Tipos.NUMERIC == valorfloat)
					{
						// EXCLUI TUDO DO MEIO/FINAL
						while(!IsEmpty(F1))
						{
							dequeue(&F1, &Aux);
							
							FREE = Aux->patual->prox;
						    Aux->patual->prox = Aux->patual->prox->prox;
							free(FREE);
							
				  	    	enqueue(&F2, Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					else
					{
						// SÓ ANDA
						while(!IsEmpty(F1))
						{
							dequeue(&F1,&Aux);
							Aux->patual = Aux->patual->prox;
							enqueue(&F2,Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}		
			break;
			case 'D':
					if (Estatico->patual != NULL && strcmp(Estatico->patual->Tipos.DATE, valordata) == 0)
					{
						// EXCLUI TUDO DO COMEÇO
						while(!IsEmpty(F1))
						{
							dequeue(&F1, &Aux);
							Aux->pdados = Aux->pdados->prox;
				  			free(Aux->patual);
				  	    	Aux->patual = Aux->pdados;
				  	    	enqueue(&F2, Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					if (Estatico->patual != NULL && Estatico->patual->prox != NULL && strcmp(Estatico->patual->prox->Tipos.DATE, valordata) == 0)
					{
						// EXCLUI TUDO DO MEIO/FINAL
						while(!IsEmpty(F1))
						{
							dequeue(&F1, &Aux);
							
							FREE = Aux->patual->prox;
						    Aux->patual->prox = Aux->patual->prox->prox;
							free(FREE);
							
				  	    	enqueue(&F2, Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					else
					{
						// SÓ ANDA
						while(!IsEmpty(F1))
						{
							dequeue(&F1,&Aux);
							Aux->patual = Aux->patual->prox;
							enqueue(&F2,Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
			break;
			case 'C': 
				    if (Estatico->patual != NULL && Estatico->patual->Tipos.CHARACTER_1 == valorchar)
					{
						// EXCLUI TUDO DO COMEÇO
						while(!IsEmpty(F1))
						{
							dequeue(&F1, &Aux);
							Aux->pdados = Aux->pdados->prox;
				  			free(Aux->patual);
				  	    	Aux->patual = Aux->pdados;
				  	    	enqueue(&F2, Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					if (Estatico->patual != NULL && Estatico->patual->prox != NULL && Estatico->patual->prox->Tipos.CHARACTER_1 == valorchar)
					{
						// EXCLUI TUDO DO MEIO/FINAL
						while(!IsEmpty(F1))
						{
							dequeue(&F1, &Aux);
							
							FREE = Aux->patual->prox;
						    Aux->patual->prox = Aux->patual->prox->prox;
							free(FREE);
							
				  	    	enqueue(&F2, Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					else
					{
						// SÓ ANDA
						while(!IsEmpty(F1))
						{
							dequeue(&F1,&Aux);
							Aux->patual = Aux->patual->prox;
							enqueue(&F2,Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					
					
			break;
			case 'T':
					//puts(valortexto);
					//puts(Estatico->patual->Tipos.CHARACTER_20);
					if (Estatico->patual != NULL && strcmp(Estatico->patual->Tipos.CHARACTER_20, valortexto) == 0)
					{
						//puts(valortexto);
						//puts(Estatico->patual->Tipos.CHARACTER_20);
						// EXCLUI TUDO DO COMEÇO
						while(!IsEmpty(F1))
						{
							dequeue(&F1, &Aux);
							Aux->pdados = Aux->pdados->prox;
				  			free(Aux->patual);
				  	    	Aux->patual = Aux->pdados;
				  	    	enqueue(&F2, Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					if (Estatico->patual != NULL && Estatico->patual->prox != NULL && strcmp(Estatico->patual->prox->Tipos.CHARACTER_20, valortexto) == 0)
					{
						//puts(valortexto);
						//puts(Estatico->patual->Tipos.CHARACTER_20);
						// EXCLUI TUDO DO MEIO/FINAL
						while(!IsEmpty(F1))
						{
							dequeue(&F1, &Aux);
							
							FREE = Aux->patual->prox;
						    Aux->patual->prox = Aux->patual->prox->prox;
							free(FREE);
							
				  	    	enqueue(&F2, Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					else
					{
						// SÓ ANDA
						while(!IsEmpty(F1))
						{
							dequeue(&F1,&Aux);
							Aux->patual = Aux->patual->prox;
							enqueue(&F2,Aux);
						}
						DEVOLVE_F2_PARA_F1(&F1,&F2);
					}
					
			break;
		}
	}
}





void PercorreSemWhere (PTabelas *Tabela) // O SEGUNDO PARAMETRO SERIA O NOME DA COLUNA ONDE A GENTE IRIA ANALISAR A CONDICAO
{
	PCampos *Estatico, *Aux;
	// No parametro se precisar - char Exemplo_Coluna[]
	FILA *F1,*F2;
	Init(&F1);
	Init(&F2);
	// FILA 1 VAI ESTAR TODAS AS COLUNAS / FILA 2 VAI ARMAZENAR OS PONTEIROS QUE SAIREM DA FILA 1, E APï¿½S O CICLO, VOLTAR PARA A FILA 1
	
	//Estatico = BuscaCampo(Tabela,Exemplo_Coluna);

	//if (Estatico != NULL)
	printf("\n\n");
	printf("### TABELA : %s ###\n\n", Tabela->TabelaNome);
	Aux = Tabela->ptrcampos;
	while (Aux != NULL) // ENCHER A FILA COM OS ENDEREï¿½OS DAS COLUNAS
	{
		Aux->patual = Aux->pdados;
		printf("/ %s /",Aux->NomeCampo);
		enqueue(&F1,Aux);
		Aux = Aux->prox;
	}
	printf("\n\n");
	do 
	{		
		while (!IsEmpty(F1))
		{	
			dequeue(&F1,&Aux);
			if (Aux != NULL && Aux->patual != NULL)
			{
				if (Aux->Tipo == 'I') 
					printf("%d\t",Aux->patual->Tipos.INTEGER);
				else
				if (Aux->Tipo == 'N')
					printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
				else
				if (Aux->Tipo == 'C')
					printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
				else
				if (Aux->Tipo == 'D')
					printf("%s\t",Aux->patual->Tipos.DATE);
				else // Tipo Text = T
					printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
				Aux->patual = Aux->patual->prox;
			
				enqueue(&F2,Aux);
			}
		}	
		while (!IsEmpty(F2))
		{
			dequeue(&F2,&Aux);
			enqueue(&F1,Aux);
		}
		printf("\n");
	}while (!IsEmpty(F1));

}


void SELECTAlgumasColunas (PTabelas *Tabela, FILA *F1)
{
	Pilha *P1;
	Init(&P1);

	FILA *F2; Init(&F2);
	PCampos *Aux;
	
	printf("\n\n");
	printf("### TABELA : %s ###\n\n", Tabela->TabelaNome);
	printf("\n\n");
	
	// INVERTER A FILA
	while (!IsEmpty(F1))
	{
		dequeue(&F1,&Aux);
		Push(&P1,Aux);
	}
	while (!IsEmpty(P1))
	{
		Pop(&P1,&Aux);
		enqueue(&F1,Aux);
	}
	
	// MOSTRAR AS COLUNAS
	while (!IsEmpty(F1))
	{
		dequeue(&F1,&Aux);
		Aux->patual = Aux->pdados;
		printf("/ %s /",Aux->NomeCampo);
		enqueue(&F2,Aux);
	}
	while (!IsEmpty(F2))
	{
		dequeue(&F2,&Aux);
		enqueue(&F1,Aux);
	}
	printf("\n\n");
	do 
	{		
		while (!IsEmpty(F1))
		{	
			dequeue(&F1,&Aux);
			if (Aux != NULL && Aux->patual != NULL)
			{
				if (Aux->Tipo == 'I') 
					printf("%d\t",Aux->patual->Tipos.INTEGER);
				else
				if (Aux->Tipo == 'N')
					printf("%.2f\t",Aux->patual->Tipos.NUMERIC);
				else
				if (Aux->Tipo == 'C')
					printf("%c\t",Aux->patual->Tipos.CHARACTER_1);
				else
				if (Aux->Tipo == 'D')
					printf("%s\t",Aux->patual->Tipos.DATE);
				else // Tipo Text = T
					printf("%s\t",Aux->patual->Tipos.CHARACTER_20);
				Aux->patual = Aux->patual->prox;
			
				enqueue(&F2,Aux);
			}
		}	
		while (!IsEmpty(F2))
		{
			dequeue(&F2,&Aux);
			enqueue(&F1,Aux);
		}
		printf("\n");
	}while (!IsEmpty(F1));
}



void UPDATE (PTabelas *Tabela, char Coluna_Condicao[], char Coluna_Alterada[], char Valor_Comparar[], char Valor_Alterar[])
{
	PCampos *Aux, *ColCondicao, *ColAlterada;
	
	ColCondicao = BuscaCampo(Tabela,Coluna_Condicao);
	ColAlterada = BuscaCampo(Tabela,Coluna_Alterada);
	
	int CONDint;
	float CONDfloat;
	char CONDchar;
	char CONDdata[900], CONDtexto[900];
	
	int ALTint;
	float ALTfloat;
	char ALTchar;
	char ALTdata[900], ALTtexto[900];
	
	switch(ColCondicao->Tipo)
	{
		case 'I': CONDint = atoi(Valor_Comparar); break;
		case 'N': CONDfloat = atof(Valor_Comparar); break;
		case 'D': strcpy(CONDdata,Valor_Comparar); break;
		case 'C': CONDchar = Valor_Comparar[1]; break;
		case 'T': strcpy(CONDtexto,Valor_Comparar); break;
	}
	
	switch(ColAlterada->Tipo)
	{
		case 'I': ALTint = atoi(Valor_Alterar); break;
		case 'N': ALTfloat = atof(Valor_Alterar); break;
		case 'D': strcpy(ALTdata,Valor_Alterar); break;
		case 'C': ALTchar = Valor_Alterar[1]; break;
		case 'T': strcpy(ALTtexto,Valor_Alterar); break;
	}
	
	ColCondicao->patual = ColCondicao->pdados;
	ColAlterada->patual = ColAlterada->pdados;
	
	if (ColCondicao != ColAlterada)
	{
		while (ColCondicao->patual != NULL)
		{
			switch(ColCondicao->Tipo)
			{
				case 'I': if (ColCondicao->patual->Tipos.INTEGER == CONDint) 
						  {
						  	switch(ColAlterada->Tipo)
						  	{
						  		case 'I': ColAlterada->patual->Tipos.INTEGER = ALTint; break;
								case 'N': ColAlterada->patual->Tipos.NUMERIC = ALTfloat; break;
								case 'D': strcpy(ColAlterada->patual->Tipos.DATE,ALTdata); break;
								case 'C': ColAlterada->patual->Tipos.CHARACTER_1 = ALTchar; break;
								case 'T': strcpy(ColAlterada->patual->Tipos.CHARACTER_20,ALTtexto); break;
						  	}
						  }
						  ColCondicao->patual = ColCondicao->patual->prox;
						  ColAlterada->patual = ColAlterada->patual->prox;
						  break;
						  
				case 'N': if (ColCondicao->patual->Tipos.NUMERIC == CONDfloat)
						  {
						  	switch(ColAlterada->Tipo)
						  	{
						  		case 'I': ColAlterada->patual->Tipos.INTEGER = ALTint; break;
								case 'N': ColAlterada->patual->Tipos.NUMERIC = ALTfloat; break;
								case 'D': strcpy(ColAlterada->patual->Tipos.DATE,ALTdata); break;
								case 'C': ColAlterada->patual->Tipos.CHARACTER_1 = ALTchar; break;
								case 'T': strcpy(ColAlterada->patual->Tipos.CHARACTER_20,ALTtexto); break;
						  	}
						  }
						  ColCondicao->patual = ColCondicao->patual->prox;
						  ColAlterada->patual = ColAlterada->patual->prox;
						  break;
						  
				case 'D': if (strcmp(ColCondicao->patual->Tipos.DATE,CONDdata) == 0) 
						  {	
						  	switch(ColAlterada->Tipo)
						  	{
						  		case 'I': ColAlterada->patual->Tipos.INTEGER = ALTint; break;
								case 'N': ColAlterada->patual->Tipos.NUMERIC = ALTfloat; break;
								case 'D': strcpy(ColAlterada->patual->Tipos.DATE,ALTdata); break;
								case 'C': ColAlterada->patual->Tipos.CHARACTER_1 = ALTchar; break;
								case 'T': strcpy(ColAlterada->patual->Tipos.CHARACTER_20,ALTtexto); break;
						  	}
						  }
						  ColCondicao->patual = ColCondicao->patual->prox;
						  ColAlterada->patual = ColAlterada->patual->prox;
						  break;
						  
				case 'C': if (ColCondicao->patual->Tipos.CHARACTER_1 == CONDchar)
						  {
							switch(ColAlterada->Tipo)
						  	{
						  		case 'I': ColAlterada->patual->Tipos.INTEGER = ALTint; break;
								case 'N': ColAlterada->patual->Tipos.NUMERIC = ALTfloat; break;
								case 'D': strcpy(ColAlterada->patual->Tipos.DATE,ALTdata); break;
								case 'C': ColAlterada->patual->Tipos.CHARACTER_1 = ALTchar; break;
								case 'T': strcpy(ColAlterada->patual->Tipos.CHARACTER_20,ALTtexto); break;
						  	}	
						  } 
						  ColCondicao->patual = ColCondicao->patual->prox;
						  ColAlterada->patual = ColAlterada->patual->prox;
				    	  break;
				    	  
				case 'T': if (strcmp(ColCondicao->patual->Tipos.CHARACTER_20,CONDtexto) == 0)
						  {
						  	switch(ColAlterada->Tipo)
						  	{
						  		case 'I': ColAlterada->patual->Tipos.INTEGER = ALTint; break;
								case 'N': ColAlterada->patual->Tipos.NUMERIC = ALTfloat; break;
								case 'D': strcpy(ColAlterada->patual->Tipos.DATE,ALTdata); break;
								case 'C': ColAlterada->patual->Tipos.CHARACTER_1 = ALTchar; break;
								case 'T': strcpy(ColAlterada->patual->Tipos.CHARACTER_20,ALTtexto); break;
						  	}
						  }
						  ColCondicao->patual = ColCondicao->patual->prox;
						  ColAlterada->patual = ColAlterada->patual->prox;
						  break;
						  
			}
		}
	}
	else // AS COLUNAS SAO IGUAIS
	{
		while (ColCondicao->patual != NULL)
		{
			switch(ColCondicao->Tipo)
			{
				case 'I': if (ColCondicao->patual->Tipos.INTEGER == CONDint)
								ColCondicao->patual->Tipos.INTEGER = ALTint;
				 		  break;
				case 'N': if (ColCondicao->patual->Tipos.NUMERIC == CONDfloat)
								ColCondicao->patual->Tipos.NUMERIC = ALTfloat; 
						  break;
				case 'D': if (strcmp(ColCondicao->patual->Tipos.DATE,CONDdata) == 0)
							strcpy(ColCondicao->patual->Tipos.DATE,ALTdata); 
						  break;
				case 'C': if (ColCondicao->patual->Tipos.CHARACTER_1 == CONDchar)
							ColCondicao->patual->Tipos.CHARACTER_1 = ALTchar; 
						  break;
				case 'T': if (strcmp(ColCondicao->patual->Tipos.CHARACTER_20,CONDtexto) == 0)
							strcpy(ColCondicao->patual->Tipos.CHARACTER_20,ALTtexto);
						  break;
			}
			ColCondicao->patual = ColCondicao->patual->prox;
		}
	}
	
}





int main ()
{
	textcolor(WHITE);
	
	
	
	PontDB *BANCODEDADOS = NULL;
	
	LeituraScript(&BANCODEDADOS);
	
	ExibeConferir(BANCODEDADOS);
	
	LeituraTerminal(&BANCODEDADOS);
	
	//ExibeDados(BANCODEDADOS);
	
	printf("\n\n\nEXECUCAO TERMINADA\n");
	return 0;
}

