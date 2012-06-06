#include "copy.hh"

namespace mimosa
{
  namespace bencode
  {
    bool copyValue(Decoder & dec,
                   Encoder & enc)
    {
      int stack = 0;

      do {
        switch (dec.pull()) {

        case kInt:
          if (!enc.pushInt(dec.getInt()))
            return false;
          break;

        case kData:
          if (!enc.pushData(dec.getData()))
            return false;
          break;

        case kList:
          if (!enc.startList())
            return false;
          ++stack;
          break;

        case kDict:
          if (!enc.startDict())
            return false;
          ++stack;
          break;

        case kEnd:
          if (!enc.end())
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
                   Encoder &       enc)
    {
      switch (token) {

      case kInt:
        if (!enc.pushInt(dec.getInt()))
          return false;
        return true;

      case kData:
        if (!enc.pushData(dec.getData()))
          return false;
        return true;

      case kList:
        if (!enc.startList())
          return false;
        return true;

      case kDict:
        if (!enc.startDict())
          return false;
        return true;

      case kEnd:
        if (!enc.end())
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
