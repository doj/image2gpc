# Name of the source and output
TARGET = image2gpc
SRC = image2gpc.cpp
OUT = $(TARGET).exe

# Compiler and flags
CXX = cl
CXXFLAGS = /nologo /O2 /EHsc /MD /DNDEBUG
LDFLAGS = /link /OUT:$(OUT)

# Default target
all: $(OUT)

# Link and compile
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS)

# Clean build artifacts
clean:
	del /Q $(OUT) *.obj *.pdb *.ilk

test:	all
	$(OUT) gta5-bw.png
