#Maintainer: Dean Birch <dean.birch0@gmail.com>
pkgname=g15date-svn
pkgver=0.2
pkgrel=1
pkgdesc="Displays date and time (including an analogue clock) for the G15 keyboard LCD screen."
arch=('i686')
license=('Unknown')
depends=('g15daemon-svn' 'libg15render-svn' 'libg15-svn')
source=(https://dean36963@github.com/dean36963/g15date.git/$pkgname-$pkgver-$pkgrel.tar.gz)
md5sums=('f7bf3d0d5dad1da672c64776543c2360')


build() {
  mkdir "$srcdir/$pkgname-$pkgver"
  cd "$srcdir/$pkgname-$pkgver"
  make
}

package() {
  cd "$srcdir/$pkgname-$pkgver"
  make install
}


