#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TKFim -1
#define TKErro -2
#define TKId 1
#define TKWhile 2
#define TKFor 3
#define TKSwitch 4
#define TKCase 5
#define TKOtherwise 6
#define TKEnd 7
#define TKIf 8
#define TKElse 9
#define TKElseIf 10
#define TKAtrib 11
#define TKSoma 12
#define TKAbrePar 13
#define TKFechaPar 14
#define TKAbreChave 15
#define TKFechaChave 16
#define TKVirgula 17
#define TKPontoeVirg 18
#define TKString 19
#define TKAbreColchete 20
#define TKFechaColchete 21
#define TKSubtracao 22
#define TKMultiplicacao 23
#define TKPotencia 24
#define TKDivisao 25
#define TKComentario 26
#define TKConstanteInteira 27
#define TKIgual 28
#define TKDiferente 29
#define TKMaior 30
#define TKMaiorIgual 31
#define TKMenor 32
#define TKMenorIgual 33
#define TKEBinario 34
#define TKELogico 35
#define TKOuLogico 36
#define TKOuBinario 37
#define TKConstanteReal 38
#define TKFunction 39
#define TKBreak 40
#define TKParfor 41
#define TKTry 42
#define TKCatch 43
#define TKContinue 44
#define TKReturn 45
#define TKTrue 46
#define TKFalse 47
#define TKDoisPontos 48
#define TKNegacao 49

int pos = 0;

struct pal_res{
	char palavra[20];
	int tk;
};

struct pal_res lista_pal[] =
{
	{ "while", TKWhile },
	{ "for", TKFor },
	{ "switch", TKSwitch },
	{ "case", TKCase },
	{ "otherwise", TKOtherwise },
	{ "end", TKEnd },
	{ "if", TKIf },
	{ "else", TKElse },
	{ "elseif", TKElseIf },
	{ "function", TKFunction },
	{ "break", TKBreak },
	{ "parfor", TKParfor },
	{ "try", TKTry },
	{ "catch", TKCatch },
	{ "continue", TKContinue },
	{ "return", TKReturn },
	{ "true", TKTrue },
	{ "false", TKFalse },
	{ "fimtabela", TKId }
};

int tk;
int linha = 1, coluna = 0;
char* characters;
FILE * newFile;

