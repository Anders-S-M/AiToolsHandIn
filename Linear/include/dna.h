#ifndef DNA_H
#define DNA_H

#include <vector>

class DNA
{
    private:
        std::mt19937 generator;
        std::uniform_int_distribution<int> uniform_distribution;
        std::normal_distribution<double> normal_distribution;

    public:
        std::vector<float> genes;
        float fitness;

    public:
        DNA()
        {
            std::random_device rd;
            generator = std::mt19937(rd());

            fitness = -1;
        }

        DNA(std::vector<float> _genes)
        {
            std::random_device rd;
            generator = std::mt19937(rd());

            genes = _genes;
            fitness = -1;
        }

        void mutate(float rate)
        {
            // For each gene
            for(size_t gene = 0; gene < genes.size(); gene++)
            { 
                // Decide if mutation occurs
                uniform_distribution = std::uniform_int_distribution<int>(0, 10000);
                float mutation = uniform_distribution(generator)/10000.0;

                if(mutation <= rate)
                {
                    // Get mutation amount
                    normal_distribution = std::normal_distribution<double>(0.0, 0.5);
                    double amount = normal_distribution(generator);

                    // Mutate gene
                    genes[gene] += amount;
                }
            }
        }

        void randomize(int range)
        {
            // Get distribution range
            int dist_range = (range/2)*1000;

            // Get mutation amount
            uniform_distribution = std::uniform_int_distribution<int>(-dist_range, dist_range);
            
            // For each gene
            for(size_t gene = 0; gene < genes.size(); gene++)
            {
                // Ranomize gene
                float value = uniform_distribution(generator)/1000.0;
                genes[gene] = value;
            }
        }
        void print()
        {
            // Print array-style output
            std::cout << "{ ";
            for(size_t i = 0; i < genes.size(); i++)
                std::cout << genes[i] << ", ";
            std::cout << "\b\b }" << std::endl;
        }
};

#endif
