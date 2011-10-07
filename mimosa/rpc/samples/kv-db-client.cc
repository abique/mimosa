#include <cerrno>
#include <cstdio>

#include <gflags/gflags.h>
#include <mimosa/init.hh>
#include <mimosa/rpc/channel.hh>
#include <mimosa/net/connect.hh>
#include <mimosa/stream/fd-stream.hh>

#include "kv-db.pb.h"

using namespace mimosa;

DEFINE_string(host, "localhost", "the host to connect to");
DEFINE_int32(port, 4242, "the port to connect to");
DEFINE_string(op, "set", "the operation (get, set, del)");
DEFINE_string(key, "", "the key");
DEFINE_string(value, "", "the value");

int main(int argc, char ** argv)
{
  mimosa::init(argc, argv);

  /* connect to the server */
  int fd = net::connectToHost(FLAGS_host, FLAGS_port);
  if (fd < 0)
  {
    printf("failed to connect to %s: %s\n", FLAGS_host.c_str(), ::strerror(errno));
    return 1;
  }

  /* establish the rpc channel */
  rpc::Channel::Ptr channel(new rpc::Channel(new stream::FdStream(fd)));
  channel->start();

  /* create our service client */
  rpc::samples::pb::Database::Client db(channel);
  if (FLAGS_op == "get")
  {
    // setup the request
    auto request = new rpc::samples::pb::Key;
    request->set_key(FLAGS_key);

    // do the call
    auto call = db.get(request);
    call->wait(); // wait for the call to get
    if (call->isCanceled()) // check if an error occured
      printf(" -- get canceled\n");
    else
      printf(" -- get(%s): %s\n", FLAGS_key.c_str(), call->response().value().c_str());
  }
  else if (FLAGS_op == "set")
  {
    // setup the request
    auto request = new rpc::samples::pb::KeyValue;
    request->set_key(FLAGS_key);
    request->set_value(FLAGS_value);

    // do the call
    auto call = db.set(request);
    call->wait(); // wait for the call to get
    if (call->isCanceled()) // check if an error occured
      printf(" -- set canceled\n");
    else
      printf(" -- set(%s): %s\n", FLAGS_key.c_str(), FLAGS_value.c_str());
  }
  else if (FLAGS_op == "del")
  {
    // setup the request
    auto request = new rpc::samples::pb::Key;
    request->set_key(FLAGS_key);

    // do the call
    auto call = db.del(request);
    call->wait(); // wait for the call to get
    if (call->isCanceled()) // check if an error occured
      printf(" -- del canceled\n");
    else
      printf(" -- del(%s)\n", FLAGS_key.c_str());
  }
  channel->close();

  mimosa::deinit();

  return 0;
}
