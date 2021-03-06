
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <vector>
#include <list>

#define	LINE_SEP	'#'
#define	FIELD_SEP	','

// vector of real numbers to store one line of sample data from the file
typedef std::vector<double> Data;
// list of rows from the input file
typedef	std::list<Data>     Chart;

// private functions

static bool read_proba(std::istream &str, Data &tdx);
static bool read_chart(Chart &chart, std::istream &str);
static double compute_k(const Chart &chart, int j, double time_slice);
static double compute_function(double p);
static void do_job(const Chart &chart, double time_slice);

//----------------------------------------------------------------------
//	bool read_proba(std::istream &str, Data &tdx);
// ----------------------------------------------------------------------

bool read_proba(std::istream &str, Data &tdx)
{
    char c;
    double number;

    // tdx will hold all samples from one line of the input file
    tdx.clear();

    // read the first number from the input until the field
    // or line separator
    str >> number;
    while (str >> c && c != FIELD_SEP && c != LINE_SEP) {
	// empty
    }

    while (str && c == FIELD_SEP) {
	// we have found one number, add it to the output and
	// read the next one
	tdx.push_back(number);

	str >> number;
	while (str >> c && c != FIELD_SEP && c != LINE_SEP) {
	    // empty
	}
    }
    if (!str || c != LINE_SEP)
	return false;

    // we have reached the end of line separator and have
    // successfully stored one line in tdx
    tdx.push_back(number);
    return true;
}

//----------------------------------------------------------------------
//	bool read_chart(Chart &chart, istream &str);
// ----------------------------------------------------------------------

static bool read_chart(Chart &chart, std::istream &str)
{
    Data dx;
    // read all lines from the input file and store in chart
    while (read_proba(str, dx)) {
	chart.push_back(dx);
    }
    return chart.size() > 1 && chart.front().size() > 0;
}

//----------------------------------------------------------------------
//	static double compute_k(const Chart &chart, int j, double time_slice)
//----------------------------------------------------------------------

static double compute_k(const Chart &chart, int j, double time_slice)
{
    double i = 0;
    Chart::const_iterator prev, current;

    // calculate integral of data from column of the input file
    prev = current = chart.begin();
    for (current++; current != chart.end(); prev = current, current++) {
	i += ((*prev)[j] + (*current)[j]) * time_slice / 2;
    }
    if (i <= 0) {
	std::cerr << "bad input, zero integral" << std::endl;
	exit(1);
    }

    return i;
}

//----------------------------------------------------------------------
//	compute_entropy(const Chart &chart, int j, 
//			double time_slice, double k);
//----------------------------------------------------------------------

static double compute_entropy(const Chart &chart, int j, 
			      double time_slice, double k)
{
    double	entropy = 0;
    Chart::const_iterator current = chart.begin();
    double prevp = compute_function((*current)[j] / k);
    double p;

    // calculate -integral(p(x)/k * log2(p(x)/k))
    for (current++; current != chart.end(); prevp = p, current++) {
	p = compute_function((*current)[j] / k);
	entropy -= (prevp + p) * time_slice / 2;
    }

    return entropy;
}

static double compute_function(double p)
{
    double value = 0;

    if (p < 0) {
	std::cerr << "bad probability: " << p << std::endl;
	exit(1);
    } else if (p > 0) {
	value = p * log2(p);
    }

    return value;
}

//----------------------------------------------------------------------
//	static void do_job(const Chart &chart, double time_slice);
//----------------------------------------------------------------------

static void do_job(const Chart &chart, double time_slice)
{
    int width = chart.front().size();
    std::vector<double> entropies(width);
    double entropy = 0;

    // calculate entropy separately for each column of input
    for (int j = 0; j < width; j++) {
	double k = compute_k(chart, j, time_slice);
	entropies[j] = compute_entropy(chart, j, time_slice, k);
	std::cout << j+1 << ". entropy: " << entropies[j] << std::endl;
	entropy += entropies[j];
    }
    std::cout << "\nsum entropy: " << entropy << std::endl;
}

//----------------------------------------------------------------------
//	main
//----------------------------------------------------------------------

int main(int argc, char *argv[])
{
    Chart		chart;
    double		time_slice;
    std::ifstream	input_file;
    std::istream	*input;

    if (argc != 3) {
	std::cerr << "usage: entropy frequency file_name" << std::endl;
	exit(1);
    } else {
	time_slice = atoi(argv[1]);
	if (time_slice <= 0) {
	    std::cerr << "bad frequency" << std::endl;
	    exit(1);
	}
	time_slice = 1/time_slice;
	if (!strcmp(argv[2],"-")) {
	    input = &std::cin;
	} else {
	    input_file.open(argv[2]);
	    if (!input_file) {
		std::cerr << "can't open file: " << argv[2] << std::endl;
		exit(1);
	    }
	    input = &input_file;
	}
    }
    if (!read_chart(chart, *input)) {
	std::cerr << "error occured during processing of input file" << std::endl;
	exit(1);
    }

    do_job(chart, time_slice);

    return 0;
}
