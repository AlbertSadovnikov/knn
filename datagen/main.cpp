#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "rand.h"
#include "dist.h"
#include "data.pb.h"
#include "serialize.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace std;

vector<vector<float>> generate_data(unsigned dim, unsigned num, float delta) {
    vector<vector<float>> ret;
    while (ret.size() < num) {
        const auto v = random_vector<float>(dim);
        bool flag = true;
        for (const auto& item : ret) {
            if (knn::distance<512>(item.data(), v.data()) < delta) {
                flag = false;
                break;
            }
        }
        if (!flag) continue;
        ret.push_back(v);
        if (ret.size() % 1000 == 0) cout << ret.size() << '\n';
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
                ("num", po::value<unsigned>()->default_value(0x1000), "number of vectors")
                ("delta", po::value<float>()->default_value(1.f), "delta");

        po::store(po::parse_command_line(ac, av, desc), vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }
        po::notify(vm);
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
    cout << "Generating data..." << '\n';
    const auto data = generate_data(dim, num, delta);
    cout << "done." << '\n';

    // Serialization
    {
        ofstream os(filename, std::ofstream::binary);
        auto h = make_unique<knn::Header>();
        h->set_nsize(num);
        h->set_vsize(dim);
        h->set_id(0);
        h->set_description("Test data");
        if (!knn::common::Serialize(os, *h)) {
            cout << "Error writing header!" << '\n';
            return 1;
        }
        auto pb_v = make_unique<knn::Vector>();
        for (uint32_t i = 0; i < data.size(); i++) {
            pb_v->set_id(i);
            pb_v->set_class_id(i);
            google::protobuf::RepeatedField<float> rf(data[i].begin(), data[i].end());
            pb_v->mutable_data()->Swap(&rf);
            if (!knn::common::Serialize(os, *pb_v)) {
                cout << "Error writing vector " << i << '\n';
                return 1;
            }
        }
    }

    // Verification
    {
        ifstream is(filename, std::ifstream::binary);
        google::protobuf::io::IstreamInputStream input_stream(&is);

        const auto h = knn::common::Deserialize<knn::Header>(input_stream);
        if (!h) {
            cout << "Verification failed reading header" << '\n';
            return 1;
        }
        for (uint32_t i = 0; i < h->nsize(); i++) {
            const auto v = knn::common::Deserialize<knn::Vector>(input_stream);
            if (!v) {
                cout << "Verification failed at " << i << '\n';
                return 1;
            }
        }
    }


    return 0;
}

