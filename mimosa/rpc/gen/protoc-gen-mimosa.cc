﻿#include <string>
#include <sstream>
#include <cassert>

#include <zlib.h> // For crc32

#include <google/protobuf/compiler/code_generator.h>
#if GOOGLE_PROTOBUF_VERSION >= 5030000
#include <google/protobuf/compiler/cpp/cpp_generator.h>
#else
#include <google/protobuf/compiler/cpp/generator.h>
#endif
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/printer.h>

namespace gp   = google::protobuf;
namespace gpio = google::protobuf::io;
namespace gpc  = google::protobuf::compiler;

class ServiceGenerator : public gpc::CodeGenerator
{
  static uint32_t computeId(const std::string_view & str)
  {
    const uint32_t init = ::crc32(0L, Z_NULL, 0);
    return ::crc32(init, (const Bytef *)str.data(), str.size());
  }

  static uint32_t computeMethodId(const gp::MethodDescriptor * method)
  {
    return computeId(method->name());
  }

  static uint32_t computeServiceId(const gp::ServiceDescriptor * service)
  {
    return computeId(service->full_name());
  }

  static std::string cppFullName(const std::string_view & input)
  {
    std::ostringstream os;

    std::string::size_type pos1 = 0;
    std::string::size_type pos2 = 0;
    do {
      pos2 = input.find('.', pos1);
      os << "::" << input.substr(pos1, pos2 - pos1);
      pos1 = pos2 + 1;
    } while (pos2 != std::string::npos);
    return os.str();
  }

  static void fillMethodDict(const gp::MethodDescriptor *         method,
                             std::map<std::string, std::string> * dict)
  {
    std::ostringstream method_id;
    method_id << computeMethodId(method) << "UL";
    (*dict)["MethodId"] = method_id.str();
    (*dict)["MethodName"] = method->name();
    (*dict)["RequestType"] = cppFullName(method->input_type()->full_name());
    (*dict)["ResponseType"] = cppFullName(method->output_type()->full_name());

    int nb_spaces = 14 + method->name().length();
    (*dict)["ParamAlign"] = std::string(nb_spaces, ' ');
    (*dict)["InlineParamAlign"] = std::string(nb_spaces + 7, ' ');
  }

  static bool fillServiceDict(const gp::ServiceDescriptor *        service,
                              std::map<std::string, std::string> * dict,
                              std::string *                        /*error*/)
  {
    std::ostringstream service_id;

    service_id << computeServiceId(service) << "UL";

    std::map<std::string, std::string> variables;
    (*dict)["ServiceName"]     = service->name();
    (*dict)["ServiceFullName"] = service->full_name();
    (*dict)["ServiceId"]       = service_id.str();
    return true;
  }

  static bool generateServiceClient(gpio::Printer &               printer,
                                    const gp::ServiceDescriptor * service,
                                    std::string *                 error)
  {
    std::map<std::string, std::string> variables;
    if (!fillServiceDict(service, &variables, error))
      return false;
    printer.Print(
      variables,
      "class $ServiceName$_Client : public $ServiceName$\n"
      "{\n"
      "public:\n"
      "  MIMOSA_DEF_PTR($ServiceName$_Client);"
      "\n"
      "  inline $ServiceName$_Client(::mimosa::rpc::Channel::Ptr channel)\n"
      "    : channel_(channel)\n"
      "  {\n"
      "  }\n"
      "\n"
      "  inline ::mimosa::rpc::Channel::Ptr channel() const { return channel_; }\n"
      "\n"
      );

    printer.Indent();
    printer.Print("// Rpc methods\n");
    for (int mi = 0; mi < service->method_count(); ++mi)
    {
      auto method = service->method(mi);
      fillMethodDict(method, &variables);
      printer.Print(
        variables,
        "inline ::mimosa::rpc::Call< $RequestType$,\n"
        "                            $ResponseType$>::Ptr\n"
        "$MethodName$($RequestType$ * request)\n"
        "{\n"
        "  ::mimosa::rpc::Call< $RequestType$,\n"
        "                       $ResponseType$>::Ptr call(\n"
        "    new ::mimosa::rpc::Call< $RequestType$,\n"
        "                             $ResponseType$>(request));\n"
        "  call->setServiceId($ServiceId$);\n"
        "  call->setMethodId($MethodId$);\n"
        "  channel_->sendCall(call.get());\n"
        "  return call;\n"
        "}\n"
        "\n"
        );
    }
    printer.Print("// End of rpc methods\n");
    printer.Outdent();

    printer.Print(
      "protected:\n"
      "  ::mimosa::rpc::Channel::Ptr channel_;\n"
      "}; // End of service's client\n\n");
    return true;
  }