int rec_equ(char st[], char lex[], int * linha, int * coluna)
{
	int estado = 0,
		fim = 0,
		posl = 0,
		palavra;

	while (!fim)
	{
		char c = st[pos];
		lex[posl++] = c;
		switch (estado)
		{
		case 0:
			(*coluna) += 1;
			if (c == '\0')
			{
				return TKFim;
			}
			pos++;
			if (c == ' ' || c == '\t')
			{
				posl--;
				break;
			}
			if (c == '\n')
			{
				posl--;
				(*linha) += 1;
				(*coluna) = 0;
				break;
			}
			if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
			{
				estado = 1;
				break;
			}
			if (c >= '0' && c <= '9') { estado = 7; break; }
			if (c == '.') { estado = 14; break; }
			if (c == '\'') { estado = 2; break; }
			if (c == '%') { estado = 3; break; }
			if (c == '=') { estado = 8; break; }
			if (c == '~') { estado = 9; break; }
			if (c == '>') { estado = 10; break; }
			if (c == '<') { estado = 11; break; }
			if (c == '&') { estado = 12; break; }
			if (c == '|') { estado = 13; break; }

			lex[posl] = '\0';
			if (c == '+') { return TKSoma; }
			if (c == '-') { return TKSubtracao; }
			if (c == '*') { return TKMultiplicacao; }
			if (c == '^') { return TKPotencia; }
			if (c == '/') { return TKDivisao; }
			if (c == '(') { return TKAbrePar; }
			if (c == ')') { return TKFechaPar; }
			if (c == '{') { return TKAbreChave; }
			if (c == '}') { return TKFechaChave; }
			if (c == '[') { return TKAbreColchete; }
			if (c == ']') { return TKFechaColchete; }
			if (c == ',') { return TKVirgula; }
			if (c == ';') { return TKPontoeVirg; }
			if (c == ':') { return TKDoisPontos; }
			if (c == '_') { return TKErro; }
			break;
		case 1:
			if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
			{
				pos++;
				break;
			}
			lex[--posl] = '\0';
			palavra = palavra_reservada(lex);
			return palavra;
		case 2:
			pos++;
			if (c == '\n')
			{
				return TKErro;
			}
			if (c != '\'')
			{
				break;
			}
			lex[posl] = '\0';
			return TKString;
		case 3:
			if (c == '{')
			{
				estado = 4;
			}
			else if (c == '\n')
			{
				lex[--posl] = '\0';
				return TKComentario;
			}
			else
			{
				estado = 6;
			}
			pos++;
			break;
		case 4:
			if (c == '%')
			{
				estado = 5;
			}
			pos++;
			break;
		case 5:
			pos++;
			if (c == '%')
			{
				break;
			}
			else if (c != '}')
			{
				estado = 4;
				break;
			}
			lex[posl] = '\0';
			return TKComentario;
		case 6:
			if (c != '\n')
			{
				pos++;
				break;
			}
			lex[--posl] = '\0';
			return TKComentario;
		case 7:
			if (c >= '0' && c <= '9')
			{
				pos++;
				break;
			}
			else if (c == '.')
			{
				pos++;
				estado = 14;
				break;
			}
			else if (c == 'e')
			{
				pos++;
				estado = 15;
				break;
			}
			lex[--posl] = '\0';
			return TKConstanteInteira;
		case 14:
			if (c >= '0' && c <= '9')
			{
				pos++;
				break;
			}
			if (c == 'e')
			{
				pos++;
				estado = 15;
				break;
			}
			if (c == '.')
			{
				pos++;
				return TKErro;
			}
			lex[--posl] = '\0';
			return TKConstanteReal;
		case 15:
			if (c >= '0' && c <= '9')
			{
				pos++;
				break;
			}
			if (c == '.' || c == 'e')
			{
				pos++;
				return TKErro;
			}
			lex[--posl] = '\0';
			return TKConstanteReal;
		case 8:
			if (c == '=')
			{
				pos++;
				lex[posl] = '\0';
				return TKIgual;
			}
			else
			{
				lex[--posl] = '\0';
				return TKAtrib;
			}
			break;
		case 9:
			if (c == '=')
			{
				pos++;
				lex[posl] = '\0';
				return TKDiferente;
			}
			else
			{
				return TKNegacao;
			}
			break;
		case 10:
			if (c == '=')
			{
				pos++;
				lex[posl] = '\0';
				return TKMaiorIgual;
			}
			else
			{
				lex[--posl] = '\0';
				return TKMaior;
			}
			break;
		case 11:
			if (c == '=')
			{
				pos++;
				lex[posl] = '\0';
				return TKMenorIgual;
			}
			else
			{
				lex[--posl] = '\0';
				return TKMenor;
			}
			break;
		case 12:
			if (c == '&')
			{
				pos++;
				lex[posl] = '\0';
				return TKELogico;
			}
			else
			{
				lex[--posl] = '\0';
				return TKEBinario;
			}
			break;
		case 13:
			if (c == '|')
			{
				pos++;
				lex[posl] = '\0';
				return TKOuLogico;
			}
			else
			{
				lex[--posl] = '\0';
				return TKOuBinario;
			}
		}
	}
}

void leToken()
{
	char lex[200];
	while ((tk = rec_equ(characters, lex, &linha, &coluna)) != TKFim)
	{
		if (tk == TKErro)
		{
			printf("Ocorreu um erro lexico!\n");
			return;
		}

		printf("Token: %d\t Linha: %d\t Coluna: %d\tLex: %s \n", tk, linha, coluna, lex);
		fprintf(newFile, "Token: %d\t Linha: %d\t Coluna: %d\tLex: %s \n", tk, linha, coluna, lex);
		coluna += strlen(lex) - 1;
		if (lex[strlen(lex) - 1] == '\n')
			linha++;
	}
}

