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

struct token {
	int tk;
	int linha;
	int coluna;
};


int tk;
int linha = 1, coluna = 0;
char* characters;
struct token* tokens;
int pos = 0, posTK = -1, ultPosTK = -1;
FILE * newFile;
size_t space = 1;

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
			return TKErro;
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
				posl = 0;
				//return TKComentario;
				estado = 0;
				break;
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
			if (c == '\n')
			{
				(*linha) += 1;
				(*coluna) = 0;
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
			posl = 0;
			//return TKComentario;
			estado = 0;
			break;
		case 6:
			if (c != '\n' && c != '\0')
			{
				pos++;
			}
			else
			{
				posl = 0;
				//return TKComentario;
				estado = 0;
			}
			break;
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

	posTK++;
	if (posTK > ultPosTK)
	{
		tk = rec_equ(characters, lex, &linha, &coluna);
		if (tk == TKErro)
		{
			printf("Token desconhecido!\tLinha: %d\tColuna: %d\n", linha, coluna);
			return;
		}
		if (tk == TKFim)
		{
			return;
		}
		ultPosTK = posTK;
		tokens = (struct token *)realloc(tokens, sizeof(struct token) * space);
		tokens[posTK].tk = tk;
		tokens[posTK].linha = linha;
		tokens[posTK].coluna = coluna;
		fprintf(newFile, "Token: %d\t Linha: %d\t Coluna: %d\tLex: %s \n", tk, linha, coluna, lex);
		coluna += strlen(lex) - 1;
		if (lex[strlen(lex) - 1] == '\n')
		{
			linha++;
		}

	}
	else
	{
		tk = tokens[posTK].tk;
	}
}

void msgErro(char* msg)
{
	printf("Erro\n");
	printf("%s \tLinha: %d \tColuna: %d\n", msg, tokens[posTK].linha, tokens[posTK].coluna);
}

void erroAbrePar()
{
	msgErro("Parenteses esperado!");
}

void erroFechaPar()
{
	msgErro("Faltou fechar os parenteses!");
}

void erroExpInvalida()
{
	msgErro("Expressao invalida!");
}

void erroEnd()
{
	msgErro("END esperado!");
}

void erroVal()
{
	msgErro("Falta informar o valor!");
}

void erroDoisPt()
{
	msgErro("Dois pontos esperado!!");
}

void ocorreuErro()
{
	msgErro("Ocorreu um erro");
}

void voltaPos(int posicao)
{
	posTK = posicao;
	tk = tokens[posTK].tk;
}

int setPos()
{
	return posTK;
}

int EXP0();
int FUNCTION();
int EXP1();
int COMP0();
int BLOCO();
int VAL();

int id()
{
	if (tk == TKId)
	{
		leToken();
		return 1;
	}
	return 0;
}

int cte()
{
	if (tk == TKConstanteInteira)
	{
		leToken();
		return 1;
	}
	if (tk == TKConstanteReal)
	{
		leToken();
		return 1;
	}
	if (tk == TKString)
	{
		leToken();
		return 1;
	}
	if (tk == TKTrue)
	{
		leToken();
		return 1;
	}
	if (tk == TKFalse)
	{
		leToken();
		return 1;
	}
	return 0;
}

int ATRIB()
{
	if (id())
	{
		if (tk == TKAtrib)
		{
			leToken();
			if (VAL())
			{
				return 1;
			}
			erroVal();
			return 0;
		}
		return 0;
	}
	return 0;
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
			erroFechaPar();
			return 0;
		}
		return 0;
	}
	int marcaPos = setPos();
	if (id())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (cte())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (FUNCTION())
	{
		return 1;
	}
	return 0;
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
		erroExpInvalida();
		return 0;
	}
	else if (EXPFIM())
	{
		return 1;
	}
	return 0;
}

