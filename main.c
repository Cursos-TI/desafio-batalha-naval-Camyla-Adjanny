#include <stdio.h>
#include <stdlib.h> // abs()

#define TAM_TABULEIRO 10
#define TAM_NAVIO 3
#define VALOR_NAVIO 3
#define AGUA 0
#define ACERTO -1

#define TAM_HABILIDADE 5 // Tamanho da matriz de habilidade

// Inicializa o tabuleiro com água (~)
void inicializarTabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    for (int i = 0; i < TAM_TABULEIRO; i++)
        for (int j = 0; j < TAM_TABULEIRO; j++)
            tabuleiro[i][j] = AGUA;
}

// Exibe o tabuleiro com emojis e símbolos
void exibirTabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    printf("🗺️ Tabuleiro:\n  ");
    for (int c = 0; c < TAM_TABULEIRO; c++)
        printf("%d ", c);
    printf("\n");

    for (int i = 0; i < TAM_TABULEIRO; i++) {
        printf("%d ", i);
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            char simbolo;
            if (tabuleiro[i][j] == AGUA)
                simbolo = '~';  // Água
            else if (tabuleiro[i][j] == VALOR_NAVIO)
                simbolo = 'N';  // Navio
            else if (tabuleiro[i][j] == ACERTO)
                simbolo = 'X';  // Acerto
            else
                simbolo = '?';
            printf("%c ", simbolo);
        }
        printf("\n");
    }
    printf("\n");
}

// Função para exibir matriz de habilidade (0 e 1)
void exibirMatrizHabilidade(int matriz[TAM_HABILIDADE][TAM_HABILIDADE]) {
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Posiciona navio horizontalmente
int posicionarNavioHorizontal(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO], int linha, int coluna) {
    if (linha < 0 || linha >= TAM_TABULEIRO) return 0;
    if (coluna < 0 || coluna + TAM_NAVIO > TAM_TABULEIRO) return 0;
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (tabuleiro[linha][coluna + i] != AGUA)
            return 0;
    }
    for (int i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[linha][coluna + i] = VALOR_NAVIO;
    }
    return 1;
}

// Posiciona navio verticalmente
int posicionarNavioVertical(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO], int linha, int coluna) {
    if (coluna < 0 || coluna >= TAM_TABULEIRO) return 0;
    if (linha < 0 || linha + TAM_NAVIO > TAM_TABULEIRO) return 0;
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (tabuleiro[linha + i][coluna] != AGUA)
            return 0;
    }
    for (int i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[linha + i][coluna] = VALOR_NAVIO;
    }
    return 1;
}

// Ataca uma posição no tabuleiro
int atacar(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO], int linha, int coluna) {
    if (linha < 0 || linha >= TAM_TABULEIRO || coluna < 0 || coluna >= TAM_TABULEIRO)
        return -1; // Fora dos limites
    if (tabuleiro[linha][coluna] == VALOR_NAVIO) {
        tabuleiro[linha][coluna] = ACERTO;
        return 1; // Acertou
    }
    if (tabuleiro[linha][coluna] == AGUA)
        return 0; // Errou
    return -1; // Já atacado
}

// Verifica se todos os navios foram destruídos
int verificarVitoria(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    for (int i = 0; i < TAM_TABULEIRO; i++)
        for (int j = 0; j < TAM_TABULEIRO; j++)
            if (tabuleiro[i][j] == VALOR_NAVIO)
                return 0; // Ainda navios intactos
    return 1; // Todos destruídos
}

// ===== Habilidades Especiais =====

// Preenche matriz com zeros
void zerarMatriz(int matriz[TAM_HABILIDADE][TAM_HABILIDADE]) {
    for (int i = 0; i < TAM_HABILIDADE; i++)
        for (int j = 0; j < TAM_HABILIDADE; j++)
            matriz[i][j] = 0;
}

// Habilidade Cone 🔺 (padrão triangular apontando para baixo)
void habilidadeCone(int matriz[TAM_HABILIDADE][TAM_HABILIDADE]) {
    zerarMatriz(matriz);
    int meio = TAM_HABILIDADE / 2;
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = meio - i; j <= meio + i; j++) {
            if (j >= 0 && j < TAM_HABILIDADE)
                matriz[i][j] = 1;
        }
    }
}

