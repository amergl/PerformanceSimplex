#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <vector>
// #include <cstring>

// min/max number of variables for simplex
// max values for tableau coefficients
const int VAR_MIN   = 100;
const int VAR_MAX   = 150;
const int COEFF_MAX = 1000;

int numVariables;
int numRestrictions;
int width;
int height;


// returns random value between min and max
int random_value(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

// create randow simplex tableau
// template<typename T, int height, int width>
// void create_tableau(T (&tableau)[height][width])
void create_tableau(int **tableau, int **raw)
{

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (i >= numRestrictions)
            {
                // negative random values in target function row
                // 0 if in column of slack variable
                tableau[i][j] = (j>=numVariables) ? 0
                                                  : -1*random_value(0, COEFF_MAX);
                // raw file: target function in first row
                raw[0][j] = tableau[i][j];
            }

            // right hand side b
            else if (j == width-1)
            {
                tableau[i][j] = random_value(0, COEFF_MAX);

                // raw file: - target function in first row -> shift by one row
                //           - only one column for slack variables
                raw[i+1][numVariables+1] = tableau[i][j];
            }

            // slack variables
            else if (j >= numVariables)
            {
                tableau[i][j] = ((j-numVariables) == i) ? 1 : 0;

                // raw file: - target function in first row -> shift by one row
                //           - one column for slack variables (-1 instead of 1)
                if (tableau[i][j] != 0)
                    raw[i+1][numVariables] = -1;
            }

            // random values for restrictions
            else
            {
                tableau[i][j] = random_value(0, COEFF_MAX);

                // raw file: target function in first row -> shift by one row
                raw[i+1][j] = tableau[i][j];
            }
        } // for j
    } // for i
}


// template<typename T, int height, int width>
// std::ostream& writemap(std::ostream& os, T (&matrix)[height][width])
std::ostream& write_tableau(std::ostream& os, int **tableau)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            os << tableau[i][j]<<" ";
        }
        os<<"\n";
    }
    return os;
}

std::ostream& write_raw(std::ostream& os, int **raw)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < numVariables+2; ++j)
        {
            if (!(j>=numVariables && i == 0))
                os << raw[i][j]<<" ";
        }
        os<<"\n";
    }
    return os;
}

// main function
int main(int argc, char* argv[])
{

    int numDatasets;

    // check how many tableus are wanteds
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <Number of datasets>"
                  << std::endl;
        return 1;
    }
    else
        numDatasets = atoi(argv[1]);

    // create tableaus
    for (int set_number = 0; set_number < numDatasets; ++set_number)
    {

        // random number of variables
        numVariables    = random_value(VAR_MIN, VAR_MAX);

        // one more restriction than variables
        numRestrictions = numVariables + 1;

        // width = variables + slack variables + b
        width  =  numVariables + numRestrictions + 1;

        // height = restrictions + target function row
        height = numRestrictions + 1;

        // initialize (condensed) tableau
        int **tableau, **raw;
        tableau = new int *[height];
        raw     = new int *[height];
        for(int i = 0; i < height; ++i)
        {
            tableau[i] = new int[width];
            raw[i]     = new int[numVariables+2];
        }


        create_tableau(tableau, raw);

        // filename: data<set_number>.txt or .raw (starts with 4!)
        std::stringstream filename, rawname;
        filename << "test/data" << 4+set_number << ".txt";
        rawname  << "test/data" << 4+set_number << ".raw";

        std::fstream tableau_ofs(filename.str().c_str(), std::ios::out);
        std::fstream raw_ofs(rawname.str().c_str(), std::ios::out);

        if (tableau_ofs.is_open())
        {
            write_tableau(tableau_ofs, tableau);
            write_raw(raw_ofs, raw);

            std::cout << "===== LP #" << (set_number+1) << " =====" << std::endl;
            write_tableau(std::cout, tableau);
            std::cout << "--- condensed" << " --" << std::endl;
            write_raw(std::cout, raw);
            std::cout << "=================" << std::endl << std::endl;

            tableau_ofs.close();
            raw_ofs.close();
        }
    } // create tableaus

    return 0;
}
