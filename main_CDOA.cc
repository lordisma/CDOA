#include "CDOA.h"
#include "buffer.h"
#include <iostream>

using namespace realea;

/*
TRABAJO A REALIZAR
-------------------
Usar codigo para inicializar parametros por terminal,
o desde fichero
*/

int main(int argc, char *argv[]){
  int dim = 50;
  int fun = 1;
  int maxevals = 15000 * dim;
  int numbersOfProof = 20;

  int seed=time(NULL);
  int type_ls=100;

  Buffer bff;
  bff.getFromParameter(argv[1],argv[2], argv[3] , type_ls, seed, dim);

  Random random(new SRandom(seed));

  tChromosomeReal sol(dim);
  ProblemCEC2014 cec2014(dim);
  ILocalSearch *ls;
  ILSParameters *ls_options;

  // Domain is useful for clipping solutions
  // Set the problem to allow the LS to eval solutions

  if (type_ls == 1) {
     // Get the Solis Wets problem
     SolisWets *sw = new SolisWets();
     // Set the delta values
     sw->setDelta(0.2);
     ls = sw;
  }
  else if (type_ls == 2) {
    ls = new SimplexDim();
  }
  else if (type_ls == 3) {
    CMAESHansen *cmaes = new CMAESHansen("cmaesinit.par");
    cmaes->searchRange(0.1);
    ls = cmaes;
  }

  //Algorithm definition and aplication
  CDOA alg(dim);
  CDOA best(dim);
  // Init the initial solution (for LS)
  // Set the problem to allow the LS to eval solutions
  ProblemPtr problem = cec2014.get(fun);
  DomainRealPtr domain;
  domain = problem->getDomain();
  ls->setProblem(problem.get());
  // Set the random to generate mutations
  ls->setRandom(&random);
  // Get the initial parameters
  ls_options = ls->getInitOptions(sol);

  getInitRandom(&random, domain, sol);
  tFitness fitness;
  tFitness parFitness = 0.0;//, //bestFitnes = 0.0;

  alg.setRandom(&random);
  alg.setProblem(problem);
  alg.setILocalSearch(ls);
  alg.setLSOption(ls_options);
/*
  best.setRandom(&random);
  best.setProblem(problem);
  best.setILocalSearch(ls);
  best.setLSOption(ls_options);
*/
for(unsigned f = 1; f <= 20; ){
  parFitness = 0.0;
  std::cout << "Inicio Funcion: " << f << '\n';
  for(unsigned i = 0; i < (unsigned)numbersOfProof; i++){
    alg.apply(sol,fitness,maxevals);
    parFitness += fitness;

    //best.applyBetter(sol,fitness,maxevals);
    //bestFitnes += fitness;
  }
  f++;
  problem = cec2014.get(f);
  ls->setProblem(problem.get());
  ls->setRandom(&random);
  ls_options = ls->getInitOptions(sol);
  alg.setRandom(&random);
  alg.setProblem(problem);
  alg.setILocalSearch(ls);
  alg.setLSOption(ls_options);
  //best.setRandom(&random);
  //best.setProblem(problem);
  //best.setILocalSearch(ls);
  //best.setLSOption(ls_options);
  std::cout << "Valor Normal: " << parFitness/numbersOfProof /*<< " Valor Mejora: " << bestFitnes/numbersOfProof*/ << '\n';
}

}
