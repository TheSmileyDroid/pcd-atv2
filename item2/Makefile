override CFLAGS += -g -lm -fopenmp

all: build/ build/pragma_omp_critical build/pragma_reduction
	@echo "Rodando o programa com critical"
	@build/pragma_omp_critical
	@echo ""
	@echo "Rodando o programa com reduction"
	@build/pragma_reduction


build/:
	mkdir -p build


build/pragma_omp_critical: pragma_omp_critical.c
	$(CC) $(CFLAGS) $< -o $@

build/pragma_reduction: pragma_reduction.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f build/*
	
