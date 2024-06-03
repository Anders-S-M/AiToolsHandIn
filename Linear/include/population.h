#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "dna.h"

class Population
{
    public:
        DNA fittest1;
        DNA fittest2;

        std::vector<DNA> members;
    private: 
        std::mt19937 generator;
        std::uniform_int_distribution<int> distribution;

    public:
        Population()
        {
            std::random_device rd;
            generator = std::mt19937(rd());
        }

        size_t getSize()
        {
            return members.size();
        }

        void mutate(float rate)
        {
            for(size_t i = 0; i < members.size(); i++)
            {
                members[i].mutate(rate);
            }
        }

        void selection()
        {
            // NOTE: Roulette implementaion
            
            // Get total fitness
            float sum = 0;
            for(size_t i = 0; i < members.size(); i++)
                sum += members[i].fitness;
            
            // Get two points on wheel
            distribution = std::uniform_int_distribution<int>(0, (int) sum);
            int point1 = distribution(generator);
            int point2 = distribution(generator);

            // Find most fit
            sum = 0;
            for(size_t i = 0; i < members.size(); i++)
            {
                sum += members[i].fitness;
                if(sum >= point1)
                {
                    fittest1 = members[i];
                    members[i]. fitness = -1;
                    break;
                }
            }
            
            // Find second most fit
            sum = 0;
            for(size_t i = 0; i < members.size(); i++)
            {
                sum += members[i].fitness;
                if(sum >= point2)
                {
                    fittest2 = members[i];
                    members[i].fitness = -1;
                    break;
                }
            }
        }
        
        DNA get_fittest(){
            DNA true_fittest = members[0];
            for(int i = 0; i < members.size(); i++){
                if(members[i].fitness > true_fittest.fitness){
                    true_fittest = members[i];
                }
            }
            // printf("Fittest: %f\n", true_fittest.fitness);
            return true_fittest;
        }

        void random(size_t size, DNA root)
        {
            // Reset population
            reset();

            // Fill population with random dna (based on root)
            for(size_t i = 0; i < size; i++)
            {
                // Create member from root genes
                DNA member(root.genes);

                // Randomize dna genes (from -10 to 10);
                member.randomize(20);

                // Add dna to population
                members.push_back(member);
            }
        }

        void crossoverUniform(float mutation_rate)
        {
            // Chromosome size
            size_t size = fittest1.genes.size();
            // printf("Size: %d\n", size);

            // Generate uniform random bit mask
            std::vector<int> mask;
            distribution = std::uniform_int_distribution<int>(0, 1);
            for(size_t i = 0; i < size; i++)
                mask.push_back(distribution(generator));

            // Generate 2 children
            DNA child1;
            DNA child2;
            for(size_t i = 0; i < size; i++)
            {
                if(mask[i])
                {
                    child1.genes.push_back(fittest1.genes[i]);
                    child2.genes.push_back(fittest2.genes[i]);
                }
                else
                {
                    child1.genes.push_back(fittest2.genes[i]);
                    child2.genes.push_back(fittest1.genes[i]);
                }
            }

            // Mutate children
            child1.mutate(mutation_rate);
            child2.mutate(mutation_rate);
            
            // Return child
            members.push_back(child1);
            members.push_back(child2);
        }
        
        void elimination(size_t count)
        {
            // Kill x worst individuals
            for(size_t dead = 0; dead < count; dead++)
            {
                float worst = members[0].fitness;
                float index = 0;

                for(size_t i = 1; i < members.size(); i++)
                {
                    if(members[i].fitness < worst)
                    {
                        worst = members[i].fitness;
                        index = i;
                    }
                }
                members.erase(members.begin()+index);
            }
        }

        float avgFitness()
        {
            float sum = 0;
            for(size_t i = 0; i < members.size(); i++)
                sum += members[i].fitness;
                // printf("Fitness: %f\n", members[i].fitness);
            return sum/members.size();
        }

        void reset()
        {
            members.clear();
        }

        void print()
        {
            for(size_t i = 0; i < members.size(); i++)
            {
                std::cout << "(" << (members[i].fitness)*100.0 << "%)\t";
                members[i].print();
            }
        }
};

#endif
