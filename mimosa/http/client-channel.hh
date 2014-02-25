#ifndef MIMOSA_HTTP_CLIENT_CHANNEL_HH
# define MIMOSA_HTTP_CLIENT_CHANNEL_HH

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class ClientChannel : private NonCopyable
    {
    public:
      ClientChannel(stream::BufferedStream::Ptr stream);
      ~ClientChannel();

      inline void setRemoteAddr(const ::sockaddr * addr, ::socklen_t addr_len)
      {
        addr_     = addr;
        addr_len_ = addr_len;
      }

      inline const ::sockaddr * remoteAddr() const { return addr_; }
      inline ::socklen_t remoteAddrLen() const { return addr_len_; }

      inline void setReadTimeout(Time timeout) { read_timeout_ = timeout; }
      inline void setWriteTimeout(Time timeout) { write_timeout_ = timeout; }

      inline bool isSsl() const { return is_ssl_; }
      inline void setSsl(bool is_ssl) { is_ssl_ = is_ssl; }

    private:
      friend class MessageReader<ServerChannel, Request>;
      friend class MessageWriter<ServerChannel, Response>;

      stream::BufferedStream::Ptr stream_;
      const ::sockaddr *          addr_;
      ::socklen_t                 addr_len_;
      Time                        read_timeout_;
      Time                        write_timeout_;
      bool                        is_ssl_;
    };
  }
}

#endif /* !MIMOSA_HTTP_CLIENT_CHANNEL_HH */
