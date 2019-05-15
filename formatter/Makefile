BIN_FILES  = formatter

all: $(BIN_FILES)
.PHONY : all

formatter:
	javac -d . FormatterService.java
	javac -d . FormatterPublisher.java

clean:
	rm -f *.class

.SUFFIXES:
.PHONY : clean
