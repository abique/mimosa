#ifndef MIMOSA_RPC_PROTOCOL_HH
# define MIMOSA_RPC_PROTOCOL_HH

# include <endian.h>
# include <cstdint>

namespace mimosa
{
  namespace rpc
  {
    enum MsgType
    {
      kClose  = 0x01,
      kCall   = 0x02,
      kResult = 0x03,
      kError  = 0x04,
    };

    enum ErrorType
    {
      kSucceed         = 0x00,
      kCancel          = 0x01,
      kInvalidMsg      = 0x02,
      kServiceNotFound = 0x03,
      kMethodNotFound  = 0x04,
      kInternalError   = 0x05,
      kDuplicateTag    = 0x06,
      kNotImplemented  = 0x07,
    };

    enum TagOrigin
    {
      kOriginMe  = 0x00,
      kOriginYou = 0x01,
    };

    struct Msg
    {
      uint8_t type_;
    } __attribute__((packed));

    struct MsgClose : public Msg
    {
    } __attribute__((packed));

    struct MsgCall : public Msg
    {
      uint32_t tag_;
      uint32_t service_id_;
      uint32_t method_id_;
      uint32_t rq_size_;
      char     rq_[0];
    } __attribute__((packed));

    struct MsgResult : public Msg
    {
      uint32_t tag_;
      uint32_t rp_size_;
      char     rp_[0];
    } __attribute__((packed));

    struct MsgError : public Msg
    {
      uint32_t tag_;
      uint8_t  tag_origin_;
      uint8_t  error_;
    } __attribute__((packed));
  }
}

#endif /* !MIMOSA_RPC_PROTOCOL_HH */
