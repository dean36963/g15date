#Maintainer: Dean Birch <dean.birch0@gmail.com>
#notetoself - $srcdir/pgk/usr/bin/g15date
pkgname=g15date-svn
pkgver=0.2
pkgrel=1
pkgdesc="Displays date and time (including an analogue clock) for the G15 keyboard LCD screen."
arch=('i686')
license=('Unknown')
depends=('g15daemon-svn' 'libg15render-svn' 'libg15-svn')
source=(https://github.com/downloads/dean36963/g15date/g15date-svn-0.2-1.tar.gz/$pkgname-$pkgver-$pkgrel.tar.gz)
md5sums=('2a7c496d8ababa3c16e9f938c033dd66')



build() {
#  mkdir "$srcdir/$pkgname-$pkgver"
 # cd "$srcdir/$pkgname-$pkgver"
	cd "$srcdir"
  make
}

package() {
	mkdir -p "$pkgdir/usr/bin/"
	cp "$srcdir/g15date" "$pkgdir/usr/bin/"
}

