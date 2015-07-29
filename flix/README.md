# FLiX: FLi eXector utility

FLiX provides a simple command-line executable for running FLi programs.  To
test, try `$ bootstrap.py --test-flix`.  See the `*.pbtxt` files in this
directory for example FLi programs:

  * shell.pbtxt -- Starts a `ok::fli::fsl::FSLShell` program.  Useful for inspecting,
      running, and modifying a serialized FLi program's `env`.
  * oarphsay.pbtxt -- Simple `cowsay`-like program that runs the `ok::fli::fsl::OarphSay`
      Functor.

