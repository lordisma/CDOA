# include "CDOA.h"

using namespace realea;

tChromosomeReal CDOA::PersonalExperience(individuo X_i, tChromosomeReal &d_0){
  tChromosomeReal current_sol = X_i.value, movement = RandomMovement(), best_toFar = X_i.bestValue;
  tChromosomeReal newSol(dim), direc(dim);

  for(unsigned i = 0; i < (unsigned)dim; i++){
      newSol[i] = current_sol[i] + ((best_toFar[i] - current_sol[i]) * movement[i] * step);
      direc[i] = best_toFar[i] - current_sol[i];

      if(newSol[i] > UP[i]) newSol[i] = UP[i];
      if(newSol[i] < DW[i]) newSol[i] = DW[i];
  }

  d_0 = direc;
  return newSol;
}

tChromosomeReal CDOA::OtherExperience(tChromosomeReal X_i, individuo X_j, individuo Current, tChromosomeReal d_0 ,tChromosomeReal &d_1){
  tChromosomeReal Xj = X_j.value, C_i = Current.value, tau = RandomMovement();
  tChromosomeReal sol(dim), direc(dim);
  double beta1 = m_random->randreal(-1,1), beta11 = m_random->randreal(0,2);

  for(unsigned i = 0; i < (unsigned)dim; i++){
    direc[i] = (beta1 * d_0[i]) + ( beta11 * (Xj[i] - C_i[i]));
    sol[i] = X_i[i] + (tau[i] * step * direc[i]);

    if(sol[i] > UP[i]) sol[i] = UP[i];
    if(sol[i] < DW[i]) sol[i] = DW[i];


  }

  d_1 = direc;
  return sol;
}

tChromosomeReal CDOA::CenterCalculate(){
  tChromosomeReal sol(dim);
  double sumpar = 0.0;

  for(unsigned i=0; i < (unsigned) dim; i++){
    sumpar = 0.0;
    for(unsigned p=0; p < population.size(); p++)
      sumpar += population[p].value[i];
    sol[i] = sumpar / POPSIZE;
    if(sol[i] > UP[i]) sol[i] = UP[i];
    if(sol[i] < DW[i]) sol[i] = DW[i];
  }

  return sol;
}

tChromosomeReal CDOA::GroupThinking(tChromosomeReal X_i,individuo Current ,tChromosomeReal d_1, tChromosomeReal &d_2){
  tChromosomeReal C_i = Current.value, tau = RandomMovement();
  tChromosomeReal sol(dim), direc(dim);
  double beta22 = m_random->randreal(0,2);
  double beta2 = m_random->randreal(-1,1);

  for(unsigned i = 0; i < (unsigned)dim; i++){
    direc[i] = (beta2 * d_1[i]) + ( beta22 * (center[i] - C_i[i]));
    sol[i] = X_i[i] + (tau[i] * step * direc[i]);

    if(sol[i] > UP[i]) sol[i] = UP[i];
    if(sol[i] < DW[i]) sol[i] = DW[i];
  }

  d_2 = direc;
  return sol;
}

tChromosomeReal CDOA::LeaderMove(tChromosomeReal X_i,individuo Current ,tChromosomeReal d_2, tChromosomeReal &d_3){
  tChromosomeReal C_i = Current.value, tau = RandomMovement(), leader = population[pos_best].value;
  tChromosomeReal sol(dim), direc(dim);
  double beta33 = m_random->randreal(0,2);
  double beta3 = m_random->randreal(-1,1);

  for(unsigned i = 0; i < (unsigned)dim; i++){
    direc[i] = (beta3 * d_2[i]) + ( beta33 * (leader[i] - C_i[i]));
    sol[i] = X_i[i] + (tau[i] * step * direc[i]);

    if(sol[i] > UP[i]) sol[i] = UP[i];
    if(sol[i] < DW[i]) sol[i] = DW[i];
  }

  d_3 = direc;
  return sol;
}

tChromosomeReal CDOA::Selector(vector<tChromosomeReal> newpop){
  tChromosomeReal best;
  tFitness bestVal = m_problem->eval(newpop[0]), actual;

  for(unsigned i = 0; i < newpop.size(); i++){
    actual = m_problem->eval(newpop[i]);

    if(actual <= bestVal){
      best = newpop[i];
      bestVal = actual;
    }
  }

  return best;
}

