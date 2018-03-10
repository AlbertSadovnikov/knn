#include <boost/program_options.hpp>
#include <iostream>

#include "rand.h"
#include "dist.h"

namespace po = boost::program_options;
using namespace std;


int main(int ac, const char** av) {

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("dim", po::value<unsigned>()->default_value(512), "vector dimension")
            ("num", po::value<unsigned>()->default_value(0x10000), "number of vectors")
            ("delta", po::value<float>()->default_value(1.f), "delta");

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    const auto dim = vm["dim"].as<unsigned>();
    cout << "Dimension is set to " << dim
         << (vm["dim"].defaulted() ? " (default)." : ".") << '\n';

    const auto num = vm["num"].as<unsigned>();
    cout << "Number of vectors is set to " << num
         << (vm["num"].defaulted() ? " (default)." : ".") << '\n';

    const auto delta = vm["delta"].as<float>();
    cout << "Min distance is set to " << delta
         << (vm["delta"].defaulted() ? " (default)." : ".") << '\n';

    cout << "------------------------------------" << '\n';

    const auto a = random_vector<float>(dim);
    copy(begin(a), end(a), ostream_iterator<float>(cout, ", ")); cout << "\b\b\n";
    const auto b = random_vector<float>(dim);
    copy(begin(b), end(b), ostream_iterator<float>(cout, ", ")); cout << "\b\b\n";

    cout << dist(a, b) << endl;



    return 0;
}

