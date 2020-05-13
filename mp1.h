#ifndef MP1_H
#define DATE_H
#include <unordered_map>
#include <iterator>
#include <set>
#include <vector>
class MP1
    {
        private:   
            std::unordered_map<std::string, int> pos;
            std::unordered_map<std::string, int> neg;
            double totPWord = 0;
            double totNWord = 0;

            double totalPos = 0;
            double totalNeg = 0;
            double unique = 0;

            std::set<std::string> stopWords;

            double testTime = 0;
            double testAcc = 0;

            double trainTime = 0;
            double trainAcc = 0;

            double smoothing = 1;

        public:
            void read(std::string train, std::string test);

            void trainData(std::string train);
            void readStopText();
            void trainHelper(std::string rate, std::string line);

            std::vector<double> testData(std::string test);
            bool testHelper(std::string rate, std::string line);
            void printBigram();
            double printBigramHelper(std::string word, std::string label);

    };



#endif