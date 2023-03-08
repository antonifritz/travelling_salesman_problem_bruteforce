#include <iostream>
#include <windows.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

std::vector<int> shortest_path_cities;
int shortest_path;

void printAdjacencyMatrix(int numberOfVertexes, int** adjacencyMatrix)
{
    printf("\nMacierz sasiedztwa:\n\n    ");
    for (int i = 0; i < numberOfVertexes; i++)
    {
        printf("%3d", i);
    }

    printf("\n");

    printf("----");

    for (int i = 0; i < numberOfVertexes; i++)
    {
        printf("---");
    }

    printf("\n");

    for (int i = 0; i < numberOfVertexes; i++)
    {
        printf("%3d|", i);
        for (int j = 0; j < numberOfVertexes; j++)
        {
            printf("%3d", adjacencyMatrix[i][j]);
        }

        printf("\n");
    }
}

void calculateTravellingSalesmanBruteForce(int numberOfVertexes, int** adjacencyMatrix)
{
    int source = 0;
    std::vector<int> cities;
    std::vector<int> path;
    int path_weight = 0;
    shortest_path = INT_MAX;

    //utworzenie listy miast

    for (int i = 1; i < numberOfVertexes; i++)
    {
        if (i != source)
        {
            cities.push_back(i);
        }
    }
    int n = cities.size();

    //pętla do while dopóki sprawdzone zostaną wszystkie permutacje zbioru miast 

    do
    {
        int j = source;

        //obliczenie dlugości ścieżki (zczytywanie danych z macierzy sasiedztwa)
        path_weight = 0;
        for (int i = 0; i < n; i++)
        {
            path_weight = path_weight + adjacencyMatrix[j][cities.at(i)];
            j = cities.at(i);
        }

        //ścieżka kończy się na źródle, którą to odległość również należy uwzględnić
        path_weight = path_weight + adjacencyMatrix[j][source];

        //jeżeli ścieżka jest krótsza od aktualnie najkrótszej ścieżki przypisujemy ją do zmiennej shortest_path
        if (path_weight < shortest_path)
        {
            shortest_path = path_weight;
            shortest_path_cities = cities;
        }
    } while (std::next_permutation(cities.begin(), cities.end()));
}

int main()
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    std::fstream iniFile;
    std::fstream dataFile;
    std::fstream outputFile;
    std::string dataFileName;
    std::string outputFileName;
    std::string line;
    std::string numberOfRepeatsString;
    std::string numberString;
    int number = 0;
    int numberOfRepeats = 10;
    int numberOfVertexes = 6;
    int** adjacencyMatrix;
    int counter = 0;
    std::cout << "czytanie danych konfiguracyjnych z pliku .ini...\n";
    iniFile.open("config.ini", std::ios::out | std::ios::in);

    if (iniFile.is_open())
    {
        std::getline(iniFile, line);
        int i = 0;

        while (line.at(i) != 32)
        {
            dataFileName.push_back(line.at(i));
            i++;
        }

        i++;

        while (line.at(i) != 32 && i != line.length() - 1)
        {
            numberOfRepeatsString.push_back(line.at(i));
            i++;
        }

        numberOfRepeats = atoi(numberOfRepeatsString.c_str());

        std::getline(iniFile, line);

        outputFileName = line;
    }

    iniFile.close();

    dataFile.open(dataFileName, std::ios::out | std::ios::in);

    std::getline(dataFile, line);
    numberOfVertexes = atoi(line.c_str());

    adjacencyMatrix = new int* [numberOfVertexes];
    for (int i = 0; i < numberOfVertexes; i++)
    {
        adjacencyMatrix[i] = new int[numberOfVertexes];
    }

    for (int i = 0; i < numberOfVertexes; i++)
    {
        for (int j = 0; j < numberOfVertexes; j++)
        {
            adjacencyMatrix[i][j] = -1;
        }
    }

    for (int i = 0; i < numberOfVertexes; i++)
    {
        std::getline(dataFile, line);
        for (int j = 0; j < line.length(); j++)
        {
            if (numberString.empty() && line.at(j) == 32)
            {

            }
            else if (line.at(j) != 32)
            {
                numberString.push_back(line.at(j));
            }
            else
            {
                number = atoi(numberString.c_str());
                adjacencyMatrix[i][counter] = number;
                counter++;
                numberString = "";
            }
        }

        number = atoi(numberString.c_str());
        adjacencyMatrix[i][counter] = number;
        counter = 0;
        numberString = "";
    }

    dataFile.close();

    std::cout << "Nazwa pliku z danymi wejsciowymi: " << dataFileName << std::endl;
    std::cout << "Wielkosc instancji: " << numberOfVertexes << std::endl;
    std::cout << "Liczba powtorzen: " << numberOfRepeats << std::endl;
    printAdjacencyMatrix(numberOfVertexes, adjacencyMatrix);

    double time = 0;

    for (int i = 0; i < numberOfRepeats; i++)
    {
        double interval = 0;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);

        calculateTravellingSalesmanBruteForce(numberOfVertexes, adjacencyMatrix);

        QueryPerformanceCounter(&end);
        interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

        time = time + interval;
    }

    time = time / ((double)numberOfRepeats);


    outputFile.open("outputFile.csv", std::ios::out | std::ios::app);
    
    outputFile << numberOfVertexes << ";" << time << ";" << numberOfRepeats << ";" << shortest_path << "; 0 -> ";
    for (int city : shortest_path_cities)
    {
        outputFile << city << " -> ";
    }
    outputFile << "0\n";

    outputFile.close();

    std::cout<<"\nDlugosc sciezki: " << shortest_path << std::endl;
    std::cout << "Sciezka: 0 -> ";
    for (int city : shortest_path_cities)
    {
        std::cout << city << " -> ";
    }
    std::cout << "0\n";
}
