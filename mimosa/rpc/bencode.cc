#include "bencode.hh"
#include "exception.hh"

namespace mimosa
{
  namespace rpc
  {
#define ENC_FIELD(Type, Fn, Push)                                       \
    case google::protobuf::FieldDescriptor::TYPE_##Type:                \
    if (field->is_repeated()) {                                         \
      enc.startList();                                                  \
      for (int i = 0; i < ref->FieldSize(msg, field); ++i)              \
        enc.push##Push(ref->GetRepeated##Fn(msg, field, i));            \
      enc.end();                                                        \
    } else {                                                            \
      if (field->is_required() && !field->has_default_value() &&        \
          !ref->HasField(msg, field))                                   \
        throw MissingRequiredField();                                   \
      enc.push##Push(ref->Get##Fn(msg, field));                         \
    }                                                                   \
    break

    void encode(bencode::Encoder & enc,
                const google::protobuf::Message & msg)
    {
      enc.startDict();
      auto desc = msg.GetDescriptor();
      auto ref = msg.GetReflection();
      for (int i = 0; i < desc->field_count(); ++i) {
        auto field = desc->field(i);
        enc.pushData(field->name());
        switch (field->type()) {
          ENC_FIELD(DOUBLE, Double, Int);
          ENC_FIELD(FLOAT, Float, Int);
          ENC_FIELD(INT64, Int64, Int);
          ENC_FIELD(UINT64, UInt64, Int);
          ENC_FIELD(FIXED64, UInt64, Int);
          ENC_FIELD(SFIXED64, Int64, Int);
          ENC_FIELD(SINT64, Int64, Int);
          ENC_FIELD(INT32, Int32, Int);
          ENC_FIELD(UINT32, UInt32, Int);
          ENC_FIELD(FIXED32, UInt32, Int);
          ENC_FIELD(SFIXED32, Int32, Int);
          ENC_FIELD(SINT32, Int32, Int);
          ENC_FIELD(BOOL, Bool, Int);
          ENC_FIELD(STRING, String, Data);
          ENC_FIELD(BYTES, String, Data);

        case google::protobuf::FieldDescriptor::TYPE_GROUP:
          break;
        case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
          if (field->is_repeated()) {
            enc.startList();
            for (int i = 0; i < ref->FieldSize(msg, field); ++i)
              encode(enc, ref->GetRepeatedMessage(msg, field, i));
            enc.end();
          } else {
            if (field->is_required() && !field->has_default_value() &&
                !ref->HasField(msg, field))
              throw MissingRequiredField();
            encode(enc, ref->GetMessage(msg, field));
          }
          break;

        default:
          assert(false);
          break;
        }
      }
      enc.end();
    }
  }
}
