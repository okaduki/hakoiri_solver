PROGRAM = hakoiri_solver
CXXFLAGS += -std=c++11 -O3
OBJS = hakoiri_solver.o

$(PROGRAM): $(OBJS)
	g++ $^ -o $@

.PHONY: clean

clean:
	$(RM) $(PROGRAM) $(OBJS)