override CFLAGS += -g -pthread -lm

all: build/ build/main-wait build/main-nowait
	@echo "Rodando o programa com espera"
	@build/main-wait
	@echo "Rodando o programa sem espera"
	@build/main-nowait


build/:
	mkdir -p build


build/main-wait: main-wait.c
	$(CC) $(CFLAGS) $< -o $@

build/main-nowait: main-nowait.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f build/main-wait build/main-nowait
