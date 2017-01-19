
#include <list>
#include <vector>
#include <algorithm>

class Decoder {
public:
    
    Decoder(signed char**,int,int);	// Constructor - Matrix    
	~Decoder();	// Destructor
        
    
	// Decode a chromosome, returning its fitness as a double-precision floating point:
	double decode( std::vector< double >& chromosome) const;
    
private:
    const signed char** matriz;
    int rows;
    int cols;
};

