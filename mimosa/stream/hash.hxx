namespace mimosa
{
  namespace stream
  {
    template <typename Ctx,
              void Init(Ctx *),
              void Update(Ctx *, size_t, const uint8_t *),
              void Digest(Ctx *, size_t, uint8_t *),
              size_t Len>
    Hash<Ctx, Init, Update, Digest, Len>::Hash()
    {
      reset();
    }

    template <typename Ctx,
              void Init(Ctx *),
              void Update(Ctx *, size_t, const uint8_t *),
              void Digest(Ctx *, size_t, uint8_t *),
              size_t Len>
    void
    Hash<Ctx, Init, Update, Digest, Len>::reset()
    {
      Init(&ctx_);
    }

    template <typename Ctx,
              void Init(Ctx *),
              void Update(Ctx *, size_t, const uint8_t *),
              void Digest(Ctx *, size_t, uint8_t *),
              size_t Len>
    int64_t
    Hash<Ctx, Init, Update, Digest, Len>::write(const char * data,
                                                uint64_t     nbytes)
    {
      Update(&ctx_, nbytes, (const uint8_t*)data);
      return nbytes;
    }

    template <typename Ctx,
              void Init(Ctx *),
              void Update(Ctx *, size_t, const uint8_t *),
              void Digest(Ctx *, size_t, uint8_t *),
              size_t Len>
    int64_t
    Hash<Ctx, Init, Update, Digest, Len>::read(char *       /*data*/,
                                               uint64_t     /*nbytes*/)
    {
      assert(false && "invalid operation");
      return -1;
    }

    template <typename Ctx,
              void Init(Ctx *),
              void Update(Ctx *, size_t, const uint8_t *),
              void Digest(Ctx *, size_t, uint8_t *),
              size_t Len>
    char *
    Hash<Ctx, Init, Update, Digest, Len>::digest()
    {
      Digest(&ctx_, sizeof (digest_), (uint8_t*)digest_);
      return digest_;
    }
  }
}
