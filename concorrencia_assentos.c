#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define NUM_ASSENTOS 20 //tem que ser par
#define NUM_CLIENTES 15
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400

int assentos[NUM_ASSENTOS];
int usar_mutex = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int desenha_grid(SDL_Renderer* renderer, int largura_assento){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2);
    for (int i = largura_assento; i < WINDOW_WIDTH; i = i + largura_assento) {
        SDL_RenderDrawLine(renderer, i, 0, i, WINDOW_HEIGHT);
    }
    return 1;
}

void* comprar_assento(void* arg) {
    int cliente_id = *(int*)arg;
    free(arg);

    for (int i = 0; i < NUM_ASSENTOS; i++) {
        int assento_escolhido = rand() % NUM_ASSENTOS;

        if (usar_mutex) {
            pthread_mutex_lock(&mutex);
        }

        if (assentos[assento_escolhido] == 0) {
            usleep(100);
            assentos[assento_escolhido]++;
            printf("Cliente %d comprou o assento %d.\n", cliente_id, assento_escolhido);

            if (usar_mutex) {
                pthread_mutex_unlock(&mutex);
            }
            return NULL;
        }

        if (usar_mutex) {
            pthread_mutex_unlock(&mutex);
        }
    }

    printf("Cliente %d não conseguiu comprar um assento (todos ocupados).\n", cliente_id);
    return NULL;
}

void desenhar_assentos(SDL_Renderer* renderer) {
    int largura_assento = WINDOW_WIDTH / (NUM_ASSENTOS / 2);
    int altura_assento = WINDOW_HEIGHT / 2;

    for (int i = 0; i < NUM_ASSENTOS; i++) {
        int x = i % (NUM_ASSENTOS / 2) * largura_assento;
        int y = (i < NUM_ASSENTOS / 2) ? 0 : altura_assento;

        if (assentos[i] > 1) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Vermelho (overbooking)
        } else if (assentos[i] == 1) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Azul (ocupado)
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Verde (vazio)
        }

        SDL_Rect rect = {x, y, largura_assento, altura_assento};
        SDL_RenderFillRect(renderer, &rect);
    }
    desenha_grid(renderer, largura_assento);
}

int main(int argc, char* argv[]) {
    pthread_t clientes[NUM_CLIENTES];
    srand(time(NULL));

    if (argc > 1 && strcmp(argv[1], "mutex") == 0) {
        usar_mutex = 1;
        printf("Executando com mutex para sincronização.\n");
    } else {
        printf("Executando sem mutex (possíveis condições de corrida).\n");
    }

    for (int i = 0; i < NUM_ASSENTOS; i++) {
        assentos[i] = 0;
    }

    for (int i = 0; i < NUM_CLIENTES; i++) {
        int* cliente_id = malloc(sizeof(int));
        *cliente_id = i + 1;
        pthread_create(&clientes[i], NULL, comprar_assento, cliente_id);
    }

    for (int i = 0; i < NUM_CLIENTES; i++) {
        pthread_join(clientes[i], NULL);
    }

    printf("\nResultado final dos assentos:\n");
    for (int i = 0; i < NUM_ASSENTOS; i++) {
        if (assentos[i] > 1) {
            printf("Assento %d: Overbooking\n", i);
            continue;
        }
        printf("Assento %d: %s\n", i, assentos[i] > 0 ? "Ocupado" : "Vazio");
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Assentos do Ônibus",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "Erro ao criar janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erro ao criar renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    desenhar_assentos(renderer);

    SDL_RenderPresent(renderer);

    SDL_Event e;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    if (usar_mutex) {
        pthread_mutex_destroy(&mutex);
    }

    return 0;
}