  static bool generateServiceServer(gpio::Printer &               printer,
                                    const gp::ServiceDescriptor * service,
                                    std::string *                 error)
  {
    std::map<std::string, std::string> variables;
    if (!fillServiceDict(service, &variables, error))
      return false;
    printer.Print(
      variables,
      "class $ServiceName$_Server : public $ServiceName$\n"
      "{\n"
      "public:\n"
      "  MIMOSA_DEF_PTR($ServiceName$_Server);\n"
      "\n"
      );

    printer.Indent();
    printer.Print("// You have to implement the following rpc methods\n");
    for (int mi = 0; mi < service->method_count(); ++mi)
    {
      auto method = service->method(mi);
      fillMethodDict(method, &variables);
      printer.Print(
        variables,
        "virtual bool $MethodName$(\n"
        "  $RequestType$ & request,\n"
        "  $ResponseType$ & response) = 0;\n"
        );
      if (mi + 1 < service->method_count())
        printer.Print("\n");
    }
    printer.Print("// End of rpc methods\n\n");

    printer.Outdent();
    printer.Print(
      "protected:\n"
      "  inline virtual ::mimosa::rpc::Service::CallMethodStatus callMethod(\n"
      "    ::mimosa::rpc::BasicCall::Ptr basic_call,\n"
      "    const char *                  request_data,\n"
      "    uint32_t                      request_size)\n"
      "  {\n"
      "    switch (basic_call->methodId())\n"
      "    {\n"
      );

    printer.Indent(); printer.Indent();
    for (int mi = 0; mi < service->method_count(); ++mi)
    {
      auto method = service->method(mi);
      fillMethodDict(method, &variables);
      printer.Print(
        variables,
        "case $MethodId$: // $MethodName$()\n"
        "{\n"
        "  ::mimosa::rpc::Call< $RequestType$,\n"
        "                       $ResponseType$>::Ptr call(\n"
        "    reinterpret_cast< ::mimosa::rpc::Call< $RequestType$,\n"
        "                                           $ResponseType$> *> (basic_call.get()));\n"
        "  call->allocateMessages();\n"
        "  if (!call->request().ParseFromArray(request_data, request_size))\n"
        "    return ::mimosa::rpc::Service::kInvalidMsg;\n"
        "  if (!$MethodName$(call->request(), call->response()))\n"
        "    call->cancel();\n"
        "  return ::mimosa::rpc::Service::kSucceed;\n"
        "}\n"
        );
    }
    printer.Outdent(); printer.Outdent();

    printer.Print(
      "    default:\n"
      "      return ::mimosa::rpc::Service::kMethodNotFound;\n"
      "    }\n"
      "  }\n"
      "}; // End of service\n\n");
    return true;
  }

  static bool generateService(gpio::Printer &               printer,
                              const gp::ServiceDescriptor * service,
                              std::string *                 error)
  {
    std::map<std::string, std::string> variables;
    if (!fillServiceDict(service, &variables, error))
      return false;
    printer.Print(
      variables,
      "class $ServiceName$_Client;\n"
      "class $ServiceName$_Server;\n"
      "class $ServiceName$HttpHandler;\n"
      "class $ServiceName$HttpClient;\n"
      "class $ServiceName$ : public ::mimosa::rpc::Service\n"
      "{\n"
      "public:\n"
      "  MIMOSA_DEF_PTR($ServiceName$);\n"
      "  typedef $ServiceName$_Client Client;\n"
      "  typedef $ServiceName$_Server Server;\n"
      "  typedef $ServiceName$HttpHandler HttpHandler;\n"
      "  typedef $ServiceName$HttpClient HttpClient;\n"
      "  inline virtual uint32_t id() const { return $ServiceId$; }\n"
      "  inline virtual const char * name() const { return \"$ServiceFullName$\"; }\n"
      "};\n\n"
      );

    return true;
  }

