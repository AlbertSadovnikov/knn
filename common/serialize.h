#pragma once
#include <ostream>
#include <istream>
#include <google/protobuf/util/delimited_message_util.h>

namespace knn {
namespace common {

    bool Serialize(std::ostream& os, const google::protobuf::MessageLite& message) {
        return google::protobuf::util::SerializeDelimitedToOstream(message, &os);
    }

    template <typename T>
    std::unique_ptr<T> Deserialize(google::protobuf::io::IstreamInputStream& input_stream) {
        auto pm = std::make_unique<T>();
        bool clean_eof;
        const auto ret = google::protobuf::util::ParseDelimitedFromZeroCopyStream(pm.get(), &input_stream, &clean_eof);
        if (!ret && !clean_eof) return nullptr;
        return pm;
    };
  }
}