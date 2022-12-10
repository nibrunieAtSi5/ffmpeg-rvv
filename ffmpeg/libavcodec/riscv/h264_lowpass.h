#ifndef _LOWPASS_H_
#define _LOWPASS_H_
#include <riscv_vector.h>


__attribute__((always_inline)) static void v_lowpass_u8m1(vuint8m1_t *p_dst0, vuint8m1_t *p_dst1, vuint8m1_t row0, vuint8m1_t row1,
                                                          vuint8m1_t row2, vuint8m1_t row3, vuint8m1_t row4, vuint8m1_t row5,
                                                          vuint8m1_t row6, int vl)
{
    vuint16m2_t dst0 = vwaddu_vv_u16m2(row0, row5, vl);
    vuint16m2_t add00 = vwaddu_vv_u16m2(row1, row4, vl);
    vuint16m2_t add01 = vwaddu_vv_u16m2(row2, row3, vl);

    vuint16m2_t dst1 = vwaddu_vv_u16m2(row1, row6, vl);
    vuint16m2_t add10 = vwaddu_vv_u16m2(row2, row5, vl);
    vuint16m2_t add11 = vwaddu_vv_u16m2(row3, row4, vl);

    vint16m2_t dst0_s = vreinterpret_v_u16m2_i16m2(dst0);
    vint16m2_t dst1_s = vreinterpret_v_u16m2_i16m2(dst1);

    dst0_s = vmacc_vx_i16m2(dst0_s, 20, vreinterpret_v_u16m2_i16m2(add01), vl);
    dst0_s = vmacc_vx_i16m2(dst0_s, -5, vreinterpret_v_u16m2_i16m2(add00), vl);
    dst1_s = vmacc_vx_i16m2(dst1_s, 20, vreinterpret_v_u16m2_i16m2(add11), vl);
    dst1_s = vmacc_vx_i16m2(dst1_s, -5, vreinterpret_v_u16m2_i16m2(add10), vl);

    dst0_s = vmax_vx_i16m2(dst0_s, 0, vl);
    dst1_s = vmax_vx_i16m2(dst1_s, 0, vl);

    dst0 = vreinterpret_v_i16m2_u16m2(dst0_s);
    dst1 = vreinterpret_v_i16m2_u16m2(dst1_s);

    *p_dst0 = vnclipu_wx_u8m1(dst0, 5, vl);
    *p_dst1 = vnclipu_wx_u8m1(dst1, 5, vl);
}

__attribute__((always_inline)) static void v_lowpass_u32m2(vuint32m2_t *p_dst0, vuint32m2_t *p_dst1, vint16m1_t *p_row0, vint16m1_t *p_row1,
                                                           vint16m1_t *p_row2, vint16m1_t *p_row3, vint16m1_t *p_row4, vint16m1_t *p_row5,
                                                           vint16m1_t *p_row6, ptrdiff_t stride, int vl)
{
    vint32m2_t dst0_s = vwadd_vv_i32m2(*p_row0, *p_row5, vl);
    vint32m2_t add00 = vwadd_vv_i32m2(*p_row1, *p_row4, vl);
    vint32m2_t add01 = vwadd_vv_i32m2(*p_row2, *p_row3, vl);

    vint32m2_t dst1_s = vwadd_vv_i32m2(*p_row1, *p_row6, vl);
    vint32m2_t add10 = vwadd_vv_i32m2(*p_row2, *p_row5, vl);
    vint32m2_t add11 = vwadd_vv_i32m2(*p_row3, *p_row4, vl);

    dst0_s = vmacc_vx_i32m2(dst0_s, 20, add01, vl);
    dst0_s = vmacc_vx_i32m2(dst0_s, -5, add00, vl);
    dst1_s = vmacc_vx_i32m2(dst1_s, 20, add11, vl);
    dst1_s = vmacc_vx_i32m2(dst1_s, -5, add10, vl);

    dst0_s = vmax_vx_i32m2(dst0_s, 0, vl);
    dst1_s = vmax_vx_i32m2(dst1_s, 0, vl);

    *p_dst0 = vreinterpret_v_i32m2_u32m2(dst0_s);
    *p_dst1 = vreinterpret_v_i32m2_u32m2(dst1_s);
}

