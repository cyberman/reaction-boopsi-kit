/*
 * BOOPSIKit v0.1
 * --------------
 * Strict ANSI C89 helper kit for BOOPSI gadget classes (AmigaOS 3.2.3 baseline).
 *
 * Philosophy:
 * - Not a framework. No layout, no theming, no notification layer.
 * - Only reduces repetitive boilerplate: tag apply + changed masks + clamps.
 * - No compiler extensions. No inline keywords. Portable C89.
 *
 * Dependencies:
 * - exec/types.h
 * - utility/tagitem.h (GetTagData())
 *
 * Author/Maintainer: Cyberman
 * License: MIT (see License.txt)
 */

#ifndef BOOPSIKIT_BOOPSI_KIT_H
#define BOOPSIKIT_BOOPSI_KIT_H

#include <exec/types.h>
#include <utility/tagitem.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */
/* Changed mask bits (kit-wide conventions)                                  */
/* ------------------------------------------------------------------------- */

#define BK_CHG_NONE   (0UL)
#define BK_CHG_GEOM   (1UL << 0)  /* geometry/layout relevant */
#define BK_CHG_VIS    (1UL << 1)  /* visibility/disabled/etc. (optional) */
#define BK_CHG_COLOR  (1UL << 2)  /* colors/pens/palette */
#define BK_CHG_VALUE  (1UL << 3)  /* primary value/content */
#define BK_CHG_ALL    (0xFFFFFFFFUL)

/* Gadget-private change bits:
 * Use bits starting at (1UL<<16) and above to avoid collisions with kit bits.
 */

/* ------------------------------------------------------------------------- */
/* Instance header convention                                                 */
/* ------------------------------------------------------------------------- */

struct BK_InstHeader
{
    ULONG changed;   /* OR of BK_CHG_* and gadget-private bits */
    ULONG flags;     /* optional; gadget-defined */
};

/* ------------------------------------------------------------------------- */
/* Small utilities (ANSI C89)                                                */
/* ------------------------------------------------------------------------- */

#define BK_BOOLIFY(v) ((v) ? 1 : 0)

#define BK_CLAMP_ULONG(v, lo, hi) \
    (((v) < (lo)) ? (lo) : (((v) > (hi)) ? (hi) : (v)))

#define BK_CLAMP_UWORD(v, lo, hi) \
    ((UWORD)BK_CLAMP_ULONG((ULONG)(v), (ULONG)(lo), (ULONG)(hi)))

#define BK_MARK_CHANGED(ih, mask) do { (ih)->changed |= (mask); } while(0)
#define BK_CLEAR_CHANGED(ih, mask) do { (ih)->changed &= ~(mask); } while(0)

/* ------------------------------------------------------------------------- */
/* Tag apply helpers                                                         */
/*                                                                           */
/* Design goal:                                                              */
/* - Apply only if tag is present.                                           */
/* - If new value differs: assign field and set changed mask.                */
/* - No C99 features; declarations are at start of macro block.              */
/*                                                                           */
/* Notes:                                                                    */
/* - Uses GetTagData() from utility/tagitem.h                                */
/* - "absent" sentinel is 0xFFFFFFFFUL (safe for most UWORD/ULONG tags).     */
/* ------------------------------------------------------------------------- */

#define BK_TAG_ABSENT  (0xFFFFFFFFUL)

/* Apply ULONG tag */
#define BK_APPLY_ULONG(taglist, tagid, field, ih, mask)                         \
    do {                                                                        \
        ULONG _bk_v;                                                            \
        _bk_v = (ULONG)GetTagData((tagid), (ULONG)BK_TAG_ABSENT, (taglist));    \
        if (_bk_v != (ULONG)BK_TAG_ABSENT) {                                    \
            if ((ULONG)(field) != _bk_v) {                                      \
                (field) = _bk_v;                                                \
                BK_MARK_CHANGED((ih), (mask));                                  \
            }                                                                   \
        }                                                                       \
    } while(0)

/* Apply UWORD tag */
#define BK_APPLY_UWORD(taglist, tagid, field, ih, mask)                         \
    do {                                                                        \
        ULONG _bk_v;                                                            \
        _bk_v = (ULONG)GetTagData((tagid), (ULONG)BK_TAG_ABSENT, (taglist));    \
        if (_bk_v != (ULONG)BK_TAG_ABSENT) {                                    \
            UWORD _bk_nv;                                                       \
            _bk_nv = (UWORD)_bk_v;                                              \
            if ((UWORD)(field) != _bk_nv) {                                     \
                (field) = _bk_nv;                                               \
                BK_MARK_CHANGED((ih), (mask));                                  \
            }                                                                   \
        }                                                                       \
    } while(0)

/* Apply BOOL tag (normalizes to 0/1) */
#define BK_APPLY_BOOL(taglist, tagid, field, ih, mask)                          \
    do {                                                                        \
        ULONG _bk_v;                                                            \
        _bk_v = (ULONG)GetTagData((tagid), (ULONG)BK_TAG_ABSENT, (taglist));    \
        if (_bk_v != (ULONG)BK_TAG_ABSENT) {                                    \
            ULONG _bk_nb;                                                       \
            _bk_nb = (ULONG)BK_BOOLIFY(_bk_v);                                  \
            if ((ULONG)BK_BOOLIFY(field) != _bk_nb) {                           \
                (field) = (ULONG)_bk_nb;                                        \
                BK_MARK_CHANGED((ih), (mask));                                  \
            }                                                                   \
        }                                                                       \
    } while(0)

/* Apply ULONG tag with clamp */
#define BK_APPLY_ULONG_CLAMP(taglist, tagid, field, lo, hi, ih, mask)           \
    do {                                                                        \
        ULONG _bk_v;                                                            \
        _bk_v = (ULONG)GetTagData((tagid), (ULONG)BK_TAG_ABSENT, (taglist));    \
        if (_bk_v != (ULONG)BK_TAG_ABSENT) {                                    \
            ULONG _bk_nv;                                                       \
            _bk_nv = BK_CLAMP_ULONG(_bk_v, (ULONG)(lo), (ULONG)(hi));           \
            if ((ULONG)(field) != _bk_nv) {                                     \
                (field) = _bk_nv;                                               \
                BK_MARK_CHANGED((ih), (mask));                                  \
            }                                                                   \
        }                                                                       \
    } while(0)

/* Apply UWORD tag with clamp */
#define BK_APPLY_UWORD_CLAMP(taglist, tagid, field, lo, hi, ih, mask)           \
    do {                                                                        \
        ULONG _bk_v;                                                            \
        _bk_v = (ULONG)GetTagData((tagid), (ULONG)BK_TAG_ABSENT, (taglist));    \
        if (_bk_v != (ULONG)BK_TAG_ABSENT) {                                    \
            UWORD _bk_nv;                                                       \
            _bk_nv = (UWORD)BK_CLAMP_ULONG(_bk_v, (ULONG)(lo), (ULONG)(hi));    \
            if ((UWORD)(field) != _bk_nv) {                                     \
                (field) = _bk_nv;                                               \
                BK_MARK_CHANGED((ih), (mask));                                  \
            }                                                                   \
        }                                                                       \
    } while(0)

#ifdef __cplusplus
}
#endif

#endif /* BOOPSIKIT_BOOPSI_KIT_H */

