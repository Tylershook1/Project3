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
#include <iomanip>
#include <cmath>

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
    return isdigit(str[0]) ? std::stod(str) : 0.0;
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

int partition(std::vector<HouseInfo>& houses, int low, int high)
{
    // Select the pivot element
    double pivot = houses[low].MeanValue;
    int up = low, down = high;
    while(up < down)
    {
        for (int j = up; j < high; j++)
        {
            if (houses[up].MeanValue > pivot)
                break;
            up++;
        }
        for (int j = high; j > low; j--)
        {
            if (houses[down].MeanValue < pivot)
                break;
            down--;
        }
        if (up < down)
            std::swap(houses[up], houses[down]);
    }
    std::swap(houses[low], houses[down]);
    return down;
}

void quickSort(std::vector<HouseInfo>& houses, int low, int high)
{
    if (low < high)
    {
        int pivot = partition(houses, low, high);
        quickSort(houses, low, pivot - 1);
        quickSort(houses, pivot + 1, high);
    }
}

void quickSortTop(std::map<std::string, std::vector<HouseInfo>>& HouseData){
    auto start = std::chrono::high_resolution_clock ::now();

    for (auto& entry : HouseData){
        std::vector<HouseInfo>& houses = entry.second;
        quickSort(houses, 0, houses.size() - 1);
    }

    auto stop = std::chrono::high_resolution_clock ::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << "Quick Sort Time in Milliseconds: " <<  duration.count()/1000.0 << std::endl;
}

