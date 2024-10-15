  1           0 LOAD_CONST               0 (<code object what_is_this_function at 0x7fd11e886fa0, file "chal.py", line 1>)
              2 LOAD_CONST               1 ('what_is_this_function')
              4 MAKE_FUNCTION            0
              6 STORE_NAME               0 (what_is_this_function)

  8           8 BUILD_LIST               0
             10 LOAD_CONST               2 ((1775193, 1121822, 719308, 1032531, 1241562, 1604794, 1563042, 966924, 1896931, 352448, 472789, 432288, 799289, 1025726, 1333047, 1253780, 480842, 1273546, 1036331, 1714544, 219964, 1138003, 416633, 63686, 776016, 671535, 667892, 1217336, 1757210, 853059, 1894686, 881925))
             12 LIST_EXTEND              1
             14 STORE_NAME               1 (a)

 10          16 LOAD_CONST               3 (4919)
             18 STORE_NAME               2 (key)

 12          20 LOAD_CONST               4 (<code object <listcomp> at 0x7fd11e887050, file "chal.py", line 12>)
             22 LOAD_CONST               5 ('<listcomp>')
             24 MAKE_FUNCTION            0
             26 LOAD_NAME                1 (a)
             28 GET_ITER
             30 CALL_FUNCTION            1
             32 STORE_NAME               3 (newa)

 13          34 LOAD_NAME                3 (newa)
             36 GET_ITER
        >>   38 FOR_ITER                 6 (to 52)
             40 STORE_NAME               4 (aa)

 14          42 LOAD_NAME                5 (print)
             44 LOAD_NAME                4 (aa)
             46 CALL_FUNCTION            1
             48 POP_TOP
             50 JUMP_ABSOLUTE           19 (to 38)

 13     >>   52 LOAD_CONST               6 (None)
             54 RETURN_VALUE

Disassembly of <code object what_is_this_function at 0x7fd11e886fa0, file "chal.py", line 1>:
  2           0 LOAD_CONST               1 ((2, 5))
              2 UNPACK_SEQUENCE          2
              4 STORE_FAST               1 (a)
              6 STORE_FAST               2 (b)

  3           8 LOAD_GLOBAL              0 (range)
             10 LOAD_FAST                0 (n)
             12 CALL_FUNCTION            1
             14 GET_ITER
        >>   16 FOR_ITER                 9 (to 36)
             18 STORE_FAST               3 (i)

  4          20 LOAD_FAST                2 (b)
             22 LOAD_FAST                1 (a)
             24 LOAD_FAST                2 (b)
             26 BINARY_ADD
             28 ROT_TWO
             30 STORE_FAST               1 (a)
             32 STORE_FAST               2 (b)
             34 JUMP_ABSOLUTE            8 (to 16)

  5     >>   36 LOAD_FAST                1 (a)
             38 RETURN_VALUE

Disassembly of <code object <listcomp> at 0x7fd11e887050, file "chal.py", line 12>:
 12           0 BUILD_LIST               0
              2 LOAD_FAST                0 (.0)
        >>    4 FOR_ITER                 8 (to 22)
              6 STORE_FAST               1 (b)
              8 LOAD_FAST                1 (b)
             10 LOAD_GLOBAL              0 (key)
             12 BINARY_XOR
             14 LOAD_CONST               0 (256)
             16 BINARY_MODULO
             18 LIST_APPEND              2
             20 JUMP_ABSOLUTE            2 (to 4)
        >>   22 RETURN_VALUE
