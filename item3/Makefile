SRCS = $(wildcard *.java)

CLASSES = $(SRCS:.java=.class)

all: $(CLASSES)
	@java Main

$(CLASSES): $(SRCS)
	javac $(SRCS)

clean:
	rm -f *.class