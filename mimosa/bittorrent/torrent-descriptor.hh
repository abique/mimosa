#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

namespace mimosa { namespace bittorrent {
   struct TorrentDescriptor {
      TorrentDescriptor();

      struct File {
         std::string path_;
         uint64_t length_;
      };

      struct Tracker {
         std::string url_;
         bool is_backup_;
      };

      struct Node {
         std::string host_;
         uint16_t port_;
      };

      struct Sha1 {
         uint8_t bytes_[20];
      };

      struct Sha2 {
         uint8_t bytes_[32];
      };

      std::string name_;
      std::string creation_date_;
      uint64_t length_;
      uint64_t piece_length_;
      bool is_private_;
      std::vector<File> files_;
      std::vector<Tracker> trackers_;
      std::vector<Node> nodes_;
      Sha1 info_hash_v1_;
      Sha2 info_hash_v2_;
      std::vector<Sha1> pieces_;

      void dump(std::ostream &stream);
   };
}} // namespace mimosa::bittorrent
