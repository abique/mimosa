#include "copy.hh"

namespace mimosa
{
  namespace bencode
  {
    bool copyValue(Decoder & dec,
                   Encoder & enc,
                   Time      timeout)
    {
      int stack = 0;

      do {
        switch (dec.pull(timeout)) {

        case kInt:
          if (!enc.pushInt(dec.getInt(), timeout))
            return false;
          break;

        case kData:
          if (!enc.pushData(dec.getData(), timeout))
            return false;
          break;

        case kList:
          if (!enc.startList(timeout))
            return false;
          ++stack;
          break;

        case kDict:
          if (!enc.startDict(timeout))
            return false;
          ++stack;
          break;

        case kEnd:
          if (!enc.end(timeout))
            return false;
          --stack;
          break;

        case kEof:
        case kReadError:
        case kParseError:
          return false;
        }
      } while (stack > 0);

      return true;

    }

    bool copyToken(Token           token,
                   const Decoder & dec,
                   Encoder &       enc,
                   Time            timeout)
    {
      switch (token) {

      case kInt:
        if (!enc.pushInt(dec.getInt(), timeout))
          return false;
        return true;

      case kData:
        if (!enc.pushData(dec.getData(), timeout))
          return false;
        return true;

      case kList:
        if (!enc.startList(timeout))
          return false;
        return true;

      case kDict:
        if (!enc.startDict(timeout))
          return false;
        return true;

      case kEnd:
        if (!enc.end(timeout))
          return false;
        return true;

      case kEof:
      case kReadError:
      case kParseError:
      default:
        return false;
      }
    }
  }
}
