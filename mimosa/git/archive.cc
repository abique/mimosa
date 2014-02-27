#include <mimosa/archive/entry.hh>

#include "archive.hh"
#include "blob.hh"

namespace mimosa
{
  namespace git
  {
    bool gitArchive(Tree &              tree,
                    archive::Writer &   archive,
                    const std::string & prefix)
    {
      for (size_t i = 0; i < git_tree_entrycount(tree); ++i) {
        const git_tree_entry *entry = git_tree_entry_byindex(tree, i);
        if (!entry)
          break;

        mimosa::archive::Entry e(archive);

        std::string path = prefix + git_tree_entry_name(entry);

        archive_entry_set_pathname(e, path.c_str());
        archive_entry_set_mode(e, git_tree_entry_filemode_raw(entry));
        switch (git_tree_entry_filemode(entry)) {
        case GIT_FILEMODE_TREE: {
          archive_entry_set_filetype(e, S_IFDIR);
          archive_entry_set_perm(e, 0755);
          archive_write_header(archive, e);

          Tree child(git_tree_owner(tree), git_tree_entry_id(entry));
          if (child)
            gitArchive(child, archive, path + "/");
          break;
        }

        case GIT_FILEMODE_BLOB:
        case GIT_FILEMODE_BLOB_EXECUTABLE: {
          Blob blob;
          if (git_blob_lookup(blob.ref(), git_tree_owner(tree), git_tree_entry_id(entry)))
            break;
          archive_entry_set_filetype(e, S_IFREG);
          if (git_tree_entry_filemode(entry) == GIT_FILEMODE_BLOB)
            archive_entry_set_perm(e, 0644);
          else
            archive_entry_set_perm(e, 0755);
          archive_entry_set_size(e, git_blob_rawsize(blob));
          archive_write_header(archive, e);
          archive_write_data(archive, git_blob_rawcontent(blob), git_blob_rawsize(blob));
          break;
        }

        case GIT_FILEMODE_LINK:
          archive_entry_set_filetype(e, S_IFLNK);
          break;

        default:
          break;
        }
      }
      return true;
    }
  }
}
