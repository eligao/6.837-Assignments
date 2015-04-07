#pragma once
#include "matrix.h"
#include "vectors.h"

class ifs
{
public:
	// 	a constructor that creates an IFS
	ifs();
	//  constructor from a file
	ifs(const std::string & inputFileName);

	// 	a destructor that frees the memory of the various arrays(using delete)
	virtual ~ifs();

	// 	an input method that reads the IFS description
	int read(const char * inputFileName);
	// 	a render method that takes as input an image instance, a number of points and a number of iterations
	int render(const char * outputFileName, int nPoint, int nIteration, int size) const;
	//  parsing command line arguments
	int parse(char** argv, int argc);
private:
	// 	a field to store n, the number of transformations
	int nMatrix;
	// 	an array of matrices representing the n transformations
	Matrix * pMatrix;
	// 	an array of the corresponding probabilities for choosing a transformation
	float *pfMatrix;
	//  pick a random transform
	const Matrix * rand_transform() const;
	//generate float between min and max
	float randf(float LO = 0.0f, float HI = 1.0f) const;
// 	//  transforming 
// 	Vec2f& transform(Vec2f* vec);
};