using namespace std;
// map<string, vector<HouseInfo>> & HouseData
map<string, vector<HouseInfo>> top5States(string title, int numStates, std::map<std::string, std::vector<HouseInfo>>& HouseData, std::map<std::string, std::vector<Occupation>> occupationData){

    ///Variables for each state
    map<string, float> advJobSalaryPerState;
    map<string, float> advHomeValuePerState;

    ///Find the advJobSalaryPerState
    std::map<std::string, std::vector<Occupation>>::iterator iter;
    for(iter = occupationData.begin(); iter != occupationData.end(); iter++){
        //cout << iter->first << endl; //iter->first is the state code
        std::vector<Occupation>::iterator stateIter;
        int total = 0, counter = 0;
        for(stateIter = occupationData[iter->first].begin(); stateIter != occupationData[iter->first].end(); stateIter++) {
            if(stateIter->OCC_TITLE == title){
                total += stateIter->A_MEAN;
                counter += 1;
            }

        }

        if(counter != 0)
            advJobSalaryPerState[iter->first] = (float)total / (float) counter;
        else
            advJobSalaryPerState[iter->first] = 0;
    }

    ///Find the advHomeValuePerState
    std::map<std::string, std::vector<HouseInfo>>::iterator homeValIter;
    for(homeValIter = HouseData.begin(); homeValIter != HouseData.end(); homeValIter++){
        int total = 0, counter = 0;
        vector<HouseInfo>::iterator houseInfoIter;
//        cout << homeValIter->first << ": ";
        for(houseInfoIter = HouseData[homeValIter->first].begin(); houseInfoIter != HouseData[homeValIter->first].end(); houseInfoIter++){
//            cout << houseInfoIter->MeanValue << " ";
            total += houseInfoIter->MeanValue;
            counter += 1;
        }
//        cout << endl << total << " | " << counter << endl;
        advHomeValuePerState[homeValIter->first] = (float)total / (float)counter;
//        cout << endl;
    }

    map<string, float> houseAdv;
    std::map<string, float>::iterator advHomeValueIter;
    for(advHomeValueIter = advJobSalaryPerState.begin(); advHomeValueIter != advJobSalaryPerState.end(); advHomeValueIter++){
        houseAdv[advHomeValueIter->first] = (float)  advJobSalaryPerState[advHomeValueIter->first] / (float) advHomeValuePerState[advHomeValueIter->first];
    }

    /// Orginise and get the top states to live in
    // Creating a vector of pairs to store key-value pairs
    std::vector<std::pair<string, float>> vectorPairs(houseAdv.begin(), houseAdv.end());

    // Sorting the vector by values
    std::sort(vectorPairs.begin(), vectorPairs.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    map<string, vector<HouseInfo>> returnData;
    int counter = 0;

    for(int i = 0; i < vectorPairs.size(); i++){
        if(counter < numStates && vectorPairs[i].second < 1) {
            returnData[vectorPairs[i].first] = HouseData[vectorPairs[i].first];
            counter++;
        }
    }

    cout << "returnData.size(): " << returnData.size() << endl;

    return returnData;
}

// Function that displays the shell sorted housing data
void displayHouseInfo(std::map<std::string, std::vector<HouseInfo>>& HouseData, std::string FileName){
    std::ofstream homeOutputFile(FileName);
    for (auto& entry : HouseData){
        std::vector<HouseInfo>& houses = entry.second;
        int n = houses.size();
        for (int i = 0; i < n; i++) {
            homeOutputFile << houses[i].RegionID << ", " << houses[i].State << ", " << houses[i].City << ", " << houses[i].CountyName << ", " << std::fixed << std::setprecision(2) << houses[i].MeanValue << std::endl;
        }
    }
    homeOutputFile.close();
}

int main()
{
    // Open input files for occupation salary data and home cost data
    std::ifstream homeCostFile("../PropertyValues.csv");
    std::ifstream OccupationDataFile("../JobSalarys.csv");

    // Check if files are successfully opened
    if (!homeCostFile.is_open())
    {
        std::cerr << "Error opening files!" << std::endl;
        return 1;
    }

    // Map to store zip code information and salary information
    std::map<std::string, std::vector<HouseInfo>> houseData;
    // houseData is sorted by shell sort, unsortedHouseData is sorted by quick sort
    // This is to ensure both sorting algorithms are being sorted on the same unsorted dataset
    std::map<std::string, std::vector<HouseInfo>> unsortedHouseData;
    std::map<std::string, std::vector<Occupation>> occupationData;
    std::map<std::string, std::string> occupationNames;

    // Read home cost data from the file
    std::string homeCostLine;
    std::getline(homeCostFile, homeCostLine);
    while (std::getline(homeCostFile, homeCostLine))
    {
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
        unsortedHouseData[StateStr].push_back(tmp);
    }

    // Read occupation data from the file
    std::string OccupationDataLine;
    std::getline(OccupationDataFile, OccupationDataLine);
    while (std::getline(OccupationDataFile, OccupationDataLine))
    {
        std::istringstream iss(OccupationDataLine);
        std::string AREA, PRIM_STATE, OCC_TITLE, S_TOT_EMP, S_A_MEAN;

        // Parse CSV fields
        std::getline(iss, AREA, ',');
        std::getline(iss, PRIM_STATE, ',');
        std::getline(iss, OCC_TITLE, ',');
        std::getline(iss, S_TOT_EMP, ',');
        std::getline(iss, S_A_MEAN, ',');

        double TOT_EMP = convertToDouble(S_TOT_EMP);
        double A_MEAN = convertToDouble(S_A_MEAN);

        Occupation tmp = Occupation(AREA, PRIM_STATE, OCC_TITLE, TOT_EMP, A_MEAN);
        occupationData[PRIM_STATE].push_back(tmp);
        occupationNames[OCC_TITLE] = OCC_TITLE;
    }

    // Close input files
    homeCostFile.close();
    OccupationDataFile.close();

    std::cout << "Welcome to Oh, the places you can go!" << std::endl;
    std::cout
            << "This program will match you with the top 20 homes most suitable for you based on occupation and desired state to live in"
            << std::endl;
    std::cout << "Below is the list of options: " << std::endl;

    // Prints list of choices of occupations
    for (auto i = occupationNames.begin(); i != occupationNames.end(); i++)
    {
        std::cout << i->second << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Please enter an occupation to search for" << ": ";
    // Prompt the user to enter a keyword to search for to filter the OCC_TITLE
    std::string keyword;// = "Computer";
    std::cin >> keyword;

    std::set<std::string> matchingTitles = searchOccupations(occupationData, keyword);

    //for(const auto& title:matchingTitles){
    // std::cout << title << std::endl;
    // }

    std::cout << std::endl;
    if (!matchingTitles.empty())
    {
        size_t count = 1;
        for (const auto &title: matchingTitles)
        {
            std::cout << count << ". " << title << '\n';
            count++;
        }
        std::cout << std::endl;

        // Prompt the user to select a number corresponding to OCC_TITLE
        std::cout << "Select the number corresponding to the occupation: ";
        size_t selectedNumber;//= 1;
        std::cin >> selectedNumber;

//        std::cout << "fslksdkls" << std::endl;

        // Validate the user input
        if (selectedNumber > 0 && selectedNumber <= matchingTitles.size())
        {
            auto it = matchingTitles.begin();
            std::advance(it, selectedNumber - 1);
            std::string selectedTitle = *it;
            std::cout << selectedTitle << std::endl;


            // return best cost of living for the top 5 states
//            cout << selectedTitle << endl;
            map<string, vector<HouseInfo>> topStates = top5States(selectedTitle, 5, houseData, occupationData);


            // user selects a state


            // print lowest priced A_MEAN areas for houses from the selected state

            std:: cout << std::endl;
            // Search using shell sort
            shellSort(houseData);
            //displayHouseInfo(houseData, "../shellSortedFile.txt");

            // Search using Quicksort
            quickSortTop(unsortedHouseData);
            // displayHouseInfo(houseData, "../quickSortedFile.txt");

        }
    }
    else
    {
        std::cout << "No Occupation found. Rerun program." << std::endl;
    }
    return 0;
}