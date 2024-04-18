//DUPLA: Paulo Vinicius Huwer e Igor Bermude Severnini
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char string[25];

typedef struct registro {
  int id_filme;
  string titulo;
  int ano;
  float bilheteria;
  struct registro *prox;
  struct registro *ante;
} TRegistro;

typedef struct bloco {
  int numeroBloco;
  struct registro *registro;
  struct bloco *prox;
} TBloco;

typedef struct arvore {
  string nomeFil;
  int idFil;
  int bloco;
  struct arvore *esquerda;
  struct arvore *direita;
} TArvore;

typedef struct filme {
  struct bloco *inicio;
  struct bloco *fim;
  struct arvore *raiz;
} TFilme;

int numeroBlocoGlobal =
    1; // Variável global para gerar automaticamente o número do bloco

TRegistro registro;
TBloco bloco;
TArvore arvore;
TFilme filme;

// Funções:
void inicializafilme(TFilme *l);
void inicializaarvore(TArvore *r);
int menu();
void insereBloco(TFilme *f);
void insere_bloco(TFilme *f);
void insereRegistro(TFilme *f);
int verificaIdExistente(TFilme *f, int id);
TBloco *localizaBloco(TFilme *f, int bloco);
void insere_registro(TFilme *f, TBloco *bloco);
void buscarFilmePorTitulo(TFilme *f);
TBloco *localizaBlocoPorNumero(int numeroBloco);
TBloco *buscaTituloNaArvore(TArvore *raiz, string titulo);
void insereNaArvore(TArvore **raiz, int bloco, string titulo, int idFilme);
TRegistro *achaFilmeNoBloco(TBloco *bloco, string titulo);
void buscarFilmePorId(TFilme *f);
TRegistro *achaFilmeNoBlocoId(TBloco *bloco, int idFilme);
TBloco *buscaBlocoPorId(TFilme *f, int id);
void apagarFilme(TFilme *f);


int main() {
  int opcao;

  inicializafilme(&filme);
  inicializaarvore(&arvore);
  insereBloco(&filme);

  do {
    opcao = menu();

    switch (opcao) {
    case 1:
      insereRegistro(&filme);
      break;
    case 2:
      buscarFilmePorTitulo(&filme);
      break;
    case 3:
      buscarFilmePorId(&filme);
      break;
    case 4:
      apagarFilme(&filme);
      break;
    } // switch

  } while (opcao != 0);
}

//=============================================================
int menu() {
  int opcao;

  system("CLS");

  printf("\n\n\t\t=====| MENU |=====\n\n\t0 - SAIR (Encerrar Programa).\n");
  printf("\n\n\t1 - Inserir FILME Novo.\n");
  printf("\n\n\t2 - Buscar Dados de FILME por Título.\n");
  printf("\n\n\t3 - Buscar Dados de Filme por id.\n");
  printf("\n\n\t4 - Excluir FILME.\n");

  printf("\n\tInforme OPCAO desejada: ");
  scanf("%d", &opcao);

  if ((opcao < 0) || (opcao > 4)) {
    printf("\n\n\tERRO: Opcao INVALIDA!\n\tTente outra vez.\n\n");
    system("PAUSE");
  }

  return opcao;
}

