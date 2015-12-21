#include "json.hh"
#include "exception.hh"

namespace mimosa
{
  namespace rpc
  {
#define ENC_FIELD(Type, Fn, Push)                                       \
    case google::protobuf::FieldDescriptor::CPPTYPE_##Type:             \
    if (field->is_repeated()) {                                         \
      enc.startArray();                                                 \
      for (int i = 0; i < ref->FieldSize(msg, field); ++i)              \
        enc.push##Push(ref->GetRepeated##Fn(msg, field, i));            \
          enc.endArray();                                               \
    } else {                                                            \
      if (field->is_required() && !field->has_default_value() &&        \
          !ref->HasField(msg, field))                                   \
        throw MissingRequiredField();                                   \
      enc.push##Push(ref->Get##Fn(msg, field));                         \
    }                                                                   \
    break

    void jsonEncode(json::Encoder &                   enc,
                    const google::protobuf::Message & msg)
    {
      enc.startObject();
      auto desc = msg.GetDescriptor();
      auto ref = msg.GetReflection();
      for (int i = 0; i < desc->field_count(); ++i) {
        auto field = desc->field(i);

        if (!field->is_repeated() && !ref->HasField(msg, field))
          continue;

        enc.pushString(field->name());
        switch (field->cpp_type()) {
          ENC_FIELD(DOUBLE, Double, Float);
          ENC_FIELD(FLOAT, Float, Float);
          ENC_FIELD(INT64, Int64, Number);
          ENC_FIELD(UINT64, UInt64, Number);
          ENC_FIELD(INT32, Int32, Number);
          ENC_FIELD(UINT32, UInt32, Number);
          ENC_FIELD(BOOL, Bool, Boolean);
          ENC_FIELD(STRING, String, String);

        case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
          if (field->is_repeated()) {
            enc.startArray();
            for (int i = 0; i < ref->FieldSize(msg, field); ++i)
              jsonEncode(enc, ref->GetRepeatedMessage(msg, field, i));
            enc.endArray();
          } else {
            if (field->is_required() && !field->has_default_value() &&
                !ref->HasField(msg, field))
              throw MissingRequiredField();
            jsonEncode(enc, ref->GetMessage(msg, field));
          }
          break;

        case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
          if (field->is_repeated()) {
            enc.startArray();
            for (int i = 0; i < ref->FieldSize(msg, field); ++i) {
              auto eval = ref->GetRepeatedEnum(msg, field, i);
              enc.pushString(eval->name());
            }
            enc.endArray();
          } else {
            if (field->is_required() && !field->has_default_value() &&
                !ref->HasField(msg, field))
              throw MissingRequiredField();
            auto eval = ref->GetEnum(msg, field);
            enc.pushString(eval->name());
          }
          break;
        }
      }
      enc.endObject();
    }

    void jsonEncode(stream::Stream::Ptr               output,
                    const google::protobuf::Message & msg)
    {
      json::Encoder enc(output);
      jsonEncode(enc, msg);
    }

#define DEC_NUMBER(Type, Fn)                                            \
    case google::protobuf::FieldDescriptor::CPPTYPE_##Type:             \
    if (field->is_repeated()) {                                         \
      token = dec.pull();                                               \
      if (token != json::Decoder::kArrayBegin)                          \
        throw InvalidFormat();                                          \
      for (token = dec.pull(); token != json::Decoder::kArrayEnd;       \
           token = dec.pull()) {                                        \
        if (token == json::Decoder::kRational) {                        \
          refl->Add##Fn(msg, field, dec.rational());                    \
        } else if (token == json::Decoder::kInteger) {                  \
          refl->Add##Fn(msg, field, dec.integer());                     \
        } else if (token == json::Decoder::kString) {                   \
          refl->Add##Fn(msg, field, std::stod(dec.string()));           \
        } else if (token != json::Decoder::kNull) {                     \
          throw InvalidFormat();                                        \
        }                                                               \
      }                                                                 \
    } else {                                                            \
      if (refl->HasField(*msg, field))                                  \
        throw FieldAlreadySet();                                        \
      token = dec.pull();                                               \
      if (token == json::Decoder::kRational) {                          \
        refl->Set##Fn(msg, field, dec.rational());                      \
      } else if (token == json::Decoder::kInteger) {                    \
        refl->Set##Fn(msg, field, dec.integer());                       \
      } else if (token == json::Decoder::kString) {                     \
        refl->Set##Fn(msg, field, std::stod(dec.string()));             \
      } else if (token != json::Decoder::kNull) {                       \
        throw InvalidFormat();                                          \
      }                                                                 \
    }                                                                   \
    break;