__attribute__((always_inline)) static void h_lowpass_i16m1(vint16m1_t *p_dst0, vint16m1_t *p_dst1, const uint8_t **pp_src, ptrdiff_t stride, int vl)
{
    vuint8mf2_t row00 = vle8_v_u8mf2(*pp_src - 2, vl + 5);
    vuint8mf2_t row01 = vslidedown_vx_u8mf2(row01, row00, 1, vl + 5);
    vuint8mf2_t row02 = vslidedown_vx_u8mf2(row02, row00, 2, vl + 5);
    vuint8mf2_t row03 = vslidedown_vx_u8mf2(row03, row00, 3, vl + 5);
    vuint8mf2_t row04 = vslidedown_vx_u8mf2(row04, row00, 4, vl + 5);
    vuint8mf2_t row05 = vslidedown_vx_u8mf2(row05, row00, 5, vl + 5);
    *pp_src += stride;

    vuint8mf2_t row10 = vle8_v_u8mf2(*pp_src - 2, vl + 5);
    vuint8mf2_t row11 = vslidedown_vx_u8mf2(row11, row10, 1, vl + 5);
    vuint8mf2_t row12 = vslidedown_vx_u8mf2(row12, row10, 2, vl + 5);
    vuint8mf2_t row13 = vslidedown_vx_u8mf2(row13, row10, 3, vl + 5);
    vuint8mf2_t row14 = vslidedown_vx_u8mf2(row14, row10, 4, vl + 5);
    vuint8mf2_t row15 = vslidedown_vx_u8mf2(row15, row10, 5, vl + 5);
    *pp_src += stride;

    vuint16m1_t dst0_u = vwaddu_vv_u16m1(row00, row05, vl);
    vuint16m1_t add00 = vwaddu_vv_u16m1(row01, row04, vl);
    vuint16m1_t add01 = vwaddu_vv_u16m1(row02, row03, vl);

    vuint16m1_t dst1_u = vwaddu_vv_u16m1(row10, row15, vl);
    vuint16m1_t add10 = vwaddu_vv_u16m1(row11, row14, vl);
    vuint16m1_t add11 = vwaddu_vv_u16m1(row12, row13, vl);

    *p_dst0 = vreinterpret_v_u16m1_i16m1(dst0_u);
    *p_dst1 = vreinterpret_v_u16m1_i16m1(dst1_u);

    *p_dst0 = vmacc_vx_i16m1(*p_dst0, 20, vreinterpret_v_u16m1_i16m1(add01), vl);
    *p_dst0 = vmacc_vx_i16m1(*p_dst0, -5, vreinterpret_v_u16m1_i16m1(add00), vl);
    *p_dst1 = vmacc_vx_i16m1(*p_dst1, 20, vreinterpret_v_u16m1_i16m1(add11), vl);
    *p_dst1 = vmacc_vx_i16m1(*p_dst1, -5, vreinterpret_v_u16m1_i16m1(add10), vl);
}

__attribute__((always_inline)) static void h_lowpass_u16m2(vuint16m2_t *p_dst0, vuint16m2_t *p_dst1, const uint8_t **pp_src, ptrdiff_t stride, int vl)
{
    vuint8m1_t row00 = vle8_v_u8m1(*pp_src - 2, vl + 5);
    vuint8m1_t row01 = vslidedown_vx_u8m1(row01, row00, 1, vl + 5);
    vuint8m1_t row02 = vslidedown_vx_u8m1(row02, row00, 2, vl + 5);
    vuint8m1_t row03 = vslidedown_vx_u8m1(row03, row00, 3, vl + 5);
    vuint8m1_t row04 = vslidedown_vx_u8m1(row04, row00, 4, vl + 5);
    vuint8m1_t row05 = vslidedown_vx_u8m1(row05, row00, 5, vl + 5);
    *pp_src += stride;

    vuint8m1_t row10 = vle8_v_u8m1(*pp_src - 2, vl + 5);
    vuint8m1_t row11 = vslidedown_vx_u8m1(row11, row10, 1, vl + 5);
    vuint8m1_t row12 = vslidedown_vx_u8m1(row12, row10, 2, vl + 5);
    vuint8m1_t row13 = vslidedown_vx_u8m1(row13, row10, 3, vl + 5);
    vuint8m1_t row14 = vslidedown_vx_u8m1(row14, row10, 4, vl + 5);
    vuint8m1_t row15 = vslidedown_vx_u8m1(row15, row10, 5, vl + 5);
    *pp_src += stride;

    *p_dst0 = vwaddu_vv_u16m2(row00, row05, vl);
    vuint16m2_t add00 = vwaddu_vv_u16m2(row01, row04, vl);
    vuint16m2_t add01 = vwaddu_vv_u16m2(row02, row03, vl);

    *p_dst1 = vwaddu_vv_u16m2(row10, row15, vl);
    vuint16m2_t add10 = vwaddu_vv_u16m2(row11, row14, vl);
    vuint16m2_t add11 = vwaddu_vv_u16m2(row12, row13, vl);

    vint16m2_t dst0_s = vreinterpret_v_u16m2_i16m2(*p_dst0);
    vint16m2_t dst1_s = vreinterpret_v_u16m2_i16m2(*p_dst1);

    dst0_s = vmacc_vx_i16m2(dst0_s, 20, vreinterpret_v_u16m2_i16m2(add01), vl);
    dst0_s = vmacc_vx_i16m2(dst0_s, -5, vreinterpret_v_u16m2_i16m2(add00), vl);
    dst1_s = vmacc_vx_i16m2(dst1_s, 20, vreinterpret_v_u16m2_i16m2(add11), vl);
    dst1_s = vmacc_vx_i16m2(dst1_s, -5, vreinterpret_v_u16m2_i16m2(add10), vl);

    dst0_s = vmax_vx_i16m2(dst0_s, 0, vl);
    dst1_s = vmax_vx_i16m2(dst1_s, 0, vl);

    *p_dst0 = vreinterpret_v_i16m2_u16m2(dst0_s);
    *p_dst1 = vreinterpret_v_i16m2_u16m2(dst1_s);
}

