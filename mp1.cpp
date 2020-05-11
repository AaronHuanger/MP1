#include "mp1.h"
#include <iostream>
#include <sstream>
#include <fstream> 
#include <bits/stdc++.h>
#include <math.h>

int main(int argc, char *argv[]){

    if(argc < 2){
        std::cerr << "Please input 2 text files" << std::endl;
    }
    MP1 mp1;
    mp1.read(argv[1], argv[2]);
}

void MP1::readStopText(){ //reads the stopwords.txt 
    std::ifstream file; 
    
    file.open("stopwords.txt");
    std::string temp;
    getline(file,temp, '%');

    while(getline(file,temp)){
        stopWords.insert(temp);
    }

    file.close();
}
void MP1::read(std::string train, std::string test){
    std::vector<double> info;
    time_t start, end;

    time(&start);
    readStopText(); //makes the stop word text set
    trainData(train);
    info = testData(train);
    time(&end);
    trainAcc = info[info.size()-1];
    trainTime = double(end - start);

    time(&start);
    info = testData(test);
    time(&end);
    testTime = double(end - start);

    for(size_t i = 0; i < info.size()-1;i++){
        std::cout << info[i] << "\n";
    }
    testAcc = info[info.size()-1];
    
    std::cout << trainTime << " seconds (training)" << std::endl;
    std::cout << testTime << " seconds (labeling)" <<std::endl;

    std::cout.setf(std::ios::fixed,std::ios::floatfield);
    std::cout.precision(3);
s
    std::cout << trainAcc << " (training)" << std::endl;
    std::cout << testAcc << " (testing)" << std::endl;


    /*std::cout << "totPWord: " << totPWord <<std::endl;
    std::cout << "totNWord: " << totNWord <<std::endl;

    std::cout << "totalPos: " << totalPos << std::endl;
    std::cout << "totalNeg: " << totalNeg << std::endl;

    std::cout << "posMapSize: " << pos.size() << std::endl;
    std::cout << "negMapSize: " << neg.size() << std::endl; */
}

void MP1::trainData(std::string train){
    std::ifstream file;
    file.open(train);
    std::string line;

    if(file.is_open()){
        while(getline(file, line)){
            if(line.substr(line.find(",") + 1, 1) == "0"){
                trainHelper("0", line.substr(0,line.find(",")) + " ");
            }else{
                trainHelper("1", line.substr(0,line.find(",")) + " ");
            }
        }
        file.close();
    }
}

void MP1::trainHelper(std::string rate, std::string line){
    std::unordered_map<std::string, int>* data;

    double* docCount; 
    if(rate == "1"){
        totalPos++;
        data = &pos;
        docCount = &totPWord;
    }else{
        totalNeg++;
        data = &neg;
        docCount = &totNWord;
    }
    
    std::istringstream ss(line);
    std::string token = " ";

    while(getline(ss, token, ' ')){
        if(token != " "){
            if((stopWords.find(token) == stopWords.end()) && data->find(token) == data->end()){
                data->insert(std::pair<std::string, int>(token, 1));
                *docCount += 1;
            }else if(data->find(token) != data->end()){
                data->at(token)++;
                *docCount += 1;
            }else{
                //does nothing with the stop words. Stop words are completely ignored. 
            }
        }
    }
}

std::vector<double> MP1::testData(std::string test){
    std::vector<double> info;
    info.resize(0);
    std::ifstream file;
    file.open(test);

    double wrong = 0;
    double right = 0;
    std::string line;

    bool labelIsNeg;
    if(file.is_open()){
        while(getline(file, line)){
            if(line.substr(line.find(",") + 1, 1) == "0"){
                labelIsNeg = testHelper("0", line.substr(0,line.find(",")) + " ");
                if(labelIsNeg){
                    right++;
                    info.push_back(0);
                }else{
                    wrong++;
                    info.push_back(1);
                }
            }else{
                labelIsNeg = testHelper("1", line.substr(0,line.find(",")) + " ");
                if(labelIsNeg){
                    wrong++;
                    info.push_back(0);
                }else{
                    right++;
                    info.push_back(1);
                }
            }
        }
        info.push_back(right/(right+wrong));
        file.close();
    }
    return info;
}

bool MP1::testHelper(std::string rate, std::string line){
    //int noFindCount;

    double posProb = 0;
    double negProb = 0;

    double pNeg = totalNeg/(totalPos + totalNeg); //P(Cneg)
    double pPos = totalPos/(totalPos + totalNeg); //P(Cpos)

    double pWord = 0; // P(D | C)
    
    std::istringstream ss(line);
    std::string token = " ";

    
    while(getline(ss, token, ' ')){
        //if(()){
        if(token != " "){
            if(stopWords.find(token) == stopWords.end() && pos.find(token) == pos.end()){
                pWord = (smoothing)/(totPWord + (pos.size()*smoothing));
            }else{
                pWord = (pos.at(token)+smoothing)/(totPWord + (pos.size()*smoothing));
            }
            posProb += log(pWord);
            //posProb *= pWord*pPos;
            if(neg.find(token) == neg.end()){
                pWord = (smoothing)/(totNWord + (neg.size()*smoothing));
            }else{
                pWord = (neg.at(token)+1)/(totNWord + (neg.size()*smoothing));
            }
            negProb += log(pWord);
            //negProb *= pWord*pNeg;
        //}
        }
    }
    negProb += log(pNeg);
    posProb += log(pPos);
    
    return (negProb > posProb);

}
/*bool MP1::testHelper(std::string rate, std::string line){
    //int noFindCount;

    int posProb = 0;
    int negProb = 0;
    
    int posMatch = 0;
    int negMatch = 0;
    
    std::istringstream ss(line);
    std::string token;

    while(getline(ss, token, ' ')){
        if((stopWords.find(token) == stopWords.end()) && pos.find(token) == pos.end()){
            //noFindCount += fakeConst;
            posProb += log(fakeConst/(totalPos + fakeConst));
        }else if(pos.find(token) != pos.end()){
            posProb += log(pos.at(token)/totalPos);        
            posMatch++;
        }else{
            //does nothing with the stop words. Stop words are completely ignored. 
        }
    }
    while(getline(ss, token, ' ')){
        if((stopWords.find(token) == stopWords.end()) && neg.find(token) == neg.end()){
            //noFindCount += fakeConst;
            negProb += log(fakeConst/(totalNeg + fakeConst));
        }else if(neg.find(token) != neg.end()){
            negProb += log(neg.at(token)/totalNeg);        
            negMatch++;
        }else{
            //does nothing with the stop words. Stop words are completely ignored. 
        }
    }

    if(std::max(negMatch, posMatch) == negMatch){
        if(rate == "0"){
            return true;
        }else{
            return false;
        }
    }else{
        if(rate == "1"){
            return true;
        }else{
            return false;
        }
    }
}*/