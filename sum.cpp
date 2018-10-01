#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <list> 
#include <ctime>

using namespace std;

int NUM_DIGITS = 128;
int NUM_TRIALS = 2500000;

class IntegerSum
{
public:

	//Return the sum of two n-digit twos-complement binary integers as an array of bits
	unsigned char *sum(unsigned char *n1, unsigned char *n2, int numDigits)
	{
		unsigned char *sum = new unsigned char[numDigits];
		int carry = 0, i;

		//Perform addition on bitset from right-to-left
		//Addition is based on a binary full-adder using a ripple carry
		for (i = numDigits-1; i >= 0; i--)
		{
			//Our sum for this binary digit is (A XOR B) XOR Cin
			//Where Cin is the carry in, this is 0 by default
			int beforeCarry = (int)n1[i] ^ (int)n2[i];
			sum[i] = beforeCarry ^ carry;

			//Calculate the carry by (A AND B) OR ((A XOR B) AND Cin)
			//This carry value is carried over to become Cin for the next loop
			//This is the essence of the ripple-carry method
			int c1 = beforeCarry & carry;
			int c2 = (int)n1[i] & (int)n2[i];
			carry = c1 | c2;
		}

		//Print results to confirm results
		#if DEBUG
			for (i = 0; i < numDigits; i++)
			{
				cout << (int)n1[i];
			}

			cout << " + ";

			for (i = 0; i < numDigits; i++)
			{
				cout << (int)n2[i];
			}

			cout << " = ";

			for (i = 0; i < numDigits; i++)
			{
				cout << (int)sum[i];
			}

			cout << "\t" << this->toDec(n1,numDigits) << " + " << this->toDec(n2,numDigits) << " = " << this->toDec(sum, numDigits);

			if (carry == 1)
				cout << "\tOVERFLOW";

			cout << "\n";
		#endif

		return sum;
	}	

	//Return the decimal representation of a twos-complement binary integer
	int toDec(unsigned char *n, int numDigits)
	{
		int sum = 0, i;

		for (i = numDigits-1; i > 0; i--)
		{
			sum += pow(2, numDigits-1-i) * n[i];
		}

		if (n[0] == 1)
			sum -= pow(2,numDigits-1);

		return sum;
	}
};

int main(int argc, char *argv[]) 
{
	if (argc >= 3)
	{
		NUM_DIGITS = atoi(argv[1]);
		NUM_TRIALS = atoi(argv[2]);
	}

	int k, l, max = pow(2,NUM_DIGITS-1)-1;;
	IntegerSum s;
	srand(time(NULL));

	//Generate a 2d array, in order to contain 2000 binary strings
    unsigned char **inputs = new unsigned char*[NUM_TRIALS*2]();
    for (k = 0; k < NUM_TRIALS*2; k++)
        inputs[k] = new unsigned char[NUM_DIGITS]();

    //Pre-generate random binary strings of size %numDigits% in range of n-digits + and -
	for (k = 0; k < NUM_TRIALS*2; k++)
	{
		for (l = 0; l < NUM_DIGITS; l++)
		{
			inputs[k][l] = rand()%2;
		}
	}

	cout << "Starting test with:\nNUM_TRIALS = " << NUM_TRIALS << "\n" << "NUM_DIGITS = " << NUM_DIGITS << "\n\n";

	//Start timer
	clock_t begin = clock();

	//Perform addition
	for (k = 0; k < NUM_TRIALS*2; k = k + 2)
	{
		unsigned char *sum = s.sum(inputs[k], inputs[k+1], NUM_DIGITS);
	}

	//End timer
	clock_t end = clock();

	double elapsed = double(end-begin) / CLOCKS_PER_SEC;

	cout << "Elapsed time: " << (double)elapsed << "\n";
	cout << fixed << "Adds per second " << (double)NUM_TRIALS/(double)elapsed << "\n";

	if (elapsed <= .1)
		cout << "\nWarning: test may have been to small for a meaningful measurement of time. Try again with a large NUM_TRIALS\n";
	
	return 1;
}