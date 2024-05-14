CC = gcc
CFLAGS = -Wall -Wextra -fopenmp -lm

# Список исходных файлов (.c)
SOURCES = $(wildcard *.c)

# Список программ (.out)
EXECUTABLES = $(SOURCES:.c=.out)

all: $(EXECUTABLES)

# Правило для компиляции каждого исходного файла в отдельную программу
%.out: %.c
	$(CC)  $< -o $@ $(CFLAGS)

# Правило для запуска всех программ
run: $(EXECUTABLES)
	@for exec in $(EXECUTABLES); do \
		echo "Running $$exec:"; \
		./$$exec; \
	done

clean:
	rm -f $(EXECUTABLES)
