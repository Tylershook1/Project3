#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <limits>
#include <chrono>

// Class representing an occupation with relevant information
class Occupation {
public:
    std::string AREA;
    std::string PRIM_STATE;
    std::string OCC_TITLE;
    double TOT_EMP;
    double A_MEAN;

    Occupation(const std::string& area, const std::string& prim_state, const std::string& occ_title, double tot_emp, double a_mean)
            : AREA(area), PRIM_STATE(prim_state), OCC_TITLE(occ_title), TOT_EMP(tot_emp), A_MEAN(a_mean) {}
};


// Class representing information about a zip code, including home cost and associated occupations
class HouseInfo {
public:
    std::string RegionID;
    std::string State;
    std::string City;
    std::string CountyName;
    double MeanValue;

    HouseInfo(const std::string& regionID, const std::string& state, const std::string& city, const std::string& countyName, double meanValue)
            : RegionID(regionID), State(state), City(city), CountyName(countyName), MeanValue(meanValue) {}

    HouseInfo(double cost = 0.0) : MeanValue(cost) {}

    bool operator>(const HouseInfo& other) const{
        return MeanValue > other.MeanValue;
    }
};

// Function to check if a string contains only numeric characters
bool isNumeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

// Function to convert a string to double, handling non-numeric cases
double convertToDouble(const std::string& str) {
    return isNumeric(str) ? std::stod(str) : 0.0;
}

std::set<std::string> searchOccupations(const std::map<std::string, std::vector<Occupation>> occupationData, const std::string& keyword) {
    std::set<std::string> matchingTitles;

    for(const auto& state:occupationData){
        for(const auto & i : state.second){
            if(!(i.OCC_TITLE.find(keyword))){
                matchingTitles.insert(i.OCC_TITLE);
            }
        }
    }


    return matchingTitles;
}

void shellSort(std::map<std::string, std::vector<HouseInfo>>& HouseData){
    auto start = std::chrono::high_resolution_clock ::now();

    for(auto& entry : HouseData){
        std::vector<HouseInfo>& houses = entry.second;
        int n = houses.size();

        for(int gap = n/2; gap > 0; gap/=2){
            for(int i = gap; i < n; i++){
                HouseInfo tmp = houses[i];
                int j;
                for(j = i; j >=gap && houses[j - gap] > tmp; j -=gap){
                    houses[j] = houses[j - gap];
                }
                houses[j] = tmp;
            }
        }
    }

    auto stop = std::chrono::high_resolution_clock ::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << "Shell Sort Time in Milliseconds: " <<  duration.count()/1000.0 << std::endl;
}


int main() {
    // Open input files for occupation salary data and home cost data
    std::ifstream homeCostFile("../PropertyValues.csv");
    std::ifstream OccupationDataFile("../JobSalarys.csv");

    // Check if files are successfully opened
    if (!homeCostFile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return 1;
    }

    // Map to store zip code information and salary information
    std::map<std::string, std::vector<HouseInfo>> houseData;
    std::map<std::string, std::vector<Occupation>> occupationData;




    // Read home cost data from the file
    std::string homeCostLine;
    std::getline(homeCostFile, homeCostLine);
    while (std::getline(homeCostFile, homeCostLine)) {
        std::istringstream iss(homeCostLine);
        std::string RegionIDStr, StateStr, CityStr, CountyNameStr, MeanValueStr;

        // Parse CSV fields
        std::getline(iss, RegionIDStr, ',');
        std::getline(iss, StateStr, ',');
        std::getline(iss, CityStr, ',');
        std::getline(iss, CountyNameStr, ',');
        std::getline(iss, MeanValueStr, ',');

        // Convert string values to appropriate types
        double MeanValue = convertToDouble(MeanValueStr);

        HouseInfo tmp = HouseInfo(RegionIDStr, StateStr, CityStr, CountyNameStr, MeanValue);
        houseData[StateStr].push_back(tmp);

    }


    // Read occupation  data from the file
    std::string OccupationDataLine;
    std::getline(OccupationDataFile, OccupationDataLine);
    while (std::getline(OccupationDataFile, OccupationDataLine)) {
        std::istringstream iss(OccupationDataLine);
        std::string AREA, PRIM_STATE, OCC_TITLE, S_TOT_EMP, S_A_MEAN;

        //ParseCSVfields
        std::getline(iss, AREA, ',');
        std::getline(iss, PRIM_STATE, ',');
        std::getline(iss, OCC_TITLE, ',');
        std::getline(iss, S_TOT_EMP, ',');
        std::getline(iss, S_A_MEAN, ',');

        double TOT_EMP = convertToDouble(S_TOT_EMP);
        double A_MEAN = convertToDouble(S_A_MEAN);

        Occupation tmp = Occupation(AREA, PRIM_STATE, OCC_TITLE, TOT_EMP, A_MEAN);
        occupationData[PRIM_STATE].push_back(tmp);

    }


    // Close input files
    homeCostFile.close();
    OccupationDataFile.close();


    // Prompt the user to enter a keyword to search for to filter the OCC_TITLE
    std::string keyword;
    std::cout << "Please enter a keyword to search for.";
    std::cin >> keyword;


    std::set<std::string> matchingTitles = searchOccupations(occupationData,keyword);

    for(const auto& title:matchingTitles){
        std::cout << title << std::endl;
    }


    size_t count=1;
    for(const auto& title:matchingTitles){
        std::cout<<count<<"."<<title<<'\n';
        count++;
    }

//    Prompt the user to select a number corresponding to OCC_TITLE
    std::cout<<"Select a number corresponding to the OCC_TITLE:";
    size_t selectedNumber;
    std::cin >> selectedNumber;


//    Validate the user input
    if(selectedNumber > 0 && selectedNumber <= matchingTitles.size()) {
        auto it = matchingTitles.begin();
        std::advance(it, selectedNumber - 1);
        std::string selectedTitle = *it;

        std::cout << selectedTitle << std::endl;


        // return best cost of living for the top 5 states


        // user selects a state


        // print lowest priced A_MEAN areas for houses from the selected state

        // Search using shell sort
        shellSort(houseData);


        // Search using Quicksort


    }
    return 0;
}
