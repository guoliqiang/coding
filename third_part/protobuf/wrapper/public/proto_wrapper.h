#ifndef PROTO_WRAPPER_H_
#define PROTO_WRAPPER_H_

#include <string>
#include <map>
#include "file/public/file.h"
#include "third_part/protobuf/include/google/protobuf/text_format.h"
#include "third_part/protobuf/include/google/protobuf/compiler/importer.h"
#include "third_part/protobuf/include/google/protobuf/dynamic_message.h"
#include "base/public/string_util.h"
#include "base/public/shared_ptr.h"


namespace proto_wrapper {

inline bool ParseFromASCII(const std::string &str,
                           google::protobuf::Message *message) {
  return google::protobuf::TextFormat::ParseFromString(str, message);
}

inline bool ConvertToString(const google::protobuf::Message &message,
                            std::string *str) {
  return google::protobuf::TextFormat::PrintToString(message, str);
}

inline bool ConvertToBinaryFile(const std::string &src_path,
                                const std::string &dst_path,
                                google::protobuf::Message *message) {
  if (message == NULL)
    return false;
  std::string content;
  if (!file::File::ReadFileToString(src_path, &content)) {
    return false;
  }
  message->Clear();
  if (!ParseFromASCII(content, message)) {
    return false;
  }
  std::string binary_format_str;
  message->SerializeToString(&binary_format_str);
  file::File::WriteStringToFile(binary_format_str, dst_path);
  return true;
}

inline std::string MessageName(const google::protobuf::Message & message) {
  return message.GetDescriptor()->name();
}

// not support repeated member and neted message.
inline void GetNameValuePair(const google::protobuf::Message & message,
    std::map<std::string, std::string> * vec) {
  const google::protobuf::Reflection * reflection = message.GetReflection();
  std::vector<const google::protobuf::FieldDescriptor *> field_desc;
  reflection->ListFields(message, &field_desc);
  for (int i = 0; i < field_desc.size(); i++) {
    const std::string & name = field_desc[i]->name();
    std::string value = "";
    const google::protobuf::FieldDescriptor::Type type = field_desc[i]->type();
    if (type == google::protobuf::FieldDescriptor::TYPE_BYTES) {
      value = reflection->GetString(message, field_desc[i]);
    } else if (type == google::protobuf::FieldDescriptor::TYPE_STRING) {
      value = reflection->GetString(message, field_desc[i]);
    } else if (type == google::protobuf::FieldDescriptor::TYPE_INT32) {
      value = IntToString(reflection->GetInt32(message, field_desc[i]));
    } else if (type == google::protobuf::FieldDescriptor::TYPE_INT64) {
      value = Int64ToString(reflection->GetInt64(message, field_desc[i]));
    } else if (type == google::protobuf::FieldDescriptor::TYPE_UINT32) {
      value = UintToString(reflection->GetUInt32(message, field_desc[i]));
    } else if (type == google::protobuf::FieldDescriptor::TYPE_UINT64) {
      value = Uint64ToString(reflection->GetUInt64(message, field_desc[i]));
    } else if (type == google::protobuf::FieldDescriptor::TYPE_DOUBLE) {
      value = DoubleToString(reflection->GetDouble(message, field_desc[i]));
    } else if (type == google::protobuf::FieldDescriptor::TYPE_FLOAT) {
      value = DoubleToString(reflection->GetFloat(message, field_desc[i]));
    } else if (type == google::protobuf::FieldDescriptor::TYPE_BOOL) {
      value = reflection->GetBool(message, field_desc[i]) ? "true" : "false";
    } else {
      CHECK(false) << "type error!";
    }
    vec->insert(std::make_pair(name, value));
  }
}

// not support repeated member and neted message.
inline void SetNameValuePair(
     const std::map<std::string, std::string> & vec,
     google::protobuf::Message * message) {
   const google::protobuf::Descriptor * descriptor = message->GetDescriptor();
   const google::protobuf::Reflection * reflection = message->GetReflection();

   for (std::map<std::string, std::string>::const_iterator i = vec.begin();
        i != vec.end(); i++) {
     const google::protobuf::FieldDescriptor * ptr =
         descriptor->FindFieldByName(i->first);
     CHECK(ptr != NULL) << "illegal name:" << i->first;

     const google::protobuf::FieldDescriptor::Type type = ptr->type();
     if (type == google::protobuf::FieldDescriptor::TYPE_BYTES) {
       reflection->SetString(message, ptr, i->second);
     } else if (type == google::protobuf::FieldDescriptor::TYPE_STRING) {
       reflection->SetString(message, ptr, i->second);
     } else if (type == google::protobuf::FieldDescriptor::TYPE_INT32) {
       reflection->SetInt32(message, ptr, StringToInt(i->second));
     } else if (type == google::protobuf::FieldDescriptor::TYPE_INT64) {
       reflection->SetInt64(message, ptr, StringToInt64(i->second));
     } else if (type == google::protobuf::FieldDescriptor::TYPE_UINT32) {
       reflection->SetUInt32(message, ptr, StringToInt(i->second));
     } else if (type == google::protobuf::FieldDescriptor::TYPE_UINT64) {
       reflection->SetUInt64(message, ptr, StringToInt64(i->second));
     } else if (type == google::protobuf::FieldDescriptor::TYPE_DOUBLE) {
       reflection->SetDouble(message, ptr, StringToDouble(i->second));
     } else if (type == google::protobuf::FieldDescriptor::TYPE_FLOAT) {
       reflection->SetFloat(message, ptr, StringToDouble(i->second));
     } else if (type == google::protobuf::FieldDescriptor::TYPE_BOOL) {
       bool value = false;
       CHECK(StringToBool(i->second, &value)) << "convert to bool false!";
       reflection->SetBool(message, ptr, value);
     } else {
       CHECK(false) << "type error!";
     }
   }
}

inline base::shared_ptr<google::protobuf::Message> SetNameValuePair(
    const std::map<std::string, std::string> & vec,
    const google::protobuf::Descriptor & type) {
  const google::protobuf::Message * msg_template =
      google::protobuf::MessageFactory::generated_factory()->GetPrototype(&type);
  CHECK(msg_template != NULL);
  google::protobuf::Message * msg = msg_template->New();
  // can also call msg_template->GetReflection to get reflection
  SetNameValuePair(vec, msg);
  return base::shared_ptr<google::protobuf::Message>(msg);
}

class PbRuntimeCreater {
 public:
  PbRuntimeCreater() : importer_(&source_tree_, NULL) {}

  void AddProtoFile(const std::string & path) {
    std::string dirname = file::File::DirName(path);
    std::string filename = file::File::BaseName(path);
    source_tree_.MapPath("", dirname);
    CHECK(importer_.Import(filename) != NULL)
        << "import " << dirname << " " << filename << " error";
  }

  base::shared_ptr<google::protobuf::Message> CreateMessageByName(
      const std::string & name) {
    if (!messages_.count(name)) {
      const google::protobuf::Descriptor * des =
          importer_.pool()->FindMessageTypeByName(name);
      CHECK(des != NULL) << "find Message " << name << " failed!";
      const google::protobuf::Message * msg = factory_.GetPrototype(des);
      CHECK(msg != NULL) << "get msg failed for Message " << name;
      messages_.insert(std::make_pair(name, msg));
    }
    return base::shared_ptr<google::protobuf::Message>(messages_[name]->New());
  }

 private:
  google::protobuf::compiler::DiskSourceTree source_tree_;
  google::protobuf::compiler::Importer importer_;
  google::protobuf::DynamicMessageFactory factory_;
  std::map<std::string, const google::protobuf::Message *> messages_;
};
}  // namespace proto_wrapper


#endif  // PROTO_WRAPPER_H_
