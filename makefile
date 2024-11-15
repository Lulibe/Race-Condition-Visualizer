# Nome do executável
EXEC = concorrencia_assentos

# Arquivo fonte
SRC = concorrencia_assentos.c

# Flags do compilador
CFLAGS = -Wall -Wextra -pthread -lSDL2

# Alvo padrão
all: $(EXEC)

# Compilar o programa
$(EXEC): $(SRC)
	@gcc $(SRC) -o $(EXEC) $(CFLAGS)
	@echo "Compilado com sucesso!"

# Rodar o programa
run: $(EXEC)
	@./$(EXEC)

# Rodar o programa com mutex
run_mutex: $(EXEC)
	@./$(EXEC) mutex

# Limpar os arquivos gerados
clean:
	@rm -f $(EXEC)
	@echo "Arquivos removidos!"