int EXPFIM()
{
	if (tk == TKAbrePar)
	{
		leToken();
		if (EXP1())
		{
			if (tk == TKFechaPar)
			{
				leToken();
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else if (id())
	{
		return 1;
	}
	else if (cte())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int EXP15()
{
	if (tk == TKSubtracao)
	{
		leToken();
		if (EXPFIM())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (EXPFIM())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int EXP14()
{
	if (tk == TKPotencia)
	{
		leToken();
		if (EXP15())
		{
			EXP14();
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int EXP13()
{
	if (EXP15())
	{
		EXP14();
		return 1;
	}
	else
	{
		return 0;
	}
}

int EXP12()
{
	if (tk == TKEBinario)
	{
		leToken();
		if (EXP13())
		{
			EXP12();
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int EXP11()
{
	if (EXP13())
	{
		EXP12();
		return 1;
	}
	else
	{
		return 0;
	}
}

int EXP10()
{
	if (tk == TKDivisao)
	{
		leToken();
		if (EXP11())
		{
			EXP10();
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int EXP9()
{
	if (EXP11())
	{
		EXP10();
		return 1;
	}
	else
	{
		return 0;
	}
}

int EXP8() 
{
	if (tk == TKMultiplicacao)
	{
		leToken();
		if (EXP9())
		{
			EXP8();
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int EXP7()
{
	if (EXP9())
	{
		EXP8();
		return 1;
	}
	else
	{
		return 0;
	}
}

int EXP6()
{
	if (tk == TKOuBinario)
	{
		leToken();
		if (EXP7())
		{
			EXP6();
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int EXP5()
{
	if (EXP7())
	{
		EXP6();
		return 1;
	}
	else
	{
		return 0;
	}
}

int EXP4()
{
	if (tk == TKSubtracao)
	{
		leToken();
		if (EXP5())
		{
			EXP4();
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int EXP3()
{
	if (EXP5())
	{
		EXP4();
		return 1;
	}
	else
	{
		return 0;
	}
}

int EXP2()
{
	if (tk == TKSoma)
	{
		leToken();
		if (EXP3())
		{
			EXP2();
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int EXP1()
{
	if (EXP2())
	{
		EXP1();
		return 1;
	}
	else
	{
		return 0;
	}
}

int EXP0()
{
	if (COMP0())
	{
		return 1;
	}
	else if (EXP1())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int COMP5()
{
	if (tk == TKAbrePar)
	{
		leToken();
		if (COMP0())
		{
			if (tk == TKFechaPar)
			{
				leToken();
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else if (EXP1())
	{
		if (tk == TKMaior || tk == TKMaiorIgual || tk == TKMenor || tk == TKMenorIgual || 
			tk == TKIgual || tk == TKDiferente)
		{
			leToken();
			if (EXP1())
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int COMP4()
{
	if (tk == TKNegacao)
	{
		leToken();
		if (COMP5())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (COMP5())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int COMP3()
{
	if (tk == TKELogico)
	{
		leToken();
		if (COMP4())
		{
			COMP3();
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int COMP2()
{
	if (COMP4())
	{
		COMP3();
		return 1;
	}
	else
	{
		return 0;
	}
}

int COMP1()
{
	if (tk == TKOuLogico)
	{
		leToken();
		if (COMP2())
		{
			COMP1();
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int COMP0()
{
	if (COMP2())
	{
		COMP1();
		return 1;
	}
	else
	{
		return 0;
	}
}

int	ELSE()
{
	if (tk == TKElse)
	{
		leToken();
		if (COMANDO())
		{
			return 1;
		}
	}
	else if (tk == TKElseIf)
	{
		leToken();
		if (tk == TKAbrePar)
		{
			leToken();
			if (COMP0())
			{
				if (COMANDO())
				{
					if (ELSE())
					{
						return 1;
					}
					else { return 1; }
				}
				else { return 0; }
			}
			else { return 0; }
		}
		else { return 0; }
	}
	else { return 0; }
}

int IF()
{
	if (tk == TKIf) // TKIf
	{
		leToken();
		if (tk == TKAbrePar)
		{
			leToken();
			if (COMP0())
			{
				if (tk == TKFechaPar)
				{
					leToken();
					if (COMANDO())
					{
						if (ELSE())
						{
							if (tk == TKEnd)
							{
								leToken();
								return 1;
							}
							else { return 0; }
						}
						else { return 0; }
					}
					else { return 0; }
				}
				else { return 0; }
			}
			else { return 0; }
		}
		else { return 0; }
	}
	else { return 0; }
}

//????   como faze essa porra
int COMENTARIO()
{
	if ("%")
	{
		leToken();
		if (string())
		{
			if ('\n')
			{
				leToken();
				return 1;
			}
			else { return 0; }
		}
		else if (tk == TKAbreChave)
		{
			leToken();
			if (string())
			{
				if (tk == TKFechaChave)
				{
					leToken();
					if ("%")
					{
						leToken();
						return 1;
					}
					else { return 0; }
				}
				else { return 0; }
			}
			else { return 0; }
		}
		else { return 0; }
	}
	else { return 0; }
	return 0;
}

int TRY()
{
	if (tk == TKTry) // TKTry
	{
		leToken();
		if (COMANDO())
		{
			if (tk == TKCatch)
			{
				leToken();
				if (COMANDO())
				{
					if (tk == TKEnd) // TKEnd
					{
						leToken();
						return 1;
					}
					else { return 0; }
				}
				else { return 0; }
			}
			else { return 0; }
		}
		else { return 0; }
	}
	else { return 0; }
}

int CRIAFUNCTION()
{
	if (tk == TKFunction) // TKFunction
	{
		leToken();
		if (FUNCTION())
		{
			if (COMANDO())
			{
				if (tk == TKEnd)
				{
					leToken();
					return 1;
				}
				else { return 0; }
			}
			else { return 0; }
		}
		else { return 0; }
	}
	else { return 0; }
}

int FUNCTION()
{
	if (id())
	{
		if (tk == TKAbrePar)
		{
			leToken();
			if (PARAM0())
			{
				if (tk == TKFechaPar)
				{
					leToken();
					return 1;
				}
				else { return 0; }
			}
			else { return 0; }
		}
		else { return 0; }
	}
	else { return 0; }
}

int PARFOR()
{
	if (ATRIB())
	{
		if (tk == TKDoisPontos) // 
		{
			leToken();
			if (VAL())
			{
				if (tk == TKPontoeVirg) // TKPontoEVirgula
				{
					leToken();
					if (COMANDO())
					{
						if (tk == TKPontoeVirg)
						{
							leToken();
							if (tk == TKEnd) // TKEnd
							{
								return 1;
							}
							else { return 0; }
						}
						else { return 0; }
					}
					else { return 0; }
				}
				else { return 0; }
			}
			else { return 0; }
		}
		else { return 0; }
	}
	else { return 0; }
}

int CASE()
{
	if (tk == TKCase) // TKCase
	{
		leToken();
		if (cte())
		{
			if (COMANDO())
			{
				if (CASE())
				{
					return 1;
				}
				else
				{
					return 1;
				}
			}
		}
	}
	else if (tk == TKOtherwise) // TKOtherwise
	{
		leToken();
		if (COMANDO())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int SWITCH()
{
	if (tk == TKSwitch) // TKSwitch
	{
		leToken();
		if (id())
		{
			if (CASE())
			{
				if (tk == TKEnd) // TKEnd
				{
					leToken();
					return 1;
				}
				else { return 0; }
			}
			else { return 0; }
		}
		else { return 0; }
	}
	else { return 0; }
}

int	WHILE()
{
	if (tk == TKWhile) // tkWhile
	{
		leToken();
		if (tk == TKAbrePar) // tkAbreParenteses
		{
			leToken();
			if (COMP0())
			{
				if (tk == TKFechaPar) // tkFechaParenteses
				{
					if (COMANDO())
					{
						if (tk == TKEnd) // tkEnd
						{
							return 1;
						}
						else { return 0; }
					}
					else { return 0; }
				}
				else { return 0; }
			}
			else { return 0; }
		}
		else { return 0; }
	}
	else { return 0; }
}

int FOR()
{
	if (tk == TKFor) // tkFor
	{
		leToken();
		if (tk == TKAbrePar) // tkAbrePar
		{
			leToken();
			if (ATRIB())
			{
				if (tk == TKPontoeVirg) // tkPontoVirgula
				{
					leToken();
					if (COMP0())
					{
						if (tk == TKPontoeVirg) // tkPontoVirgula
						{
							leToken();
							if (ATRIB())
							{
								if (COMANDO())
								{
									if (tk == TKEnd) // tkEnd
									{
										return 1;
									}
								}
								else { return 0; }
							}
							else { return 0; }
						}
						else { return 0; }
					}
					else { return 0; }
				}
				else { return 0; }
			}
			else { return 0; }
		}
		else { return 0; }
	}
	else { return 0; }
}

int VAL()
{
	if (id())
	{
		return 1;
	}
	else if (cte())
	{
		return 1;
	}
	else if (EXP())
	{
		return 1;
	}
	else if (FUNCTION())
	{
		return 1;
	}
}

int ATRIB()
{
	if (id())
	{
		if (tk == TKAtrib) // tkAtrib
		{
			if (VAL())
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int COMANDO()
{
	if (ATRIB())
	{
		return 1;
	}
	else if (FOR())
	{
		return 1;
	}
	else if (WHILE())
	{
		return 1;
	}
	else if (SWITCH())
	{
		return 1;
	}
	else if (IF())
	{
		return 1;
	}
	else if (TRY())
	{
		return 1;
	}
	else if (PARFOR())
	{
		return 1;
	}
	else if (CRIAFUNCTION())
	{
		return 1;
	}
	else if (FUNCTION())
	{
		return 1;
	}
	else if (tk == TKBreak) // TKbreak
	{
		leToken();
		return 1;
	}
	else if (tk == TKContinue)
	{
		leToken();
		return 1;
	}
	else if (tk == TKReturn)
	{
		if (VAL())
		{
			return 1;
		}
		else
		{
			leToken();
			return 1;
		}
	}
	else if (COMENTARIO())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int BLOCO()
{
	if (COMANDO())
	{
		if (tk == TKPontoeVirg)
		{
			leToken();
		}
		BLOCO();
		return 1;
	}
	else
	{
		return 0;
	}
}

int INICIO()
{
	if (BLOCO())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int palavra_reservada(char lex[])
{
	int postab = 0;
	while (strcmp("fimtabela", lista_pal[postab].palavra) != 0)
	{
		if (strcmp(lex, lista_pal[postab].palavra) == 0)
		{
			return lista_pal[postab].tk;
		}
		postab++;
	}
	return TKId;
}

int main()
{
	int i = 0;
	char ch;
	size_t space = 1;
	characters = (char *)malloc(space);

	FILE * fp = fopen("Entrada.m", "r");

	if (fp == NULL)
	{
		printf("Erro ao abrir o arquivo de entrada (Entrada.m).\n");
		getchar();
		exit(1);
	}

	while ((ch = fgetc(fp)) != EOF)
	{
		characters[i] = ch;
		i++;
		space++;
		characters = (char *)realloc(characters, space);
	}

	characters[i] = '\0';

	newFile = fopen("Saida.lex", "wb+");

	if (newFile == NULL)
	{
		printf("Erro ao abrir o arquivo de saida (Saida.lex).\n");
		getchar();
		exit(1);
	}

	do
	{
		leToken();
		if (!INICIO())
		{
			printf("Erro");
			return 0;
		}
	} while (tk != TKFim);

	/*while ((tk = rec_equ(characters, lex, &linha, &coluna)) != TKFim)
	{
	if (tk == TKErro)
	{
	printf("Ocorreu um erro lexico!\n");
	break;
	}

	printf("Token: %d\t Linha: %d\t Coluna: %d\tLex: %s \n", tk, linha, coluna, lex);
	fprintf(newFile, "Token: %d\t Linha: %d\t Coluna: %d\tLex: %s \n", tk, linha, coluna, lex);
	coluna += strlen(lex) - 1;
	if (lex[strlen(lex) - 1] == '\n')
	linha++;
	}*/

	fclose(newFile);

	printf("Arquivo gerado com sucesso!");
	getchar();
	//system("pause");
	return 0;
}
