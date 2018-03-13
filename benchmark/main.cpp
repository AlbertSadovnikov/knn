#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <chrono>

#include "data.pb.h"
#include "serialize.h"
#include "knn.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace std;

int main(int ac, const char** av) {
    po::variables_map vm;
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "produce help message")
                ("data", po::value<string>()->required(), "input data file name")
                ("test", po::value<string>(), "input test file name");

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

    const auto filename = vm["data"].as<string>();
    cout << "Using data from " << filename << '\n';
    cout << "------------------------------------" << '\n';

    if(!fs::exists(filename)) {
        cout << filename << " doesn't exist!" << '\n';
        return 1;
    }


    // data
    constexpr uint32_t V = 512;
    constexpr uint32_t N = 65536;
    constexpr uint32_t K = 1;
    constexpr uint32_t Q = 1000;

    const auto data = make_unique<float[]>(V * N);
    const auto query = make_unique<float[]>(V * Q);
    const auto indexes = make_unique<uint32_t[]>(K * Q);
    const auto dist = make_unique<float[]>(K * Q);

    {
        ifstream is(filename, std::ifstream::binary);
        google::protobuf::io::IstreamInputStream input_stream(&is);
        const auto h = knn::common::Deserialize<knn::Header>(input_stream);
        if (!h) {
            cout << "Could not deserialize header" << '\n';
            return 1;
        }

        if (h->nsize() != N) {
            cout << "Rebuild with N = " << h->nsize() << '\n';
            return 1;
        }
        if (h->vsize() != V) {
            cout << "Rebuild with V = " << h->vsize() << '\n';
            return 1;
        }

        size_t idx = 0;
        for (uint32_t i = 0; i < N; i++) {
            const auto v = knn::common::Deserialize<knn::Vector>(input_stream);
            if (!v) {
                cout << "Could not deserialize vector " << i << '\n';
                return 1;
            }
            for (const auto value : v->data()) {
                data[idx++] = value;
            }
        }
    }

    cout << "data loaded." << '\n';

    // no test data, using same set for query
    memcpy(query.get(), data.get(), V * Q * sizeof(float));

    cout << "Computing..." << '\n';
    const auto start = chrono::high_resolution_clock::now();
    knn::knn_base<V, N, Q, K>(data.get(), query.get(), indexes.get(), dist.get());
    const auto end = chrono::high_resolution_clock::now();
    const auto duration_seconds = chrono::duration_cast<chrono::duration<double, std::ratio<1>>>(end - start);
    const auto duration_milliseconds = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "completed in " << duration_seconds.count() << " seconds, "
         << duration_milliseconds.count() / double(Q) << "ms per single query. "<< '\n';

    // verify
    for(int i = 0; i < Q; i++) {
        if (indexes[i] != i) {
            cout << "wrong lookup index!" << '\n';
            return 1;
        }
        if (dist[i] > 1e-5f) {
            cout << "wrong distance at " << i << " = " << dist[i] << '\n';
            return 1;
        }
    }
    cout << "Response verified." << '\n';
    return 0;
}