__attribute__((always_inline)) static void h_lowpass_u8m1_l2src(vuint8m1_t *p_dst0, vuint8m1_t *p_dst1, const uint8_t **pp_src, ptrdiff_t stride, int vl)
{
    vuint8m1_t row00 = vle8_v_u8m1(*pp_src - 2, vl + 5);
    vuint8m1_t row01 = vslidedown_vx_u8m1(row01, row00, 1, vl + 5);
    vuint8m1_t row02 = vslidedown_vx_u8m1(row02, row00, 2, vl + 5);
    vuint8m1_t row03 = vslidedown_vx_u8m1(row03, row00, 3, vl + 5);
    vuint8m1_t row04 = vslidedown_vx_u8m1(row04, row00, 4, vl + 5);
    vuint8m1_t row05 = vslidedown_vx_u8m1(row05, row00, 5, vl + 5);
    *pp_src += stride;

    vuint8m1_t row10 = vle8_v_u8m1(*pp_src - 2, vl + 5);
    vuint8m1_t row11 = vslidedown_vx_u8m1(row11, row10, 1, vl + 5);
    vuint8m1_t row12 = vslidedown_vx_u8m1(row12, row10, 2, vl + 5);
    vuint8m1_t row13 = vslidedown_vx_u8m1(row13, row10, 3, vl + 5);
    vuint8m1_t row14 = vslidedown_vx_u8m1(row14, row10, 4, vl + 5);
    vuint8m1_t row15 = vslidedown_vx_u8m1(row15, row10, 5, vl + 5);
    *pp_src += stride;

    vuint16m2_t dst0_u = vwaddu_vv_u16m2(row00, row05, vl);
    vuint16m2_t add00 = vwaddu_vv_u16m2(row01, row04, vl);
    vuint16m2_t add01 = vwaddu_vv_u16m2(row02, row03, vl);

    vuint16m2_t dst1_u = vwaddu_vv_u16m2(row10, row15, vl);
    vuint16m2_t add10 = vwaddu_vv_u16m2(row11, row14, vl);
    vuint16m2_t add11 = vwaddu_vv_u16m2(row12, row13, vl);

    vint16m2_t dst0_s = vreinterpret_v_u16m2_i16m2(dst0_u);
    vint16m2_t dst1_s = vreinterpret_v_u16m2_i16m2(dst1_u);

    dst0_s = vmacc_vx_i16m2(dst0_s, 20, vreinterpret_v_u16m2_i16m2(add01), vl);
    dst0_s = vmacc_vx_i16m2(dst0_s, -5, vreinterpret_v_u16m2_i16m2(add00), vl);
    dst1_s = vmacc_vx_i16m2(dst1_s, 20, vreinterpret_v_u16m2_i16m2(add11), vl);
    dst1_s = vmacc_vx_i16m2(dst1_s, -5, vreinterpret_v_u16m2_i16m2(add10), vl);

    dst0_s = vmax_vx_i16m2(dst0_s, 0, vl);
    dst1_s = vmax_vx_i16m2(dst1_s, 0, vl);

    dst0_u = vreinterpret_v_i16m2_u16m2(dst0_s);
    dst1_u = vreinterpret_v_i16m2_u16m2(dst1_s);

    *p_dst0 = vnclipu_wx_u8m1(dst0_u, 5, vl);
    *p_dst1 = vnclipu_wx_u8m1(dst1_u, 5, vl);

    *p_dst0 = vaaddu_vv_u8m1(*p_dst0, row02, vl);
    *p_dst1 = vaaddu_vv_u8m1(*p_dst1, row12, vl);    
}

