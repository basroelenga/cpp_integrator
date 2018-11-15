CXX=mpiCC

EXEC = integrator_mpi
DEPS = parser.h parameters.h core.h integrator.h force.h dust_gas.h snapshot.h
OBJS = main.o parser.o parameters.o core.o integrator.o force.o dust_gas.o snapshot.o

CXXFLAGS = -std=c++17 -lstdc++ -lm -Wall

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC)
	mv $(EXEC) ..

$(OBJS): $(DEPS)

clean:
	rm $(OBJS)