// Habilidade Cruz ➕ (linha e coluna centrais)
void habilidadeCruz(int matriz[TAM_HABILIDADE][TAM_HABILIDADE]) {
    zerarMatriz(matriz);
    int meio = TAM_HABILIDADE / 2;
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        matriz[meio][i] = 1; // linha do meio
        matriz[i][meio] = 1; // coluna do meio
    }
}

// Habilidade Octaedro 🔷 (distância Manhattan <= centro)
void habilidadeOctaedro(int matriz[TAM_HABILIDADE][TAM_HABILIDADE]) {
    zerarMatriz(matriz);
    int centro = TAM_HABILIDADE / 2;
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            if (abs(i - centro) + abs(j - centro) <= centro)
                matriz[i][j] = 1;
        }
    }
}

// Menu para escolher habilidade especial
void mostrarMenuHabilidades() {
    printf("Escolha a habilidade especial para exibir no padrão (5x5):\n");
    printf("1 - Cone 🔺\n");
    printf("2 - Cruz ➕\n");
    printf("3 - Octaedro 🔷\n");
    printf("0 - Voltar ao jogo\n");
    printf("Digite sua escolha: ");
}

int main() {
    int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO];
    inicializarTabuleiro(tabuleiro);

    // Posiciona navios (definidos no código)
    if (!posicionarNavioHorizontal(tabuleiro, 2, 4)) {
        printf("❌ Erro ao posicionar navio horizontal!\n");
        return 1;
    }
    if (!posicionarNavioVertical(tabuleiro, 5, 1)) {
        printf("❌ Erro ao posicionar navio vertical!\n");
        return 1;
    }

    int linha, coluna;
    int escolha;
    int matrizHabilidade[TAM_HABILIDADE][TAM_HABILIDADE];

    printf("🎮 Bem-vindo ao Batalha Naval com Habilidades Especiais!\n");

    while (1) {
        printf("\n");
        exibirTabuleiro(tabuleiro);

        if (verificarVitoria(tabuleiro)) {
            printf("🎉 Parabéns! Todos os navios foram destruídos! Fim de jogo.\n");
            break;
        }

        printf("Menu:\n");
        printf("1 - Atacar posição\n");
        printf("2 - Mostrar habilidade especial\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        if (escolha == 1) {
            printf("Digite linha e coluna para atacar (0 a 9): ");
            scanf("%d %d", &linha, &coluna);

            int resultado = atacar(tabuleiro, linha, coluna);
            if (resultado == 1)
                printf("💥 Acertou um navio!\n");
            else if (resultado == 0)
                printf("🌊 Errou. Água!\n");
            else if (resultado == -1)
                printf("❌ Coordenadas inválidas ou já atacadas!\n");
        }
        else if (escolha == 2) {
            while (1) {
                mostrarMenuHabilidades();
                scanf("%d", &escolha);
                if (escolha == 0)
                    break;
                else if (escolha == 1) {
                    printf("🧭 Habilidade: CONE 🔺\n");
                    habilidadeCone(matrizHabilidade);
                    exibirMatrizHabilidade(matrizHabilidade);
                }
                else if (escolha == 2) {
                    printf("➕ Habilidade: CRUZ ➕\n");
                    habilidadeCruz(matrizHabilidade);
                    exibirMatrizHabilidade(matrizHabilidade);
                }
                else if (escolha == 3) {
                    printf("🔷 Habilidade: OCTAEDRO 🔷\n");
                    habilidadeOctaedro(matrizHabilidade);
                    exibirMatrizHabilidade(matrizHabilidade);
                }
                else {
                    printf("❌ Opção inválida!\n");
                }
            }
        }
        else if (escolha == 0) {
            printf("👋 Saindo do jogo. Até a próxima!\n");
            break;
        }
        else {
            printf("❌ Opção inválida! Tente novamente.\n");
        }
    }
    return 0;
}
