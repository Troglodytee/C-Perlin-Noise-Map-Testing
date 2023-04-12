#ifndef PNF
#define PNF

#define MAX_PRIME_INDEX 10


typedef struct
{
	int numX, numY, numOctaves, primeIndex, maxPrimeIndex;
	double persistence;
	int primes[MAX_PRIME_INDEX][3];
} PERLINFACTORY;

PERLINFACTORY NewPerlinFactory();

double Noise(PERLINFACTORY* perlinFactory, int i, int x, int y);

double Interpolate(double a, double b, double x);

double SmoothedNoise(PERLINFACTORY* perlinFactory, int i, int x, int y);

double InterpolatedNoise(PERLINFACTORY* perlinFactory, int i, double x, double y);

double ValueNoise2D(PERLINFACTORY* perlinFactory, double x, double y);


#endif