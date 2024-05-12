CC = gcc
CFLAGS = -Wall -fopenmp -lm

# Получаем список всех файлов с расширением .c в текущем каталоге
SRCS := $(wildcard *.c)

# Формируем имена исполняемых файлов из имен исходных файлов
BINS := $(SRCS:.c=)

all: $(BINS)

# Правило для компиляции каждого исходного файла в отдельную программу
%: %.c
	$(CC) -o $@ $< $(CFLAGS)


run: $(BINS)
	@for bin in $(BINS); do \
		echo "Running $$bin..."; \
		./$$bin 30; \
	done

clean:
	rm -f $(BINS)
