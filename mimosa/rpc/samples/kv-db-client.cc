#include <cerrno>
#include <cstdio>

#include <mimosa/init.hh>
#include <mimosa/options/options.hh>
#include <mimosa/rpc/channel.hh>
#include <mimosa/net/connect.hh>
#include <mimosa/stream/fd-stream.hh>

#include "kv-db.pb.h"

using namespace mimosa;

std::string & HOST  = *options::addOption<std::string>("", "host", "the host to connect to", "localhost");
uint16_t &    PORT  = *options::addOption<uint16_t>("", "port", "the port to connect to", 4242);
std::string & OP    = *options::addOption<std::string>("", "op", "the operation (get, set, del)", "set");
std::string & KEY   = *options::addOption<std::string>("", "key", "the key", "");
std::string & VALUE = *options::addOption<std::string>("", "value", "the value", "");

int main(int argc, char ** argv)
{
  mimosa::init(argc, argv);

  /* connect to the server */
  int fd = net::connectToHost(HOST, PORT);
  if (fd < 0)
  {
    printf("failed to connect to %s: %s\n", HOST.c_str(), ::strerror(errno));
    return 1;
  }

  /* establish the rpc channel */
  rpc::Channel::Ptr channel(new rpc::Channel(new stream::FdStream(fd)));

  /* create our service client */
  rpc::samples::pb::Database::Client db(channel);
  if (OP == "get")
  {
    // setup the request
    auto request = new rpc::samples::pb::Key;
    request->set_key(KEY);

    // do the call
    auto call = db.get(request);
    call->wait(); // wait for the call to get
    if (call->isCanceled()) // check if an error occured
      printf(" -- get canceled\n");
    else
      printf(" -- get(%s): %s\n", KEY.c_str(), call->response().value().c_str());
  }
  else if (OP == "set")
  {
    // setup the request
    auto request = new rpc::samples::pb::KeyValue;
    request->set_key(KEY);
    request->set_value(VALUE);

    // do the call
    auto call = db.set(request);
    call->wait(); // wait for the call to get
    if (call->isCanceled()) // check if an error occured
      printf(" -- set canceled\n");
    else
      printf(" -- set(%s): %s\n", KEY.c_str(), VALUE.c_str());
  }
  else if (OP == "del")
  {
    // setup the request
    auto request = new rpc::samples::pb::Key;
    request->set_key(KEY);

    // do the call
    auto call = db.del(request);
    call->wait(); // wait for the call to get
    if (call->isCanceled()) // check if an error occured
      printf(" -- del canceled\n");
    else
      printf(" -- del(%s)\n", KEY.c_str());
  }
  channel->close();

  mimosa::deinit();

  return 0;
}
