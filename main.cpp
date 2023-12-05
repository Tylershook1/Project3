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

    bool operator<(const Occupation& other) const{
        return A_MEAN < other.A_MEAN;
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

    bool operator<(const HouseInfo& other) const{
        return MeanValue < other.MeanValue;
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

// Function to search if selected occupation is a keyword
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

// Function that returns number of data points in each dataset to distinguish sorts
// 100092 = Occupation Data, 26261 = House Data
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

    std::cout << std::endl << "Total number of vectors in Data: " << totalVectorsinSalary << std::endl;
    std::cout << "Total number of entries in Data: " << totalEntriesinSalary << std::endl;
}


// Define a template function for shell sorting
// Gap is n/2
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

// Function used in quicksort to divide array (less than pivot on left, greater than on right)
template <typename T>
int partition(std::vector<T>& data, int low, int high)
{
    // Select the pivot element
    T pivot = data[low];
    int up = low, down = high;
    while (up < down)
    {
        for (int j = up; j < high; j++)
        {
            if (data[up] > pivot)
                break;
            up++;
        }
        for (int j = high; j > low; j--)
        {
            if (data[down] < pivot)
                break;
            down--;
        }
        if (up < down)
            std::swap(data[up], data[down]);
    }
    std::swap(data[low], data[down]);
    return down;
}

// Function to perform quicksort algorithm with pivot as last element
template <typename T>
void quickSort(std::vector<T>& data, int low, int high)
{
    if (low < high)
    {
        int pivot = partition(data, low, high);
        quickSort(data, low, pivot - 1);
        quickSort(data, pivot + 1, high);
    }
}

// Define a template function for quick sorting
template <typename T>
void quickSortTop(std::map<std::string, std::vector<T>>& data){
    auto start = std::chrono::high_resolution_clock ::now();

    for (auto& entry : data){
        std::vector<T>& dataSet = entry.second;
        quickSort(dataSet, 0, dataSet.size() - 1);
    }

    auto stop = std::chrono::high_resolution_clock ::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << "Quick Sort Time in Milliseconds: " <<  duration.count()/1000.0 << std::endl;
}

// Function that displays the shell sorted housing data to confirm it works
// Mainly used for debugging
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

// Function to find the top 5 best cost of living states
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
    std::map<std::string, std::vector<Occupation>> unsortedOccupationData;
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
        unsortedOccupationData[PRIM_STATE].push_back(tmp);
        occupationNames[OCC_TITLE] = OCC_TITLE;
    }

    // Close input files
    homeCostFile.close();
    OccupationDataFile.close();

    std::cout << "Welcome to Oh, the places you can go!" << std::endl;
    std::cout
            << "This program will match you with the top 5 areas most suitable for you to reside in based on occupation and salary"
            << std::endl;
    std::cout << "Below is the list of options: " << std::endl;

    // Prints list of choices of occupations
    for (auto i = occupationNames.begin(); i != occupationNames.end(); i++)
    {
        std::cout << i->second << std::endl;
    }

    std::cout << std::endl;
    bool loop = true;
    while(loop){

    std::cout << "Please enter an occupation to search for" << ": ";
    // Prompt the user to enter a keyword to search for to filter the OCC_TITLE
    std::string keyword;
    std::cin >> keyword;

    std::set<std::string> matchingTitles = searchOccupations(occupationData, keyword);
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
        size_t selectedNumber;
        std::cin >> selectedNumber;

        // Validate the user input
        if (selectedNumber > 0 && selectedNumber <= matchingTitles.size())
        {
            auto it = matchingTitles.begin();
            std::advance(it, selectedNumber - 1);
            std::string selectedTitle = *it;
            std::cout << selectedTitle << std::endl;

            // return best cost of living for the top 5 states
            std::cout << std::endl;
            std::cout << "Here are the top choices for you:" << std::endl;
            std::cout << std::endl;
            top5States(selectedTitle, 5, houseData, occupationData);

            // Return number of data points for house data
            countRecords(unsortedHouseData);
            std:: cout << std::endl;

            // Search using shell sort on house data
            shellSortData(houseData);

            // Search using quicksort on house data
            quickSortTop(unsortedHouseData);

            // Return number of data points for occupation data
            countRecords(occupationData);
            std:: cout << std::endl;

            // Search using shell sort on occupation data
            shellSortData(occupationData);

            // Search using quicksort on occupation data
            quickSortTop(unsortedOccupationData);

        }
        loop = false;
    }
    else
    {
        std::cout << "No Occupation found. Try again." << std::endl;
    }
    }
    return 0;
}