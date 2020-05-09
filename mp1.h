#ifndef MP1_H
#define DATE_H
#include <map>
#include <iterator>
#include <set>
#include <vector>
class MP1
    {
        private:   
            std::map<std::string, int> pos;
            std::map<std::string, int> neg;
            int totalPos = 0;
            int totalNeg = 0;             
            std::set<std::string> stopWords;

            double testTime = 0;
            double testAcc = 0;

            double trainTime = 0;
            double trainAcc = 0;

            int fakeConst = 1;

        public:
            void read(std::string train, std::string test);

            void trainData(std::string train);
            void readStopText();
            void trainHelper(std::string rate, std::string line);

            std::vector<double> testData(std::string test);
            bool testHelper(std::string rate, std::string line);

    };



#endif