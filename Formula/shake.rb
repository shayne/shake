class Shake < Formula
  desc "Make things happen"
  homepage "https://shakeit.co"
  url "https://github.com/shayne/shake/releases/download/0.0.0/shake-osx.tar.gz"
  version "0.0.0"
  
  def install
    bin.install Dir["shake"]
  end

  test do
    system "shake", "-h"
  end
end

#
# Editor modelines  -  https://www.wireshark.org/tools/modelines.html
#
# Local variables:
# c-basic-offset: 2
# tab-width: 2
# indent-tabs-mode: nil
# End:
#
# vi: set shiftwidth=2 tabstop=2 expandtab:
# :indentSize=2:tabSize=2:noTabs=true:
#
