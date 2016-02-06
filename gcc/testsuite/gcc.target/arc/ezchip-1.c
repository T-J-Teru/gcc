/* { dg-do compile } */
/* { dg-options "-mq-class -mbitops -munaligned-access -mcmem -O2 -fno-strict-aliasing" } */

enum ezdp_mem_space_type {
  EZDP_EXTERNAL_MS = 1
};
struct ezdp_ext_addr {
  struct {
    struct {
      enum ezdp_mem_space_type mem_type : 1;
      unsigned msid : 5;
    };
  };
  char user_space[];
} a;
char b;
void fn1() {
  ((struct ezdp_ext_addr *)a.user_space)->mem_type = EZDP_EXTERNAL_MS;
  ((struct ezdp_ext_addr *)a.user_space)->msid =
      ((struct ezdp_ext_addr *)a.user_space)->mem_type ? 1 : 10;
  while (b)
    ;
}
