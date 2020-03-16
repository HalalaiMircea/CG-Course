
#include "AbstractProblem.hpp"
#include "Hw2Problem2.hpp"
#include "Hw2Problem1.hpp"

int main(int argc, char **argv) {
    AbstractProblem *problem = new Hw2Problem2("Intersectii Segmente");
    problem->setInstance(problem);
    problem->init(argc, argv);
    return 0;
}