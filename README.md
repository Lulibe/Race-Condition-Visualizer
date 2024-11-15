# Simulação de Compra de Assentos de Ônibus com Threads e Visualização Gráfica (SDL2)

Este projeto simula a compra de assentos em um ônibus utilizando múltiplas threads. Cada thread representa um cliente tentando reservar um assento aleatório. O sistema pode ser executado com ou sem sincronização utilizando mutexes, permitindo demonstrar condições de corrida (race conditions) em ambientes sem controle de concorrência.

## Funcionalidades

- Simulação de clientes tentando comprar assentos aleatoriamente.
- Controle de concorrência com mutexes, evitando problemas de **race condition**.
- Visualização gráfica utilizando a biblioteca **SDL2**, mostrando o status de cada assento (vazio, ocupado ou com overbooking).

## Objetivos

- Demonstrar o uso de threads e sincronização em C.
- Explorar problemas de concorrência e **race conditions**.
- Prover uma interface gráfica simples para visualizar o estado dos assentos.

## Requisitos

- Compilador **C** (gcc ou similar).
- Biblioteca **SDL2** (para a interface gráfica).

## Como Executar

1. **Clone este repositório**:

    ```bash
    git clone https://github.com/Lulibe/Race-Condition-Visualizer.git
    cd Race-Condition-Vizualizer
    ```

2. **Compile o programa** utilizando o `Makefile`:

    ```bash
    make
    ```

3. **Execute o programa**:
   - Com mutex (sincronização):

     ```bash
     make run_mutex
     ```

   - Sem mutex (possíveis condições de corrida):

     ```bash
     make run
     ```