    static void jsonDecode2(json::Decoder &             dec,
                            google::protobuf::Message * msg)
    {
      auto desc = msg->GetDescriptor();
      auto refl = msg->GetReflection();

      while (true) {
        auto token = dec.pull();
        if (token == json::Decoder::kObjectEnd)
          break;
        const std::string name = dec.string();
        auto field = desc->FindFieldByName(name);
        if (!field) {
          dec.eatValue();
          continue;
        }

        switch (field->cpp_type()) {
        case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
          if (field->is_repeated()) {
            token = dec.pull();
            if (token != json::Decoder::kArrayBegin)
              throw InvalidFormat();
            for (token = dec.pull(); token != json::Decoder::kArrayEnd;
                 token = dec.pull()) {
              if (token == json::Decoder::kString)
                refl->AddString(msg, field, dec.string());
              else if (token != json::Decoder::kNull)
                throw InvalidFormat();
            }
          } else {
            if (refl->HasField(*msg, field))
              throw FieldAlreadySet();
            token = dec.pull();
            if (token == json::Decoder::kString)
              refl->SetString(msg, field, dec.string());
            else if (token != json::Decoder::kNull)
              throw InvalidFormat();
          }
          break;

          DEC_NUMBER(DOUBLE, Double);
          DEC_NUMBER(FLOAT, Float);
          DEC_NUMBER(INT64, Int64);
          DEC_NUMBER(UINT64, UInt64);
          DEC_NUMBER(INT32, Int32);
          DEC_NUMBER(UINT32, UInt32);

        case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
          if (field->is_repeated()) {
            token = dec.pull();
            if (token != json::Decoder::kArrayBegin)
              throw InvalidFormat();
            for (token = dec.pull(); token != json::Decoder::kArrayEnd;
                 token = dec.pull()) {
              if (token != json::Decoder::kBoolean)
                refl->AddBool(msg, field, dec.boolean());
              else if (token != json::Decoder::kNull)
                throw InvalidFormat();
            }
          } else {
            if (refl->HasField(*msg, field))
              throw FieldAlreadySet();
            token = dec.pull();
            if (token == json::Decoder::kBoolean)
              refl->SetBool(msg, field, dec.boolean());
            else if (token != json::Decoder::kNull)
              throw InvalidFormat();
          }
          break;

        case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
          if (field->is_repeated()) {
            token = dec.pull();
            if (token != json::Decoder::kArrayBegin)
              throw InvalidFormat();
            for (token = dec.pull(); token != json::Decoder::kArrayEnd;
                 token = dec.pull()) {
              if (token == json::Decoder::kNull)
                continue;
              if (token != json::Decoder::kString)
                throw InvalidFormat();
              auto edesc = field->enum_type();
              auto eval = edesc->FindValueByName(dec.string());
              if (!eval)
                throw InvalidEnumValue();
              refl->AddEnum(msg, field, eval);
            }
          } else {
            if (refl->HasField(*msg, field))
              throw FieldAlreadySet();
            token = dec.pull();
            if (token == json::Decoder::kNull)
              break;
            if (token != json::Decoder::kString)
              throw InvalidFormat();
            auto edesc = field->enum_type();
            auto eval = edesc->FindValueByName(dec.string());
            if (!eval)
              throw InvalidEnumValue();
            refl->SetEnum(msg, field, eval);
          }
          break;

        case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
          if (field->is_repeated()) {
            token = dec.pull();
            if (token != json::Decoder::kArrayBegin)
              throw InvalidFormat();
            for (auto depth = dec.depth(); depth == dec.depth(); ) {
              token = dec.pull();
              if (token == json::Decoder::kArrayEnd)
                break;
              if (token != json::Decoder::kObjectBegin)
                throw InvalidFormat();
              auto msg2 = refl->AddMessage(msg, field);
              jsonDecode2(dec, msg2);
            }
          } else {
            if (refl->HasField(*msg, field))
              throw FieldAlreadySet();
            token = dec.pull();
            if (token != json::Decoder::kObjectBegin)
              throw InvalidFormat();
            auto msg2 = refl->MutableMessage(msg, field);
            jsonDecode2(dec, msg2);
          }
          break;
        }
      }
    }

    void jsonDecode(json::Decoder &             dec,
                    google::protobuf::Message * msg)
    {
      auto token = dec.pull();
      if (token == json::Decoder::kEof)
        return;
      if (token != json::Decoder::kObjectBegin)
        throw InvalidFormat();

      jsonDecode2(dec, msg);
    }

    void jsonDecode(stream::Stream::Ptr         input,
                    google::protobuf::Message * msg)
    {
      json::Decoder dec(input);
      jsonDecode(dec, msg);
    }
  }
}
