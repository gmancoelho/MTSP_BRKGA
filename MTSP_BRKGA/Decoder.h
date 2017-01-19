
#include <list>
#include <vector>
#include <algorithm>

class Decoder {
public:
    
    Decoder(signed char**,int,int);	// Constructor - Matrix
    
	~Decoder();	// Destructor
        
    // Public Methods
    
	// Decode a chromosome, returning its fitness as a double-precision floating point:
	double decode(const std::vector< double >& chromosome) const;

private:
    const signed char** matriz;
};

