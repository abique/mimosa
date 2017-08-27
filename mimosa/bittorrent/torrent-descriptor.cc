#include "torrent-descriptor.hh"
#include "../stream/base16-encoder.hh"
#include "../format/print.hh"

namespace mimosa
{
  namespace bittorrent
  {
    TorrentDescriptor::TorrentDescriptor()
      : length_(0),
        piece_length_(0),
        is_private_(false)
    {
    }

    void
    TorrentDescriptor::dump(std::ostream &stream)
    {
      stream << "Name: " << name_ << std::endl
             << "Creation Date: " << creation_date_ << std::endl
             << "Length: " << length_ << std::endl
             << "Piece Length: " << piece_length_ << std::endl
             << "Is Private: " << is_private_ << std::endl
             << "Files:" << std::endl;
      for (auto &file : files_)
        stream << "  - " << file.path_ << " " << file.length_ << std::endl;
      stream << "Trackers: " << std::endl;
      for (auto &tracker : trackers_)
        stream << tracker.url_ << ", is backup: " << tracker.is_backup_ << std::endl;
      stream << "Nodes: " << std::endl;
      for (auto &node : nodes_)
        stream << node.host_ << " " << node.port_ << std::endl;
      stream << "Info Hash: " << std::endl;
    }
  }
}
