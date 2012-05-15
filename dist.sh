#! /bin/bash

VERSION=$(<VERSION)
PROJECT=mimosa
DISTDIR=$PROJECT-$VERSION

sed -i "s/PROJECT_NUMBER.*/PROJECT_NUMBER=$VERSION/g" Doxyfile
rm -rf $DISTDIR
mkdir $DISTDIR

cp -R \
    AUTHORS \
    CMakeLists.txt \
    ChangeLog \
    cmake \
    dist.sh \
    Doxyfile \
    LICENSE \
    README \
    mimosa \
    VERSION \
    $DISTDIR

tar -cvvv $DISTDIR | xz -ze9c >$DISTDIR.tar.xz
rm -rf $DISTDIR

md5sum $DISTDIR.tar.xz >$DISTDIR.tar.xz.md5sum