__attribute__((always_inline)) static void h_lowpass_u8m1_l2src_shift(vuint8m1_t *p_dst0, vuint8m1_t *p_dst1, const uint8_t **pp_src, ptrdiff_t stride, int vl)
{
    vuint8m1_t row00 = vle8_v_u8m1(*pp_src - 2, vl + 5);
    vuint8m1_t row01 = vslidedown_vx_u8m1(row01, row00, 1, vl + 5);
    vuint8m1_t row02 = vslidedown_vx_u8m1(row02, row00, 2, vl + 5);
    vuint8m1_t row03 = vslidedown_vx_u8m1(row03, row00, 3, vl + 5);
    vuint8m1_t row04 = vslidedown_vx_u8m1(row04, row00, 4, vl + 5);
    vuint8m1_t row05 = vslidedown_vx_u8m1(row05, row00, 5, vl + 5);
    *pp_src += stride;

    vuint8m1_t row10 = vle8_v_u8m1(*pp_src - 2, vl + 5);
    vuint8m1_t row11 = vslidedown_vx_u8m1(row11, row10, 1, vl + 5);
    vuint8m1_t row12 = vslidedown_vx_u8m1(row12, row10, 2, vl + 5);
    vuint8m1_t row13 = vslidedown_vx_u8m1(row13, row10, 3, vl + 5);
    vuint8m1_t row14 = vslidedown_vx_u8m1(row14, row10, 4, vl + 5);
    vuint8m1_t row15 = vslidedown_vx_u8m1(row15, row10, 5, vl + 5);
    *pp_src += stride;

    vuint16m2_t dst0_u = vwaddu_vv_u16m2(row00, row05, vl);
    vuint16m2_t add00 = vwaddu_vv_u16m2(row01, row04, vl);
    vuint16m2_t add01 = vwaddu_vv_u16m2(row02, row03, vl);

    vuint16m2_t dst1_u = vwaddu_vv_u16m2(row10, row15, vl);
    vuint16m2_t add10 = vwaddu_vv_u16m2(row11, row14, vl);
    vuint16m2_t add11 = vwaddu_vv_u16m2(row12, row13, vl);

    vint16m2_t dst0_s = vreinterpret_v_u16m2_i16m2(dst0_u);
    vint16m2_t dst1_s = vreinterpret_v_u16m2_i16m2(dst1_u);

    dst0_s = vmacc_vx_i16m2(dst0_s, 20, vreinterpret_v_u16m2_i16m2(add01), vl);
    dst0_s = vmacc_vx_i16m2(dst0_s, -5, vreinterpret_v_u16m2_i16m2(add00), vl);
    dst1_s = vmacc_vx_i16m2(dst1_s, 20, vreinterpret_v_u16m2_i16m2(add11), vl);
    dst1_s = vmacc_vx_i16m2(dst1_s, -5, vreinterpret_v_u16m2_i16m2(add10), vl);

    dst0_s = vmax_vx_i16m2(dst0_s, 0, vl);
    dst1_s = vmax_vx_i16m2(dst1_s, 0, vl);

    dst0_u = vreinterpret_v_i16m2_u16m2(dst0_s);
    dst1_u = vreinterpret_v_i16m2_u16m2(dst1_s);

    *p_dst0 = vnclipu_wx_u8m1(dst0_u, 5, vl);
    *p_dst1 = vnclipu_wx_u8m1(dst1_u, 5, vl);

    *p_dst0 = vaaddu_vv_u8m1(*p_dst0, row03, vl);
    *p_dst1 = vaaddu_vv_u8m1(*p_dst1, row13, vl); 
}
#endif
