#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if _WIN32
    #define LimparTela "cls"
#else
    #define LimparTela "clear"
#endif

void ImprimirLista(FILE *Lista);                      //Imprime a lista de compras
void AdicionarNaLista(FILE *Lista, char Item[30]);    //Adiciona um novo ítem à lista de compras
void RemoverDaLista(FILE *Lista);                     //Remove um ítem de lista de compras
void Menu(FILE *Lista);

struct Item
{
    char Nome[30];
    int Quantidade;
    int Status; // 0 = não comprado, 1 = comprado
};

int main()
{
    FILE *Lista;

    Menu(Lista);

    return 0;
}

void ImprimirLista(FILE *Lista)
{
    char Item[30];
    int i = 1;

    Lista = fopen("ListaDeCompras.txt", "r");

    if (Lista == NULL){
        printf("\nLista vazia! Por favor, adicione itens.\n\n");
        return;
    }

    printf("Item    Nome\n");
    while (fgets(Item, sizeof(Item), Lista) != NULL)
    {
        Item[strcspn(Item, "\n")] = '\0';
        printf("[%02d]    %-30s\n", i++, Item);
    }

    fclose(Lista);
}

void AdicionarNaLista(FILE *Lista, char* Item)
{
    Lista = fopen("ListaDeCompras.txt", "a");

    fgets(Item, sizeof(char [30]), stdin);
    if (Item[0] != '\n')
        fputs(("%s\n", Item), Lista);
    // fprintf(Lista, "%s\n", Item);

    fclose(Lista);
}

void RemoverDaLista(FILE *Lista)
{
    int selecionados[50], PosicaoSelecionada, i;
    char Item[30];
    FILE *Auxiliar;

    for (PosicaoSelecionada = 0; PosicaoSelecionada < 50; PosicaoSelecionada++)
        selecionados[PosicaoSelecionada] = 0;

    do
    {
        Lista = fopen("ListaDeCompras.txt", "r");
        i = 1;

        if (Lista == NULL){
            printf("\nLista vazia! Por favor, adicione itens.\n\n");
            return;
        }
        system(LimparTela);
        printf("Item    Nome    Selecionado\n");
        while (fgets(Item, sizeof(Item), Lista) != NULL)
        {
            Item[strcspn(Item, "\n")] = '\0';
            printf("[%02d]    %-30s [%s]\n", i, Item, (selecionados[i] ? "#" : " "));
            i++;
        }

        fclose(Lista);

        printf("Qual ítem deseja selecionar para remover? [0 para terminar]: ");
        scanf("%d", &PosicaoSelecionada);
            scanf("%*c");
        if (PosicaoSelecionada != 0)
            selecionados[PosicaoSelecionada] = 1;

    } while (PosicaoSelecionada != 0);

    printf("Removendo...\n");

    Auxiliar = fopen("Auxiliar.txt", "w");

    Lista = fopen("ListaDeCompras.txt", "r");
    i = 1;

    if (Lista == NULL){
        printf("\nLista vazia! Por favor, adicione itens.\n\n");
        return;
    }
    
    while (fgets(Item, sizeof(Item), Lista) != NULL)
    {
        if (!selecionados[i])
            fputs(Item, Auxiliar);
        i++;
    }

    fclose(Lista);
    fclose(Auxiliar);
    remove("ListaDeCompras.txt");
    rename("Auxiliar.txt", "ListaDeCompras.txt");
}

void Menu(FILE *Lista)
{
    char Item[30];
    int QntItens, Funcao;

    do
    {
        system(LimparTela);
        printf("É pra fazer o que?\n");
        printf("| [1] Imprimir a lista   |\n");
        printf("| [2] Adicionar na lista |\n");
        printf("| [3] Remover da lista   |\n");
        printf("| [0] Sair               |\n");

        scanf("%d", &Funcao);
        scanf("%*c");

        system(LimparTela);
        switch (Funcao)
        {
            case 0:
                break;
            case 1:
                printf("Você selecionou ImprimirLista()\n");
                ImprimirLista(Lista);
                break;
            case 2:
                printf("Você selecionou AdicionarNaLista()\n");

                do
                {
                    printf("Nome do ítem [ENTER para terminar]: ");
                    AdicionarNaLista(Lista, Item);
                } while (strcmp(Item, "\n") != 0);

                printf("Ítem(s) adicionado(s) com sucesso!\n");
                break;
            case 3:
                printf("Você selecionou RemoverDaLista()\n");
                RemoverDaLista(Lista);
                printf("Ítem removido com sucesso!\n");
                break;
            default:
                printf("OPÇÃO INVÁLIDA!\n");
        }

        if (Funcao != 0)
        {
            printf("Pressione ENTER para continuar...");
            getchar();
        }
        else
        {
            printf("\n\n=========== Programa encerrado ===========\n\n");
        }
    } while (Funcao != 0);
}