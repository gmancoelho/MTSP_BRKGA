
#include <list>
#include <vector>
#include <algorithm>
#include <set>


class Decoder {
public:
    
    Decoder(signed char**,int,int,int,std::vector< std::set<int> > *);	// Constructor - Matrix
	~Decoder();	// Destructor
    
	// Decode a chromosome, returning its fitness as a double-precision floating point:
	double decode(const std::vector< double >& chromosome) const;
    
    std::vector<std::set<int> > toolsPerTask;
    const signed char** matriz;
    int rows;
    int mag;
    int cols;
};

