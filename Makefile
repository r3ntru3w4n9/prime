
CXX = g++-9
CXXFLAGS = -rdynamic -std=c++11 -Wall -Wextra -Werror -flto -g
TARGETS = src/main.o \
		  src/core/Cell.o src/core/Grid.o src/core/MasterCell.o src/core/Chip.o \
		  src/Node.o src/Tree.o src/MergeTree.o src/union_find.o \
		  src/router/router3d.o src/router/cost_function.o \
		  src/quad/QuadUtil.o src/quad/QuadNode.o src/quad/QuadTree.o src/quad/QuadForest.o
# TARGETS = test_quad_forest.o Cell.o Grid.o MasterCell.o Chip.o Node.o Tree.o MergeTree.o union_find.o router3d.o cost_function.o QuadUtil.o QuadNode.o QuadTree.o QuadForest.o
BINARY = prime
FLAGS =

.PHONY: all clean

all:
	$(MAKE) clean
	$(MAKE) $(BINARY) -j

$(BINARY): $(TARGETS)
	$(CXX) $(CXXFLAGS) $? -o $(BINARY) $(FLAGS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(FLAGS)

clean:
	rm -f $(BINARY) $(TARGETS)
