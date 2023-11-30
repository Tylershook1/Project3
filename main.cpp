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
using namespace std;


// Class representing an occupation with relevant information
class Occupation {
public:
    std::string AREA;
    std::string PRIM_STATE;
    std::string OCC_TITLE;
    double TOT_EMP;
    double A_MEAN;

    bool operator>(const Occupation& other) const{
        return A_MEAN > other.A_MEAN;
    }

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

template <typename T>
void countRecords(std::map<std::string, std::vector<T>>& data){
    size_t totalVectorsinSalary = 0;
    size_t totalEntriesinSalary = 0;

    for (const auto& entry : data) {
        // Increment the number of vectors
        totalVectorsinSalary++;

        // Increment the number of entries by the size of the vector
        totalEntriesinSalary += entry.second.size();
    }

    std::cout << std::endl << "Total number of vectors in  Data: " << totalVectorsinSalary << std::endl;
    std::cout << "Total number of entries in Data: " << totalEntriesinSalary << std::endl;
}


// Define a template function for shell sorting
template <typename T>
void shellSortData(std::map<std::string, std::vector<T>>& data) {
    auto start = std::chrono::high_resolution_clock::now();

    for (auto& entry : data) {
        std::vector<T>& dataSet = entry.second;
        int n = dataSet.size();

        for (int gap = n / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < n; i++) {
                T tmp = dataSet[i];
                int j;
                for (j = i; j >= gap && dataSet[j - gap] > tmp; j -= gap) {
                    dataSet[j] = dataSet[j - gap];
                }
                dataSet[j] = tmp;
            }
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Shell Sort Time in Milliseconds: " << duration.count() / 1000.0 << std::endl;
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
    std::cout << "Quick Sort Time on Housing Data in Milliseconds: " <<  duration.count()/1000.0 << std::endl;
}

// map<string, vector<HouseInfo>> & HouseData
void top5States(
        std::string title,
        int numStates,
        std::map<std::string, std::vector<HouseInfo>>& HouseData,
        std::map<std::string, std::vector<Occupation>> occupationData
) {
    // Variables to store average job salary and average home value per state
    std::map<std::string, float> advJobSalaryPerState;
    std::map<std::string, float> advHomeValuePerState;

    // Calculate average job salary per state
    for (auto iter = occupationData.begin(); iter != occupationData.end(); ++iter) {
        int total = 0, counter = 0;
        for (const auto& occupation : iter->second) {
            if (occupation.OCC_TITLE == title) {
                total += occupation.A_MEAN;
                counter += 1;
            }
        }

        // Calculate and store the average job salary for the state
        advJobSalaryPerState[iter->first] = (counter != 0) ? static_cast<float>(total) / counter : 0;
    }

    // Calculate average home value per state
    for (auto iter = HouseData.begin(); iter != HouseData.end(); ++iter) {
        int total = 0, counter = 0;
        for (const auto& houseInfo : iter->second) {
            total += houseInfo.MeanValue;
            counter += 1;
        }

        // Calculate and store the average home value for the state
        advHomeValuePerState[iter->first] = (counter != 0) ? static_cast<float>(total) / counter : 0;
    }

    // Calculate the advantage score for each state
    std::map<std::string, float> houseAdv;
    for (const auto& entry : advJobSalaryPerState) {
        houseAdv[entry.first] = (entry.second != 0) ? (advJobSalaryPerState[entry.first]- (advHomeValuePerState[entry.first]/30.0)) : 0;
    }

    // Sort the states based on the advantage score in descending order
    std::vector<std::pair<std::string, float>> vectorPairs(houseAdv.begin(), houseAdv.end());
    std::sort(vectorPairs.begin(), vectorPairs.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });


    // Select the top states and store their information
    for (int i = 0; i < 5; ++i) {
            const std::string& state = vectorPairs[i].first;
            float jobSalary = advJobSalaryPerState[state];
            float homeValue = advHomeValuePerState[state];
            const std::vector<HouseInfo>& houseInfoVec = HouseData[state];

            std::cout << "State: " << state << std::endl;
            std::cout << "  Average Job Salary: " << jobSalary << std::endl;
            std::cout << "  Average Home Value: " << homeValue << std::endl;
            std::cout << "  Average Monthly Payment: " << (homeValue/360.0) << std::endl;
            std::cout << "  Difference in Job Salary and Yearly Mortgage Payments: " << (jobSalary - (homeValue/30.0)) << std::endl;

    }
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
            top5States(selectedTitle, 5, houseData, occupationData);



            countRecords(unsortedHouseData);
            std:: cout << std::endl;
            // Search using shell sort
            shellSortData(houseData);

            // Search using Quicksort
            quickSortTop(unsortedHouseData);



            countRecords(occupationData);
            std:: cout << std::endl;
            // Search using shell sort
            shellSortData(occupationData);

        }
    }
    else
    {
        std::cout << "No Occupation found. Rerun program." << std::endl;
    }
    return 0;
}