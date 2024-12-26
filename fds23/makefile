CC = gcc
CFLAGS = -Wall -std=c99

TARGET = LBP

SRCS = main.c lbp.c comparacao.c imagem.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

%.o: %.c comparacao.h imagem.h lbp.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

cleanall: clean
	rm -f *.pgm *.lbp
