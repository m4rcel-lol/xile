# Third-Party Sources

Git does not track empty directories, so each vendor directory contains a `.gitkeep`
marker until the pinned upstream source is fetched.

Populate these directories with the exact versions listed in `docs/BUILD.md` before
building the full production Xorg/IceWM stack. The marker files are intentionally
small and safe to leave in place; source archives can overwrite the directory
contents without affecting the build.