  static bool generateHttpClient(gpio::Printer &               printer,
                                 const gp::ServiceDescriptor * service,
                                 std::string *                 error)
  {
      std::map<std::string, std::string> variables;
      if (!fillServiceDict(service, &variables, error))
        return false;
      printer.Print(
        variables,
        "class $ServiceName$HttpClient : public ::mimosa::RefCountable<$ServiceName$HttpClient>\n"
        "{\n"
        "public:\n"
        "  MIMOSA_DEF_PTR($ServiceName$HttpClient);\n"
        "  inline\n"
        "  $ServiceName$HttpClient(const std::string &base_url)\n"
        "    : base_url_(base_url)\n"
        "  {\n"
        "  }\n"
        "\n"
        );

      printer.Indent(); printer.Indent(); printer.Indent();
      for (int mi = 0; mi < service->method_count(); ++mi)
      {
        auto method = service->method(mi);
        fillMethodDict(method, &variables);
        printer.Print(
          variables,
          "bool $MethodName$(const $RequestType$ &request,\n"
          "  $ResponseType$ *response) const\n"
          "{\n"
          "  return ::mimosa::rpc::httpCall(base_url_ + \"$MethodName$\", request, response);\n"
          "}\n"
          );
      }
      printer.Outdent(); printer.Outdent(); printer.Outdent();

      printer.Print(
        variables,
        "\n"
        "private:\n"
        "  std::string base_url_;\n"
        "};\n\n"
        );

      return true;
  }

  static bool generateHttpHandler(gpio::Printer &               printer,
                                  const gp::ServiceDescriptor * service,
                                  std::string *                 error)
  {
    std::map<std::string, std::string> variables;
    if (!fillServiceDict(service, &variables, error))
      return false;
    printer.Print(
      variables,
      "class $ServiceName$HttpHandler : public ::mimosa::http::Handler\n"
      "{\n"
      "public:\n"
      "  MIMOSA_DEF_PTR($ServiceName$HttpHandler);\n"
      "  inline\n"
      "  $ServiceName$HttpHandler($ServiceName$::Server::Ptr service, const char * start_url)\n"
      "    : service_(service),\n"
      "      start_url_(start_url)\n"
      "  {\n"
      "  }\n"
      "\n"
      "  inline virtual bool\n"
      "  handle(::mimosa::http::RequestReader & request,\n"
      "         ::mimosa::http::ResponseWriter & response) const override\n"
      "  {\n"
      "    if (request.method() != ::mimosa::http::kMethodPost)\n"
      "      return ::mimosa::http::ErrorHandler::basicResponse(\n"
      "        request, response, ::mimosa::http::kStatusMethodNotAllowed);\n"
      "\n"
      "    if (request.location().size() < start_url_.size() ||\n"
      "        strncmp(request.location().c_str(), start_url_.c_str(), start_url_.size()))\n"
      "      return ::mimosa::http::ErrorHandler::basicResponse(\n"
      "        request, response, ::mimosa::http::kStatusInternalServerError);\n"
      "\n"
      "    const std::string method_name(request.location().substr(start_url_.size()));\n"
      "\n"
      "    try {\n"
      );

    printer.Indent(); printer.Indent(); printer.Indent();
    for (int mi = 0; mi < service->method_count(); ++mi)
    {
      auto method = service->method(mi);
      fillMethodDict(method, &variables);
      printer.Print(
        variables,
        "if (method_name == \"$MethodName$\")\n"
        "{\n"
        "  std::unique_ptr< $RequestType$> rq(new $RequestType$);\n"
        "  std::unique_ptr< $ResponseType$> rp(new $ResponseType$);\n"
        "  ::mimosa::rpc::jsonDecode(&request, rq.get());\n"
        "  if (!service_->$MethodName$(*rq, *rp))\n"
        "    return ::mimosa::http::ErrorHandler::basicResponse(\n"
        "      request, response, ::mimosa::http::kStatusServiceUnavailable);\n"
        "  ::mimosa::rpc::jsonEncode(&response, *rp);\n"
        "  return true;\n"
        "}\n"
        );
    }
    printer.Outdent(); printer.Outdent(); printer.Outdent();

    printer.Print(
      variables,
      "    } catch (::mimosa::json::Decoder::ReadError e) {\n"
      "      response.write(\"Read error\", 10);\n"
      "      response.setStatus(::mimosa::http::kStatusBadRequest);\n"
      "      return true;\n"
      "    } catch (::mimosa::json::Decoder::PrematureEof e) {\n"
      "      response.write(\"Premature end of file\", 21);\n"
      "      response.setStatus(::mimosa::http::kStatusBadRequest);\n"
      "      return true;\n"
      "    } catch (::mimosa::json::Decoder::SyntaxError e) {\n"
      "      response.write(\"Syntax error\", 12);\n"
      "      response.setStatus(::mimosa::http::kStatusBadRequest);\n"
      "      return true;\n"
      "    } catch (::mimosa::rpc::InvalidFormat e) {\n"
      "      response.write(\"Invalid format\", 14);\n"
      "      response.setStatus(::mimosa::http::kStatusBadRequest);\n"
      "      return true;\n"
      "    } catch (::mimosa::rpc::MissingRequiredField e) {\n"
      "      response.write(\"Missing required field\", 22);\n"
      "      response.setStatus(::mimosa::http::kStatusBadRequest);\n"
      "      return true;\n"
      "    } catch (::mimosa::rpc::FieldAlreadySet e) {\n"
      "      response.write(\"Field already set\", 17);\n"
      "      response.setStatus(::mimosa::http::kStatusBadRequest);\n"
      "      return true;\n"
      "    } catch (::mimosa::rpc::InvalidEnumValue e) {\n"
      "      response.write(\"invalid enum value\", 18);\n"
      "      response.setStatus(::mimosa::http::kStatusBadRequest);\n"
      "      return true;\n"
      "    } catch (const ::google::protobuf::Message & e) {\n"
      "      ::mimosa::rpc::jsonEncode(&response, e);\n"
      "      return true;\n"
      "    } catch (...) {\n"
      "      return ::mimosa::http::ErrorHandler::basicResponse(\n"
      "        request, response, ::mimosa::http::kStatusInternalServerError);\n"
      "    }\n"
      "\n"
      "    return ::mimosa::http::ErrorHandler::basicResponse(\n"
      "      request, response, ::mimosa::http::kStatusNotFound);\n"
      "  }\n"
      "\n"
      "private:\n"
      "  $ServiceName$::Server::Ptr service_;\n"
      "  std::string start_url_;\n"
      "};\n\n"
      );

    return true;
  }