//=============================================================
void inicializafilme(TFilme *l) {
  l->inicio = NULL;
  l->fim = NULL;
  l->raiz = NULL;
}
//=============================================================
void inicializabloco(TBloco *b) {
  b->numeroBloco = 0;
  b->registro = NULL;
  b->prox = NULL;
}
//=============================================================
void inicializaarvore(TArvore *r) { r = NULL; }
//=============================================================
void insereBloco(TFilme *f) { insere_bloco(f); }
//=============================================================
void insere_bloco(TFilme *f) {
  TBloco *novo = (TBloco *)malloc(sizeof(TBloco));
  TBloco *atual;

  novo->prox = NULL;
  novo->registro = NULL;
  novo->numeroBloco = numeroBlocoGlobal++;

  if (f->inicio == NULL) {
    f->inicio = novo;
    f->fim = novo; // Inicialize f->fim quando a lista estiver vazia
  } else {
    int flag = 0;
    atual = f->inicio;

    while (atual != NULL) {
      if (atual->numeroBloco == novo->numeroBloco) {
        novo->prox = atual;
        if (atual == f->inicio) {
          f->inicio = novo;
        }
        flag = 1;
        break;
      }
      atual = atual->prox;
    }

    if (!flag) {
      f->fim->prox = novo;
      f->fim = novo;
    }
  }
  system("PAUSE");
}
//=============================================================
void insereRegistro(TFilme *f) {
  int idFilme;
  int bloco;
  string titulo;
  int ano;
  float bilheteria;
  TBloco *regBloco;

  printf("\n\n\t\t-----| INSERE FILME |-----\n");
  printf("\tInforme o NUMERO do BLOCO: ");
  scanf("%d", &bloco);

  regBloco = localizaBloco(f, bloco);

  if (regBloco == NULL) {
    printf("\n\tBloco inexistente!\n");
  } else {
    insere_registro(f, regBloco);
  }
}
//=============================================================
TBloco *localizaBloco(TFilme *f, int bloco) {
  TBloco *atual = f->inicio;

  while (atual != NULL) {
    if (atual->numeroBloco == bloco) {
      break;
    }
    atual = atual->prox;
  }
  return atual;
}
//=============================================================
void insere_registro(TFilme *f, TBloco *bloco) {
  TRegistro *novo_registro = (TRegistro *)malloc(sizeof(TRegistro));

  int idFilme;
  string titulo;
  int ano;
  float bilheteria;

  printf("\n\tInforme o ID do Filme: ");
  scanf("%d", &idFilme);
  int aux = verificaIdExistente(f, idFilme);
  while (aux == 1) {
    printf("\n\tID já existente!\n");
    printf("\n\tInforme o ID do Filme: ");
    scanf("%d", &idFilme);
    aux = verificaIdExistente(f, idFilme);
  }

  printf("\n\tInforme o TITULO do FILME: ");
  scanf("%s", titulo);
  printf("\n\tInforme o ANO do FILME: ");
  scanf("%d", &ano);
  printf("\n\tInforme a BILHETERIA do FILME: ");
  scanf("%f", &bilheteria);

  novo_registro->id_filme = idFilme;
  strcpy(novo_registro->titulo, titulo);
  novo_registro->ano = ano;
  novo_registro->bilheteria = bilheteria;
  novo_registro->prox = NULL;
  novo_registro->ante = NULL;

  // Verifica se o bloco está cheio (três registros)
  int count = 0;
  TRegistro *atual_registro = bloco->registro;
  while (atual_registro != NULL) {
    count++;
    atual_registro = atual_registro->prox;
  }

  // Caso o bloco informado esteja cheio cria automaticamente um novo bloco.
  if (count >= 3) {
    insereBloco(f);
    // Atualiza o ponteiro "bloco" para apontar para o novo bloco que foi criado
    printf("\n\tBloco %d cheio!\n", bloco->numeroBloco);
    bloco = f->fim;
  }

  // Adiciona o registro ao bloco existente caso ele não esteja cheio
  if (bloco->registro == NULL) {
    bloco->registro = novo_registro;
  } else {
    atual_registro = bloco->registro;
    while (atual_registro->prox != NULL) {
      atual_registro = atual_registro->prox;
    }
    atual_registro->prox = novo_registro;
    novo_registro->ante = atual_registro;
  }
  printf("\n\n\tRegistro inserido com sucesso no bloco %d.\n",
         bloco->numeroBloco);
  system("PAUSE");

  // Insere na árvore
  insereNaArvore(&f->raiz, bloco->numeroBloco, titulo, idFilme);
}
//===============================================================
int verificaIdExistente(TFilme *f, int id) {
  TBloco *atualBloco = f->inicio;

  while (atualBloco != NULL) {
    TRegistro *registroAtual = atualBloco->registro;
    while (registroAtual != NULL) {
      if (registroAtual->id_filme == id) {
        return 1; // ID existente
      }
      registroAtual = registroAtual->prox;
    }
    atualBloco = atualBloco->prox;
  }
  return 0; // ID não encontrado
}

