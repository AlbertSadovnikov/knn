#include "serialize.h"

using namespace knn::common;
using namespace std;


Serializer::Serializer(const string& path) : m_os(path, std::ofstream::binary), m_pb_oos(&m_os) {
}

Serializer::~Serializer() {
    Close();
}

void Serializer::Close() {
    m_os.close();
}

bool Serializer::writeDelimitedTo(const google::protobuf::MessageLite& message,
                                  google::protobuf::io::ZeroCopyOutputStream* rawOutput) {
    google::protobuf::io::CodedOutputStream output(rawOutput);
    const auto size = message.ByteSize();
    output.WriteVarint32((google::protobuf::uint32)size);
    uint8_t *buffer = output.GetDirectBufferForNBytesAndAdvance(size);
    if (buffer != nullptr) {
        message.SerializeWithCachedSizesToArray(buffer);
    } else {
        message.SerializeWithCachedSizes(&output);
        if (output.HadError()) return false;
    }
    return true;
}

bool Serializer::Write(const google::protobuf::MessageLite &message) {
    return writeDelimitedTo(message, &m_pb_oos);
}

Deserializer::Deserializer(const std::string& path) : m_is(path, std::ifstream::binary), m_pb_iis(&m_is) {
}

Deserializer::~Deserializer() {
    Close();
}

void Deserializer::Close() {
    m_is.close();
}

bool Deserializer::readDelimitedFrom(google::protobuf::io::ZeroCopyInputStream *rawInput,
                                     google::protobuf::MessageLite *message) {
    google::protobuf::io::CodedInputStream input(rawInput);
    uint32_t size;
    if (!input.ReadVarint32(&size)) return false;
    google::protobuf::io::CodedInputStream::Limit limit = input.PushLimit(size);
    if (!message->MergeFromCodedStream(&input)) return false;
    if (!input.ConsumedEntireMessage()) return false;
    input.PopLimit(limit);
    return true;
}