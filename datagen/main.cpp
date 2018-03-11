#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "rand.h"
#include "dist.h"
#include "data.pb.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace std;

vector<vector<float>> generate_data(unsigned dim, unsigned num, float delta) {
    vector<vector<float>> ret;
    while (ret.size() < num) {
        const auto v = random_vector<float>(dim);
        bool flag = true;
        for (const auto& item : ret) {
            if (dist(item, v) < delta) {
                flag = false;
                break;
            }
        }
        if (!flag) continue;
        ret.push_back(v);
    }
    return ret;
}

int main(int ac, const char** av) {

    po::variables_map vm;
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "produce help message")
                ("file", po::value<string>()->required(), "output file name")
                ("dim", po::value<unsigned>()->default_value(512), "vector dimension")
                ("num", po::value<unsigned>()->default_value(0x10000), "number of vectors")
                ("delta", po::value<float>()->default_value(1.f), "delta");

        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }
    }
    catch(const po::error& e) {
        cout << e.what();
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

    const auto filename = vm["file"].as<string>();
    cout << "Output filename is " << filename << '\n';
    cout << "------------------------------------" << '\n';

    if(fs::exists(filename)) {
        cout << filename << " already exists!" << '\n';
        return 1;
    }


    // generate data
    auto data = generate_data(dim, num, delta);


    //fs::ofstream ofs(filename, ios_base::binary);
    //ofs.close();

    return 0;
}