//===============================================================
void insereNaArvore(TArvore **raiz, int bloco, string titulo, int idFilme) {
  if (*raiz == NULL) {
    // Cria um novo nó da árvore
    *raiz = (TArvore *)malloc(sizeof(TArvore));
    (*raiz)->bloco = bloco;
    strcpy((*raiz)->nomeFil, titulo);
    (*raiz)->idFil = idFilme;
    (*raiz)->esquerda = NULL;
    (*raiz)->direita = NULL;
  } else {

    // Decide se o novo filme vai para a esquerda ou direita da árvore com base
    // no título
    int comparacao = strcmp(titulo, (*raiz)->nomeFil);

    if (comparacao < 0) {
      if ((*raiz)->esquerda == NULL) {
        // Insere na esquerda
        TArvore *novo = (TArvore *)malloc(sizeof(TArvore));
        novo->bloco = bloco;
        strcpy(novo->nomeFil, titulo);
        novo->idFil = idFilme;
        novo->esquerda = NULL;
        novo->direita = NULL;
        (*raiz)->esquerda = novo;
      } else {
        insereNaArvore(&(*raiz)->esquerda, bloco, titulo, idFilme);
      }

    } else {
      if ((*raiz)->direita == NULL) {
        // Insere na direita
        TArvore *novo = (TArvore *)malloc(sizeof(TArvore));
        novo->bloco = bloco;
        strcpy(novo->nomeFil, titulo);
        novo->idFil = idFilme;
        novo->esquerda = NULL;
        novo->direita = NULL;
        (*raiz)->direita = novo;
      } else {
        insereNaArvore(&(*raiz)->direita, bloco, titulo, idFilme);
      }
    }
  }
}
//================================================================

//================================================================
void buscarFilmePorTitulo(TFilme *f) {
  string titulo;
  printf("\n\n\tInforme o TITULO do FILME a ser buscado: ");
  scanf("%s", titulo);

  // Busca na árvore de busca
  TBloco *bloco = buscaTituloNaArvore(f->raiz, titulo);

  if (bloco != NULL) {
    // Exibe os dados do bloco
    TRegistro *filme = achaFilmeNoBloco(bloco, titulo);

    if (strcmp(filme->titulo, titulo) == 0) {
      printf("\n\n\t=====| DADOS DO BLOCO %d |=====\n", bloco->numeroBloco);
      printf("\n\tID: %d | TITULO: %s | ANO: %d | BILHETERIA: %.2f",
             filme->id_filme, filme->titulo, filme->ano, filme->bilheteria);
    }
  } else {
    printf("\n\n\tFilme com o titulo '%s' nao encontrado.\n", titulo);
  }
  system("PAUSE");
}
//================================================================
TBloco *buscaTituloNaArvore(TArvore *raiz, string titulo) {
  if (raiz == NULL) {
    return NULL; // Árvore vazia ou fim da busca
  }

  int comparacao = strcmp(titulo, raiz->nomeFil);

  if (comparacao == 0) {
    // Título encontrado, retorna o bloco correspondente
    return localizaBlocoPorNumero(raiz->bloco);
  } else if (comparacao < 0) {
    // Título é menor, busca na subárvore esquerda
    return buscaTituloNaArvore(raiz->esquerda, titulo);
  } else {
    // Título é maior, busca na subárvore direita
    return buscaTituloNaArvore(raiz->direita, titulo);
  }
}
//=================================================================
TBloco *localizaBlocoPorNumero(int numeroBloco) {
  TBloco *atual = filme.inicio;

  while (atual != NULL) {
    if (atual->numeroBloco == numeroBloco) {
      return atual; // Retorna o bloco correspondente
    }
    atual = atual->prox;
  }

  return NULL; // Bloco não encontrado
}

