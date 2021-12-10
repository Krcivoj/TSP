

class City
{
    public:
        double x;
        double y;

        City (double, double);
        
        bool operator == (City);
};

double d(City , City);