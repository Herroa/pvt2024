CC = gcc
CFLAGS = -Wall -fopenmp -lm

SRCDIRS := atomic blocks computation critical local_vector
TARGETS := $(SRCDIRS:%=%/main)

.PHONY: all clean run

all: $(TARGETS)

%/main: %/main.c
	$(CC) -o $@ $< $(CFLAGS) 

clean:
	rm -f $(TARGETS)
	rm -f *.pdf
	rm -f *.dat

run: all
	@for dir in $(SRCDIRS); do \
		echo "Running program in $$dir..."; \
		./$$dir/main 20; \
		echo "Generating graph in $$dir..."; \
		gnuplot $$dir/graph.gp; \
	done
