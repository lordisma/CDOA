# ifndef __CDOA__
# define __CDOA__

#include "problemcec2014.h"
#include "problem.h"
#include "solis.h"
#include "simplex.h"
#include "cmaeshan.h"
#include "random.h"
#include "srandom.h"
#include "domain.h"
#include "localsearch.h"
#include <iostream>

# define POPSIZE 30 // Size of the population vector
# define PMUT    0.01 // Probability of the mutation
# define NUMSEABEST 6 // Number of search of the best
# define MAXITERLS 30 // Numbers iterations of localsearch

namespace realea{


  /**
  *  \author Daniel Molina
  *  \brief Random solution for the starting
  *  \param The random generator, the domain problem and the vector where the
  *         solution will be saved
  */
  void getInitRandom(Random *random, DomainRealPtr domain, tChromosomeReal &crom) {
    tReal min, max;

    for (unsigned i = 0; i < crom.size(); ++i) {
        domain->getValues(i, &min, &max, true);
        crom[i] = random->randreal(min, max);
    }

  }

  /**
  * \author Ismael Marin Molina marinmolina@correo.ugr.es
  * \brief Implementation of Collective Decision Optimization Algorithm
  *        based in the paper develop by ---------- and ----------
  */

  class CDOA{
  public:
    CDOA(int dimensio):dim(dimensio){}

    /**
    * \author Ismael Marin Molina
    * \brief class for member Population
    *        representation
    */
    typedef struct {
      tFitness fit;
      tChromosomeReal value;

      tChromosomeReal bestValue;
      tFitness bestFit;

    } individuo;

    /**
    * \author Ismael Marin Molina
    * \brief First operator, personal based experience,
    *        the current individuo change to a new vector
    *        based in the personal experience.
    * \param The individuo
    */
    tChromosomeReal PersonalExperience(individuo X_i, tChromosomeReal &d_0);

    /**
    * \author Ismael Marin Molina
    * \brief Second operator, Other based experience,
    *        the current individuo is changed following
    *        a better individuo in the population
    * \param The chromosome of first operator,
    *         the new individuo,
    *         the actual individuo,
    *         the before direction
    */
    tChromosomeReal OtherExperience(tChromosomeReal X_i, individuo X_j, individuo Current,tChromosomeReal d_0, tChromosomeReal &d_1);

    /**
    * \author Ismael Marin Molina
    * \brief Calculate the center of
    *        the population
    */
    tChromosomeReal CenterCalculate();

    /**
    * \author Ismael Marin Molina
    * \brief Third operator, Other Group Thinking,
    *        the current individuo is changed following
    *        the center of the population.
    * \param The chromosome of first operator,
    *         the actual individuo,
    *         the before direction
    */
    tChromosomeReal GroupThinking(tChromosomeReal X_i,individuo Current ,tChromosomeReal d_1, tChromosomeReal &d_2);

    /**
    * \author Ismael Marin Molina
    * \brief Forth operator, Leadership,
    *        the current individuo is changed following
    *        the leader of the population.
    * \param The chromosome of first operator,
    *         the actual individuo,
    *         the before direction
    */
    tChromosomeReal LeaderMove(tChromosomeReal X_i,individuo Current ,tChromosomeReal d_2, tChromosomeReal &d_3);

    /**
    * \author Ismael Marin Molina
    * \brief Select the best chromosome of the
    *        vector and return this
    * \param A vector of tChromosomeReal
    */
    tChromosomeReal  Selector(vector<tChromosomeReal> newpop);

    /**
    * \author Ismael Marin Molina
    * \brief The last operator, the inovation operator
    *        is the same that the mutation operator
    */
    tChromosomeReal Inovation(tChromosomeReal X_i){
      int index = m_random->randint(0,dim-1);

      X_i[index] = DW[index] + m_random->rand() * (UP[index] - DW[index]);

      return X_i;
    }

    /**
    * \author Ismael Marin Molina
    * \brief Apply the CDOA algorithm about the problem
    * \param The initial solution, the fitness of this solution and the
    *        max number of iteration
    */
    unsigned apply(tChromosomeReal &sol, tFitness &fitness, unsigned itera);
    unsigned applyBetter(tChromosomeReal &sol, tFitness &fitness, unsigned itera);

    /**
    * \author Ismael Marin Molina
    * \brief Initialized the population
    */
    void InitPop();

    // set the value of random generator
    void setRandom(Random *random) {
       m_random = random;
    }

    // set the evaluation function
    void setProblem(ProblemPtr &problem) {
      m_problem = problem;

      m_domain = m_problem->getDomain();

      if(!UP.empty()) UP.clear();
      if(!DW.empty()) DW.clear();

      tReal min,max;

      for(unsigned i = 0; i < (unsigned)dim; i++){
        m_domain->getValues(i, &min, &max, true);
        UP.push_back(max);
        DW.push_back(min);
      }
    }

    // set the Local Search
    void setILocalSearch (ILocalSearch *ls){
      m_ls = ls;
    }

    void setLSOption( ILSParameters *ls_options){
      m_ls_options = ls_options;
    }


  private:
    vector<individuo> population;
    vector<int> position_members;

    Random *m_random; /*current random generator*/
    ProblemPtr m_problem; /*Current problem*/
    DomainRealPtr m_domain; /*Current domain*/

    ILocalSearch *m_ls; /*Used Local Search*/
    ILSParameters *m_ls_options; /*Parameters for the local search*/
    int dim;
    int step;

    //Necesario para la creacion
    int pos_best;

    //Upper and Down bounds
    vector<tReal> UP, DW;
    //The center of the population
    tChromosomeReal center;



  private:
    /**
    * \author Ismael Marin Molina
    * \brief QuickSort algorithm for the ordenation of the
    *        index population
    *
    */
    void sortIndexWithQuickSort( vector<individuo> & array, int first, int last, vector<int>& index) {
      individuo x = array[index[ceil((first + last) / 2)]];
      int i = first;
      int j = last;
      individuo temp_var;
      int temp_num;

      while (true) {
        while (array[index[i]].fit < x.fit) i++;
        while (x.fit < array[index[j]].fit) j--;
        if (i >= j) break;

        temp_num = index[i];
        index[i] = index[j];
        index[j] = temp_num;

        i++;
        j--;
      }

      if (first < (i -1)) sortIndexWithQuickSort(array, first, i - 1, index);
      if ((j + 1) < last) sortIndexWithQuickSort(array, j + 1, last, index);
    }

    //Function that give a Randon vector
    tChromosomeReal RandomMovement(){
      tChromosomeReal randmon(dim);

      for (unsigned i = 0; i < randmon.size(); ++i) {
        randmon[i] = m_random->rand();
      }

      return randmon;
    }

  };

}



#endif
