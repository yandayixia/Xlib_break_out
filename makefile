CXX = g++
CXXFLAGS = -g
ENDFLAGS = -lX11
EXEC = Break_Out
BLOCK = Source_Code/BaseBlock.cpp Source_Code/GameObject.cpp Source_Code/InputManager.cpp Source_Code/Paddle.cpp Source_Code/Orb.cpp Source_Code/TemplateObject.cpp Source_Code/GameManager.cpp Source_Code/IronBlock.cpp
SOURCES = ${BLOCK} Main_Game.cpp
OBJECTS = ${SOURCES:.cpp=.o}

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) $(ENDFLAGS)

.cpp.o: 
	$(CXX) -c $(CXXFLAGS) -o $@ $< $(ENDFLAGS)

clean:
	rm *o Source_Code/*o $(EXEC)

run:all
	@echo "Running..."
	./$(EXEC) $0 $1

.PHONY: all

.PHONY: run

.PHONY: clean