//=================================================================
TRegistro *achaFilmeNoBloco(TBloco *bloco, string titulo) {
  TRegistro *registroAtual = bloco->registro;
  while (registroAtual != NULL) {
    if (strcmp(registroAtual->titulo, titulo) == 0) {
      return registroAtual;
    }
    registroAtual = registroAtual->prox;
  }
  return NULL;
}
//=================================================================

//=================================================================
void buscarFilmePorId(TFilme *f) {
  int idFilme;
  printf("\n\n\tInforme o ID do FILME a ser buscado: ");
  scanf("%d", &idFilme);

  // Busca na árvore de busca
  TBloco *bloco = buscaBlocoPorId(f, idFilme);

  if (bloco != NULL) {
    // Exibe os dados do bloco
    TRegistro *filme = achaFilmeNoBlocoId(bloco, idFilme);

    if (filme->id_filme == idFilme) {
      printf("\n\n\t=====| DADOS DO BLOCO %d |=====\n", bloco->numeroBloco);
      printf("\n\tID: %d | TITULO: %s | ANO: %d | BILHETERIA: %.2f",
             filme->id_filme, filme->titulo, filme->ano, filme->bilheteria);
    }
  } else {
    printf("\n\n\tFilme com o ID '%d' nao encontrado.\n", idFilme);
  }
  system("PAUSE");
}

//===============================================================
TRegistro *achaFilmeNoBlocoId(TBloco *bloco, int idFilme) {
  TRegistro *registroAtual = bloco->registro;

  //Percorre obloco até encontrar o registro com o ID correspondente.
  while (registroAtual != NULL) {
    if (registroAtual->id_filme == idFilme) {
      printf("\n\nfefef\n");
      return registroAtual;
    }
    registroAtual = registroAtual->prox;
  }
  return NULL;
}

//===============================================================
TBloco *buscaBlocoPorId(TFilme *f, int id) {
  TBloco *atual = f->inicio;

  // Busca o id bloco por bloco
  while (atual != NULL) {
    TRegistro *registro = atual->registro;

    while (registro != NULL) {
      if (registro->id_filme == id) {
        return atual;
      }
      registro = registro->prox;
    }
    atual = atual->prox;
  }

  // Caso o id não tenha sido encontrado em nenhum bloco
  return NULL;
}

//===============================================================

//===============================================================
void apagarFilme(TFilme *f) {
  string titulo;
  TFilme *regRegistro;
  printf("\n\n\t\t-----| APAGAR FILME |-----\n\n\t\t");
  printf("\nInforme o TITULO do FILME a ser apagado: ");
  scanf("%s", titulo);

  TBloco *bloco = buscaTituloNaArvore(f->raiz, titulo);

  if (bloco != NULL) {
    TRegistro *filme = achaFilmeNoBloco(bloco, titulo);

    if (strcmp(filme->titulo, titulo) == 0) {
      if (filme->ante == NULL) {
        bloco->registro = filme->prox;
        if (filme->prox != NULL) {
          filme->prox->ante = NULL;
        }
      } else {
        filme->ante->prox = filme->prox;
        if (filme->prox != NULL) {
          filme->prox->ante = filme->ante;
        }
      }
      free(filme);
      printf("\n\tFilme '%s' apagado com sucesso.\n", titulo);
    } else {
      printf("\n\tFilme com o titulo '%s' nao encontrado.\n", titulo);
    }
  } else {
    printf("\n\tFilme com o titulo '%s' nao encontrado.\n", titulo);
  }
}
