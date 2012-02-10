#include "../copy.hh"
#include "../fd-stream.hh"
#include "../lzma-encoder.hh"

int main(int argc, char **argv)
{
  mimosa::stream::Stream::Ptr in = new mimosa::stream::FdStream(0);
  mimosa::stream::Stream::Ptr out = new mimosa::stream::FdStream(1);
  mimosa::stream::Stream::Ptr lzma = new mimosa::stream::LzmaEncoder(out);

  mimosa::stream::copy(*in, *lzma);
  lzma->flush();
  return 0;
}