unsigned CDOA::apply(tChromosomeReal &sol, tFitness &fitness, unsigned itera){

  InitPop();
  pos_best = position_members[0];
  unsigned iter = 0, bettermember;
  vector<tChromosomeReal> newpop;
  tChromosomeReal d0,d1,d2,d3;


  for(; iter < itera; iter++){
    step = 2 - 1.7 * (iter / (itera - 1)); //Step size compute
    pos_best = position_members[0]; //Find the best global

    center = CenterCalculate();
    for(unsigned t = 0; t < POPSIZE; t++, iter++){
      if (t != (unsigned)pos_best)
      {
        bettermember = position_members[m_random->randint(0,t)];//Devuelve un número entre 0 y t que es la posicion del mejor que t

        tChromosomeReal X_1 = PersonalExperience(population[t], d0); //First equation operator
        tChromosomeReal X_2 = OtherExperience(X_1,population[bettermember],population[t], d0,d1);
        tChromosomeReal X_3 = GroupThinking(X_2,population[t], d1,d2);
        tChromosomeReal X_4 = LeaderMove(X_3,population[t],d2,d3);
        newpop.push_back(X_1);
        newpop.push_back(X_2);
        newpop.push_back(X_3);
        newpop.push_back(X_4);

        if(m_random->rand() < PMUT){
          tChromosomeReal X_5 = Inovation(X_4);
          newpop.push_back(X_5);
        }

        population[t].value = Selector(newpop);
        population[t].fit = m_problem->eval(population[t].value);

        if(population[t].fit < population[t].bestFit){ population[t].bestFit = population[t].fit;
                                                       population[t].bestValue = population[t].value;}
      }
      else
      {
        for(unsigned i = 0; i < NUMSEABEST; i++){
          iter += m_ls->apply(m_ls_options, population[t].value, population[t].fit, MAXITERLS);

          if(population[t].bestFit > population[t].fit){
            population[t].bestFit = population[t].fit;
            population[t].bestValue = population[t].value;
          }
        }
      }

      newpop.clear();
    }

    sortIndexWithQuickSort(population, 0, population.size() - 1, position_members);

  }

  sol = population[pos_best].bestValue;
  fitness = population[pos_best].bestFit;

  return iter;
}

unsigned CDOA::applyBetter(tChromosomeReal &sol, tFitness &fitness, unsigned itera){

  InitPop();
  pos_best = position_members[0];
  unsigned iter = 0, bettermember;
  vector<tChromosomeReal> newpop;
  tChromosomeReal d0,d1,d2,d3;
  unsigned nonBest = 0, maxTolerance = POPSIZE * dim * MAXITERLS;


  for(; iter < itera; iter++){
    step = 2 - 1.7 * (iter / (itera - 1)); //Step size compute
    pos_best = position_members[0]; //Find the best global

    center = CenterCalculate();
    for(unsigned t = 0; t < POPSIZE; t++, iter++){
      if (t != (unsigned)pos_best)
      {
        bettermember = position_members[m_random->randint(0,t)];//Devuelve un número entre 0 y t que es la posicion del mejor que t

        tChromosomeReal X_1 = PersonalExperience(population[t], d0); //First equation operator
        tChromosomeReal X_2 = OtherExperience(X_1,population[bettermember],population[t], d0,d1);
        tChromosomeReal X_3 = GroupThinking(X_2,population[t], d1,d2);
        tChromosomeReal X_4 = LeaderMove(X_3,population[t],d2,d3);
        newpop.push_back(X_1);
        newpop.push_back(X_2);
        newpop.push_back(X_3);
        newpop.push_back(X_4);

        if(m_random->rand() < PMUT){
          tChromosomeReal X_5 = Inovation(X_4);
          newpop.push_back(X_5);
        }

        population[t].value = Selector(newpop);
        population[t].fit = m_problem->eval(population[t].value);

        if(population[t].fit < population[t].bestFit){ population[t].bestFit = population[t].fit;
                                                       population[t].bestValue = population[t].value;}
      }
      else
      {
        for(unsigned i = 0; i < NUMSEABEST; i++){
          iter += m_ls->apply(m_ls_options, population[t].value, population[t].fit, MAXITERLS);

          if(population[t].bestFit > population[t].fit){
            population[t].bestFit = population[t].fit;
            population[t].bestValue = population[t].value;
          }
        }
      }

      newpop.clear();
    }

    sortIndexWithQuickSort(population, 0, population.size() - 1, position_members);

    //Reinicio
    nonBest = (pos_best != position_members[0])? 0:(nonBest+1);

    if(nonBest == maxTolerance){
      nonBest = 0;

      individuo Best = population[position_members[0]];

      InitPop();

      population[POPSIZE - 1] = Best;

      sortIndexWithQuickSort(population, 0, population.size()-1, position_members);
    }

  }

  sol = population[pos_best].bestValue;
  fitness = population[pos_best].bestFit;

  return iter;
}

void CDOA::InitPop(){
  tChromosomeReal new_value(dim);
  individuo new_member;

  //Bucle de generacion de nuevas soluciones
  for(unsigned i = 0; i < POPSIZE; i++){
    getInitRandom(m_random, m_problem->getDomain(), new_value);

    new_member.bestValue = new_member.value = new_value;
    new_member.bestFit = new_member.fit = m_problem->eval(new_value);

    population.push_back(new_member);
    position_members.push_back(i);
  }

  //QuickSort para el orden de los miembros de la poblacion
  sortIndexWithQuickSort(population, 0, population.size() - 1, position_members);

}
