#pragma once

#include <google/protobuf/message_lite.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <fstream>

namespace knn {
namespace common {

    class Serializer {
    public:
        Serializer() = delete;
        explicit Serializer(const std::string& path);
        ~Serializer();

        void Close();
        bool Write(const google::protobuf::MessageLite& message);

    private:
        std::ofstream m_os;
        google::protobuf::io::OstreamOutputStream m_pb_oos;
        static bool writeDelimitedTo(const google::protobuf::MessageLite& message,
                                     google::protobuf::io::ZeroCopyOutputStream* rawOutput);
    };

    class Deserializer {
    public:
        Deserializer() = delete;
        explicit Deserializer(const std::string& path);
        ~Deserializer();

        void Close();
        template <typename T>
        std::unique_ptr<T> Read() {
            auto pm = std::make_unique<T>();
            if (readDelimitedFrom(&m_pb_iis, pm.get())) {
                return pm;
            }
            return nullptr;
        };

    private:
        std::ifstream m_is;
        google::protobuf::io::IstreamInputStream m_pb_iis;
        bool readDelimitedFrom(google::protobuf::io::ZeroCopyInputStream *rawInput,
                               google::protobuf::MessageLite *message);
    };
  }
}