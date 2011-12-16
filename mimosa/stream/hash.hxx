namespace mimosa
{
  namespace stream
  {
    template <gnutls_digest_algorithm_t Algo, size_t Len>
    Hash<Algo, Len>::Hash()
      : handle_(nullptr)
    {
      reset();
    }

    template <gnutls_digest_algorithm_t Algo, size_t Len>
    Hash<Algo, Len>::~Hash()
    {
      if (handle_)
      {
        gnutls_hash_deinit(handle_, nullptr);
        handle_ = nullptr;
      }
    }

    template <gnutls_digest_algorithm_t Algo, size_t Len>
    bool
    Hash<Algo, Len>::reset()
    {
      if (handle_)
      {
        gnutls_hash_deinit(handle_, nullptr);
        handle_ = nullptr;
      }

      return !gnutls_hash_init(&handle_, Algo);
    }

    template <gnutls_digest_algorithm_t Algo, size_t Len>
    int64_t
    Hash<Algo, Len>::write(const char * data,
                           uint64_t     nbytes,
                           runtime::Time /*timeout*/)
    {
      if (!handle_)
        return -1;

      if (!gnutls_hash(handle_, data, nbytes))
        return nbytes;
      return -1;
    }

    template <gnutls_digest_algorithm_t Algo, size_t Len>
    int64_t
    Hash<Algo, Len>::read(char *       /*data*/,
                          uint64_t     /*nbytes*/,
                          runtime::Time /*timeout*/)
    {
      assert(false && "invalid operation");
      return -1;
    }

    template <gnutls_digest_algorithm_t Algo, size_t Len>
    char *
    Hash<Algo, Len>::digest()
    {
      gnutls_hash_output(handle_, digest_);
      return digest_;
    }
  }
}