  bool Generate(const gp::FileDescriptor* file,
                        const std::string&        parameter,
                        gpc::GeneratorContext*    generator_context,
                        std::string*              error) const override
  {
    gpc::cpp::CppGenerator cpp_gen;
    if (!cpp_gen.Generate(file, parameter, generator_context, error))
      return false;

    std::string header_filename(file->name());
    header_filename.replace(header_filename.end() - 5, header_filename.end(), "pb.h");

    if (file->service_count() > 0)
    {
      std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> stream(
            generator_context->OpenForInsert(header_filename, "includes"));
      gpio::Printer printer(stream.get(), '$');
      printer.Print(
        "#include <memory>\n"
        "\n"
        "#include <mimosa/http/handler.hh>\n"
        "#include <mimosa/http/error-handler.hh>\n"
        "#include <mimosa/rpc/service.hh>\n"
        "#include <mimosa/rpc/call.hh>\n"
        "#include <mimosa/rpc/channel.hh>\n"
        "#include <mimosa/rpc/json.hh>\n"
        "#include <mimosa/rpc/exception.hh>\n"
        "#include <mimosa/rpc/http-call.hh>\n");
    }

    for (int i = 0; i < file->service_count(); ++i)
    {
      std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> stream(
            generator_context->OpenForInsert(header_filename, "namespace_scope"));
      gpio::Printer printer(stream.get(), '$');
      if (!generateService(printer, file->service(i), error))
        return false;
      if (!generateServiceServer(printer, file->service(i), error))
        return false;
      if (!generateServiceClient(printer, file->service(i), error))
        return false;
      if (!generateHttpHandler(printer, file->service(i), error))
        return false;
      if (!generateHttpClient(printer, file->service(i), error))
        return false;
    }

    return true;
  }
};

int main(int argc, char ** argv)
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ServiceGenerator generator;
  return gpc::PluginMain(argc, argv, &generator);
}
