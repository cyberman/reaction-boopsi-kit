# BOOPSIKit (ReAction / AmigaOS 3.2.3)

Strict **ANSI C89** helper kit for writing **BOOPSI gadget classes** on **AmigaOS 3.2.3** (NDK **3.2R4** baseline).  
No MUI. No compiler extensions. No new frameworks — just less boilerplate.

**Author/Maintainer:** Cyberman  
**License:** MIT

---

## What this is

BOOPSIKit is a tiny developer-oriented helper kit that standardizes the repetitive parts of BOOPSI/ReAction gadget development:

- Tag parsing with **changed masks**
- Clamp / bool normalization helpers
- A consistent instance header convention (`changed` / `flags`)
- Conservative refresh policy (no hidden event loops)

---

## What this is NOT

- No layout system (use `layout.gadget`)
- No theming/skin engine
- No notification abstraction layer
- No global caches / singletons

---

## Baseline / Requirements

- **AmigaOS 3.2.3** (primary target)
- **NDK 3.2R4** headers/protos
- **Strict ANSI C89** in all C sources
- Toolchain: any C89 compiler  
  (Disk classes may use a small 68k asm entry stub for ABI glue.)

---

## Repository layout

- `Developer/Include/boopsikit/boopsi_kit.h` — public header
- `Developer/Source/boopsikit/boopsi_kit.c` — optional drop-in C module
- `Examples/lamp/` — reference gadget (lamp.gadget)
- `Docs/` — quickstart + notes

---

## Quickstart (Dev)

### Option A: Header-only (recommended)

1. Copy `Developer/Include/boopsikit/boopsi_kit.h` into your include path.
2. In your gadget sources:

```c
#include <boopsikit/boopsi_kit.h>
```

3. Ensure your instance data starts with:

```c
struct BK_InstHeader { ULONG changed; ULONG flags; };
```

4. Use `BK_APPLY_*` macros in `OM_NEW` / `OM_SET` to apply tags and set changed bits.

### Option B: Header + drop-in C module

Copy `Developer/Source/boopsikit/boopsi_kit.c` into your project and compile/link it.

---

## Example: lamp.gadget

This repo includes **lamp.gadget** as a reference BOOPSI gadget:

- Display-only status lamp (5×5 .. 9×9)
- State colors + RGB32 mode
- ReAction-friendly (embed via `layout.gadget`)

`lamp.gadget` is also intended to be released separately as its own gadget.

---

## Release philosophy

- System-conform AmigaOS 3.2.3 approach
- Minimal surface area, explicit ownership, deterministic cleanup
- No “clever” compiler tricks — portability first

---

## License

MIT License. See `License.txt`.