int EXP14()
{
	if (tk == TKPotencia)
	{
		leToken();
		if (EXP15())
		{
			if (tk == TKPotencia)
			{
				if (EXP14())
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		erroExpInvalida();
		return 0;
	}
	return 0;
}

int EXP13()
{
	if (EXP15())
	{
		if (tk == TKPotencia)
		{
			if (EXP14())
			{
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

int EXP12()
{
	if (tk == TKEBinario)
	{
		leToken();
		if (EXP13())
		{
			if (tk == TKEBinario)
			{
				if (EXP12())
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		erroExpInvalida();
		return 0;
	}
	return 0;
}

int EXP11()
{
	if (EXP13())
	{
		if (tk == TKEBinario)
		{
			if (EXP12())
			{
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

int EXP10()
{
	if (tk == TKDivisao)
	{
		leToken();
		if (EXP11())
		{
			if (tk == TKDivisao)
			{
				if (EXP10())
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		erroExpInvalida();
		return 0;
	}
	return 0;
}

int EXP9()
{
	if (EXP11())
	{
		if (tk == TKDivisao)
		{
			if (EXP10())
			{
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

int EXP8()
{
	if (tk == TKMultiplicacao)
	{
		leToken();
		if (EXP9())
		{
			if (tk == TKMultiplicacao)
			{
				if (EXP8())
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		erroExpInvalida();
		return 0;
	}
	return 0;
}

int EXP7()
{
	if (EXP9())
	{
		if (tk == TKMultiplicacao)
		{
			if (EXP8())
			{
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

int EXP6()
{
	if (tk == TKOuBinario)
	{
		leToken();
		if (EXP7())
		{
			if (tk == TKOuBinario)
			{
				if (EXP6())
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		erroExpInvalida();
		return 0;
	}
	return 0;
}

int EXP5()
{
	if (EXP7())
	{
		if (tk == TKOuBinario)
		{
			if (EXP6())
			{
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

int EXP4()
{
	if (tk == TKSubtracao)
	{
		leToken();
		if (EXP5())
		{
			if (tk == TKSubtracao)
			{
				if (EXP4())
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		erroExpInvalida();
		return 0;
	}
	return 0;
}

int EXP3()
{
	if (EXP5())
	{
		if (tk == TKSubtracao)
		{
			if (EXP4())
			{
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

int EXP2()
{
	if (tk == TKSoma)
	{
		leToken();
		if (EXP3())
		{
			if (tk == TKSoma)
			{
				if (EXP2())
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		erroExpInvalida();
		return 0;
	}
	return 0;
}

int EXP1()
{
	if (EXP3())
	{
		if (tk == TKSoma)
		{
			if (EXP2())
			{
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
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
			erroFechaPar();
			return 0;
		}
		return 0;
	}
	if (EXP1())
	{
		if (tk == TKMaior || tk == TKMaiorIgual || tk == TKMenor || tk == TKMenorIgual ||
			tk == TKIgual || tk == TKDiferente)
		{
			leToken();
			if (EXP1())
			{
				return 1;
			}
			erroExpInvalida();
			return 0;
		}
		return 1;
	}
	return 0;
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
		erroExpInvalida();
		return 0;
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
			if (tk == TKELogico)
			{
				if (COMP3())
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		erroExpInvalida();
		return 0;
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
		if (tk == TKELogico)
		{
			if (COMP3())
			{
				return 1;
			}
			return 0;
		}
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
			if (tk == TKOuLogico)
			{
				if (COMP1())
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		erroExpInvalida();
		return 0;

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
		if (tk == TKOuLogico)
		{
			if (COMP1())
			{
				return 1;
			}
			return 0;
		}
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
		if (BLOCO())
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
				if (tk == TKFechaPar)
				{
					leToken();
					if (BLOCO())
					{
						if (tk == TKElse || tk == TKElseIf)
						{
							if (ELSE())
							{
								return 1;
							}
							return 0;
						}
						return 1;
					}
					return 0;
				}
				erroFechaPar();
				return 0;
			}
			erroAbrePar();
			return 0;
		}
		return 0;
	}
	return 0;
}

int IF()
{
	if (tk == TKIf)
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
					if (BLOCO())
					{
						if (tk == TKElse || tk == TKElseIf)
						{
							if (!ELSE())
							{
								return 0;
							}
						}
						if (tk == TKEnd)
						{
							leToken();
							return 1;
						}
						erroEnd();
						return 0;
					}
					return 0;
				}
				erroFechaPar();
				return 0;
			}
			return 0;
		}
		erroAbrePar();
		return 0;
	}
	return 0;
}

int TRY()
{
	if (tk == TKTry)
	{
		leToken();
		if (BLOCO())
		{
			if (tk == TKCatch)
			{
				leToken();
				if (BLOCO())
				{
					if (tk == TKEnd)
					{
						leToken();
						return 1;
					}
					erroEnd();
					return 0;
				}
				return 0;
			}
			msgErro("CATCH faltando!");
			return 0;
		}
		return 0;
	}
	return 0;
}

int CRIAFUNCTION()
{
	if (tk == TKFunction)
	{
		leToken();
		if (FUNCTION())
		{
			if (BLOCO())
			{
				if (tk == TKEnd)
				{
					leToken();
					return 1;
				}
				erroEnd();
				return 0;
			}
			return 0;
		}
		return 0;
	}
	return 0;
}

int PARAM2()
{
	if (VAL())
	{
		return 1;
	}
	erroVal();
	return 0;
}

int PARAM1()
{
	if (tk == TKVirgula)
	{
		leToken();
		if (PARAM2())
		{
			if (tk == TKVirgula)
			{
				if (PARAM1())
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		return 0;
	}
	return 0;
}

int PARAM0()
{
	if (PARAM2())
	{
		if (tk == TKVirgula)
		{
			if (PARAM1())
			{
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
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
				erroFechaPar();
				return 0;
			}
			return 0;
		}
		return 0;
	}
	return 0;
}

int PARFOR()
{
	if (ATRIB())
	{
		if (tk == TKDoisPontos)
		{
			leToken();
			if (VAL())
			{
				if (tk == TKPontoeVirg)
				{
					leToken();
					if (BLOCO())
					{
						if (tk == TKPontoeVirg)
						{
							leToken();
							if (tk == TKEnd)
							{
								return 1;
							}
							erroEnd();
							return 0;
						}
						msgErro("Ponto e virgula esperado");
						return 0;
					}
					return 0;
				}
				msgErro("Ponto e virgula esperado");
				return 0;
			}
			erroVal();
			return 0;
		}
		erroDoisPt();
		return 0;
	}
	return 0;
}

int CASEVALUE2()
{
	if (cte())
	{
		if (tk == TKVirgula)
		{
			leToken();
			if (CASEVALUE2())
			{
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

int CASEVALUE1()
{
	if (cte())
	{
		if (tk == TKVirgula)
		{
			leToken();
			if (CASEVALUE2())
			{
				return 1;
			}
			return 0;
		}
		return 0;
	}
	return 0;
}

int CASEVALUE0()
{
	if (cte())
	{
		return 1;
	}
	else if (tk == TKAbreChave)
	{
		leToken();
		if (CASEVALUE1())
		{
			if (tk == TKFechaChave)
			{
				leToken();
				return 1;
			}
			msgErro("Falta fechar as chaves");
			return 0;
		}
		return 0;
	}
	return 0;
}

int CASE()
{
	if (tk == TKCase)
	{
		leToken();
		if (CASEVALUE0())
		{
			if (BLOCO())
			{
				if (tk == TKCase || tk == TKOtherwise)
				{
					if (CASE())
					{
						return 1;
					}
					return 0;
				}
				return 1;
			}
			return 0;
		}
		return 0;
	}
	else if (tk == TKOtherwise)
	{
		leToken();
		if (BLOCO())
		{
			return 1;
		}
		return 0;
	}
	return 0;
}

int SWITCH()
{
	if (tk == TKSwitch)
	{
		leToken();
		if (id())
		{
			if (CASE())
			{
				if (tk == TKEnd)
				{
					leToken();
					return 1;
				}
				erroEnd();
				return 0;
			}
			return 0;
		}
		return 0;
	}
	return 0;
}

int	WHILE()
{
	if (tk == TKWhile)
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
					if (BLOCO())
					{
						if (tk == TKEnd)
						{
							leToken();
							return 1;
						}
						erroEnd();
						return 0;
					}
					return 0;
				}
				erroFechaPar();
				return 0;
			}
			return 0;
		}
		erroAbrePar();
		return 0;
	}
	return 0;
}

int FOR()
{
	if (tk == TKFor)
	{
		leToken();
		if (ATRIB())
		{
			if (tk == TKDoisPontos)
			{
				leToken();
				if (VAL())
				{
					if (tk == TKDoisPontos)
					{
						leToken();
						if (!VAL())
						{
							erroVal();
							return 0;
						}
					}
					if (BLOCO())
					{
						if (tk == TKEnd)
						{
							leToken();
							return 1;
						}
						erroEnd();
						return 0;
					}
					return 0;
				}
				erroVal();
				return 0;
			}
			erroDoisPt();
			return 0;
		}
		return 0;
	}
	return 0;
}

int VAL()
{
	int marcaPos = setPos();
	if (FUNCTION())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (COMP0())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (id())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (cte())
	{
		return 1;
	}
	return 0;
}

int COMANDO()
{
	int marcaPos = setPos();
	if (ATRIB())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (FOR())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (WHILE())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (SWITCH())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (IF())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (TRY())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (PARFOR())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (CRIAFUNCTION())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (FUNCTION())
	{
		return 1;
	}
	voltaPos(marcaPos);
	if (tk == TKBreak)
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
		leToken();
		if (VAL())
		{
			return 1;
		}
		erroVal();
		return 0;
	}
	return 0;
}

int BLOCO()
{
	if (COMANDO())
	{
		if (tk == TKPontoeVirg)
		{
			leToken();
		}
		if (tk == TKId || tk == TKFor || tk == TKWhile || tk == TKSwitch ||
			tk == TKIf || tk == TKTry || tk == TKParfor || tk == TKFunction ||
			tk == TKBreak || tk == TKContinue || tk == TKReturn)
		{
			if (BLOCO())
			{
				return 1;
			}
			return 0;
		}
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

int main()
{
	int i = 0;
	char ch;
	characters = (char *)malloc(space);
	tokens = (struct token *)malloc(space);

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

	leToken();
	while (tk != TKFim)
	{
		if (!INICIO())
		{
			ocorreuErro();
			getchar();
			return 0;
		}
		leToken();
	}


	fclose(newFile);

	printf("Arquivo gerado com sucesso!");

	getchar();
	system("pause");
	return 0;
}
