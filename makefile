# commands
CXX = g++
DEL = rm -f

# options
DEBUG = -g
WARNING = -Wall
OPTMIZE = -O3
STD = -std=c++17
CXXFLAGS = $(STD) $(WARNING)

#libraries
LIBS =

# directories
DECLRDIR = -I ./include
SRCDIR = ./src/
BINDIR = ./bin/

EXEC = compiler

_OBJ = Token.o Lexer.o ProgramLexer.o Parser.o ProgramParser.o FullType.o \
	AST/IdNode.o AST/ScalarNode.o AST/BinOpNode.o AST/UnOpNode.o \
	AST/PrintNode.o AST/ImportNode.o AST/ExportNode.o AST/SectionNode.o \
	AST/ForNode.o AST/DimensionsNode.o AST/AssignNode.o AST/ProgramNode.o \
	AST/CommandNode.o AST/ExprNode.o AST/FlipNode.o AST/ResizeNode.o \
	AST/RotateNode.o AST/CropNode.o AST/ModifyNode.o AST/ASTVisitor.o \
	PrintVisitor.o TypeVisitor.o PythonVisitor.o main.o
OBJ = $(patsubst %,$(BINDIR)%,$(_OBJ))

### executable target
main: bin gen $(OBJ)
	$(CXX) $(OBJ) -o $(BINDIR)$(EXEC) $(CXXFLAGS) $(LIBS)

bin:
	mkdir -p ./bin/
	mkdir -p ./bin/AST/

gen:
	cog -r include/ProgramLexer.h
	cog -r src/ProgramLexer.cpp

$(BINDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(DECLRDIR)

### clear objects & executable
clean: bin
	$(DEL) ./$(EXEC)
	$(DEL) $(BINDIR)*.o
	$(DEL) $(BINDIR)AST/*.o