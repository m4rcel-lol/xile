# Xile Architecture

Xile is organized as a native C core with thin platform backends.

```
X clients
   |
   v
Xile server lifecycle
   |
   +--> screen/framebuffer state
   +--> input dispatch
   +--> render backend selector
   +--> IceWM launcher
   |
   v
Platform overlay
   |
   +--> Windows: borderless topmost Win32 window, GDI fallback, keyboard hook
   +--> macOS: borderless shielding-level NSWindow, event tap, Metal/CGL capability probes
```

The current tree establishes the process model, configuration, logging, authentication, IPC framing, and platform windows. The production Xorg DDX integration point is represented by `src/server` and is the boundary where the pinned `xorg-server` source tree is integrated.

## IPC

`xile-ctl` sends a 4-byte little-endian length followed by a UTF-8 JSON object. Messages are bounded to 4096 bytes and parsed with fixed buffers.

## Session

Startup creates an MIT-MAGIC-COOKIE-1 entry, exports `DISPLAY` and `XAUTHORITY`, starts the embedded server, then starts IceWM. Shutdown stops IceWM, tears down the backend, removes the authority file, and clears the in-memory cookie.
