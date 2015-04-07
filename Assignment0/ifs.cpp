#include "ifs.h"
#include "image.h"
ifs::ifs()
{
	pMatrix = NULL;
	pfMatrix = NULL;
}

ifs::ifs(const std::string & inputFileName)
{
	pMatrix = NULL;
	pfMatrix = NULL;
}


ifs::~ifs()
{
// 	delete this->pMatrix;
// 	delete this->pfMatrix;
}

int ifs::read(const char * input_file)
{
	// ========================================================
	// ========================================================
	// Some sample code you might like to use for
	// parsing the IFS transformation files

	// open the file
	FILE *input = fopen(input_file, "r");
	assert(input != NULL);

	// read the number of transforms
	int num_transforms;
	fscanf(input, "%d", &num_transforms);
	// < DO SOMETHING WITH num_transforms >
	nMatrix = num_transforms;
	pfMatrix = new float[nMatrix];
	pMatrix = new Matrix[nMatrix];
	// read in the transforms
	for (int i = 0; i < num_transforms; i++) {
		float probability;
		fscanf(input, "%f", &probability);
		Matrix m;
		m.Read3x3(input);
		// < DO SOMETHING WITH probability and m >
		pfMatrix[i] = probability;
		pMatrix[i] = m;
	}

	// close the file
	fclose(input);

	// ========================================================
	// ========================================================	
	return 0;
}

int ifs::parse(char** argv, int argc)
{
	// ========================================================
	// ========================================================
	// Some sample code you might like to use for parsing 
	// command line arguments and the input IFS files

	// sample command line:
	// ifs -input fern.txt -points 10000 -iters 10 -size 100 -output fern.tga

	char *input_file = NULL;
	int num_points = 10000;
	int num_iters = 10;
	int size = 100;
	char *output_file = NULL;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-points")) {
			i++; assert(i < argc);
			num_points = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-iters")) {
			i++; assert(i < argc);
			num_iters = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			size = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	read(input_file);
	render(output_file, num_points, num_iters, size);
	return 0;
}

int ifs::render(const char * outputFileName, int nPoint, int nIteration, int size) const
{
// 	for "lots" of random points(x0, y0)
// 		for k = 0 to num_iters
// 			pick a random transform fi
// 			(xk + 1, yk + 1) = fi(xk, yk)
// 		display a dot at(xk, yk)
	Image img(size,size);
	
	img.SetAllPixels(Vec3f(1, 1, 1));

	for (int i = 0; i < nPoint;i++)
	{
		Vec2f point(randf(),randf());//a random dot.
		for (int k = 0; k < nIteration; k++)
		{
			const Matrix * matrix = rand_transform();
			matrix->Transform(point);
			bool isUnreasonable = false;
			if (point.y() < 0) {
				point.Set(point.x(), 0);
				isUnreasonable = true;
			}
			else if (point.y() >= 1) {
				point.Set(point.x(), 0.9999999);
				isUnreasonable = true;
			}
			if (point.x() < 0) {
				point.Set(0, point.y());
				isUnreasonable = true;
			}
			else if (point.x() >= 1) {
				point.Set(0.9999999, point.y());
				isUnreasonable = true;
			}
			if (isUnreasonable) {
				break;
			}
		}
		img.SetPixel(point.x()*size, point.y()*size, Vec3f(0, 0, 0));
	}
	img.SaveTGA(outputFileName);
	return 0;
}

const Matrix * ifs::rand_transform() const
{
	float r = randf(0.0f, 1.0f);
	float p = 0.0f;
	int matrix_index = 0;
	for (; matrix_index <= nMatrix; matrix_index++)
	{
		if (r < p + pfMatrix[matrix_index]) 
			break;
		p += pfMatrix[matrix_index];
	}
	return pMatrix+matrix_index;
}

float ifs::randf(float LO, float HI) const
{
	float f=LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	return f;
}
